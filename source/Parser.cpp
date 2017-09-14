#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <stack>

#include "Parser.h"
#include "StringUtil.h"
#include "nodes\TNode.h"
#include "SyntaxOpenBraceException.h"
#include "SyntaxNoEndOfStatementException.h"
#include "SyntaxUnknownCommandException.h"
#include "SyntaxEmptyLineException.h"

int Parser::parse (const std::string &t_filename) throw(SyntaxErrorException) {
  m_readStream = std::ifstream (t_filename);
  if (!m_readStream.is_open()) {
    return -1;
  }
  m_nextToken = getCurrentLineToken();
  while (!m_readStream.eof()) {
    parseForProcedure();
  }
  return 0;
}

int Parser::parseForProcedure() throw (SyntaxErrorException) {
  // Construct the AST based on the parsed line
  // Remove unecessary spaces, tabs	
  if (isMatchToken("procedure")) {
    ProcedureNode *procNode = m_builder.createProcedure(getMatchToken(tokenType::PROC_NAME));
    m_pkb->insertProcToAST(procNode);
    TNode *stmtLst = m_builder.createStmtList(m_curLineNum);
    m_builder.linkParentToChild(procNode, stmtLst);
    if (!isMatchToken("{")) {
      throw SyntaxOpenBraceException(m_curLineNum);
    }
    return parseStmtLst((StmtListNode*)stmtLst);
  }
  return -1;
}

int Parser::parseStmtLst(StmtListNode *t_node) throw (SyntaxErrorException) {
  // Parse the rest of the code in the
  m_curLineNum += 1;
  if (t_node->getChildren()->size() == 0) {
    m_pkb->insertFollows(TNode::NO_LINE_NUM, m_curLineNum);
  } else {
    m_pkb->insertFollows(t_node->getChildren()->back()->getLineNum(), m_curLineNum);
  }
  parseStmt(t_node);
  if (isMatchToken("}")) {
    // Remove from back
    if (!m_nestedStmtLineNo.empty()) {
      m_nestedStmtLineNo.pop_back();
    }
    return 1;
  }
  return parseStmtLst(t_node);
}

int Parser::parseStmt(TNode *t_node) throw (SyntaxErrorException) {
  if (isMatchToken("")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  // Var name
  if (m_nextToken != "while" && m_nextToken != "if") {
    parseAssignStmt(t_node);
    m_pkb->insertParent(t_node->getLineNum(), m_curLineNum);
    if (!isMatchToken(";")) {
      throw SyntaxNoEndOfStatmentException(m_curLineNum);
    }
  }
  else {
    // Parse container stmts
    m_pkb->insertParent(t_node->getLineNum(), m_curLineNum);
    parseContainerStmt(t_node);
  }
  return 1;
}

int Parser::parseAssignStmt(TNode* t_node) throw(SyntaxErrorException) {
  std::string varName = getMatchToken(tokenType::VAR_NAME);
  if (isConstant(varName) || !isValidName(varName)) {
    throw SyntaxUnknownCommandException("Var name is not valid", m_curLineNum);
  }
  VariableNode *left = m_builder.createVariable(m_curLineNum, varName, DUMMY_INDEX);
  VAR_INDEX varIndx = m_pkb->insertModifiesForStmt(left->getVarName(), m_curLineNum); // Wire in the uses case
  for (auto containerItr = m_nestedStmtLineNo.begin(); containerItr != m_nestedStmtLineNo.end(); containerItr++) {
    m_pkb->insertModifiesForStmt(left->getVarName(), (*containerItr));
  }
  if (!isMatchToken("=")) {
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  }
  TNode* exprNode = parseExpr();
  AssignNode* stmt = m_builder.buildAssignment(m_curLineNum, left, exprNode);
  m_pkb->insertStatementTypeTable(Grammar::GType::ASGN, m_curLineNum);
  m_pkb->insertTypeOfStatementTable(m_curLineNum, Grammar::GType::ASGN);
  m_pkb->insertAssignRelation(varIndx, stmt);
  m_builder.linkParentToChild(t_node, stmt);
  return 1;
}

TNode* Parser::parseExpr() throw (SyntaxErrorException) {
  std::stack<TNode *> exprStack;
  std::string name = getMatchToken(tokenType::VAR_NAME);
  if (isConstant(name)) {
    ConstantNode* constNode = m_builder.createConstant(m_curLineNum, atoi(name.c_str()));
  } else if (!isValidName(name)) {
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  }
  VariableNode* varNode = m_builder.createVariable(m_curLineNum, name, DUMMY_INDEX);
  m_pkb->insertUsesForStmt(varNode->getVarName(), m_curLineNum);
  for (auto containerItr = m_nestedStmtLineNo.begin(); containerItr != m_nestedStmtLineNo.end(); containerItr++) {
    m_pkb->insertUsesForStmt(varNode->getVarName(), *containerItr);
  }
  exprStack.push(varNode);
  while (m_nextToken == "+") {
    if (exprStack.empty() != true && isMatchToken("+")) {
      name = getMatchToken(tokenType::VAR_NAME);
      TNode* right = m_builder.createVariable(m_curLineNum, name, DUMMY_INDEX);
      m_pkb->insertUsesForStmt(name, m_curLineNum);
      for (auto containerItr = m_nestedStmtLineNo.begin(); containerItr != m_nestedStmtLineNo.end(); containerItr++) {
        m_pkb->insertUsesForStmt(name, *containerItr);
      }
      TNode* left = exprStack.top();
      exprStack.pop();
      PlusNode* plusNode = m_builder.buildAddition(m_curLineNum, left, right);
      exprStack.push(plusNode);
      continue;
    }
    VariableNode* varNode = m_builder.createVariable(m_curLineNum, m_nextToken, DUMMY_INDEX);
    exprStack.push(varNode);
  }
  TNode *childNode = exprStack.top();
  return childNode;
}

int Parser::parseContainerStmt(TNode* t_node) throw(SyntaxErrorException) {
  m_nestedStmtLineNo.push_back(m_curLineNum);
  if (isMatchToken("while")) {
    parseWhileStmt((WhileNode*)t_node);
  } else if (isMatchToken("if")) {
  } else {
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  }
  return 1;
}

int Parser::parseWhileStmt(TNode* t_node) throw(SyntaxErrorException) {
  VariableNode* varNode = m_builder.createVariable(m_curLineNum, getMatchToken(tokenType::VAR_NAME), DUMMY_INDEX);
  if (!isMatchToken("{")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  StmtListNode* stmtLstNode = m_builder.createStmtList(m_curLineNum);
  WhileNode *whileNode = m_builder.buildWhile(m_curLineNum, varNode, stmtLstNode);
  m_pkb->insertStatementTypeTable(Grammar::GType::WHILE, m_curLineNum);
  m_pkb->insertTypeOfStatementTable(m_curLineNum, Grammar::GType::WHILE);
  m_pkb->insertUsesForStmt(varNode->getVarName(), m_curLineNum);
  for (auto containerItr = m_nestedStmtLineNo.begin(); containerItr != m_nestedStmtLineNo.end(); containerItr++) {
    m_pkb->insertUsesForStmt(varNode->getVarName(), (*containerItr));
  }
  parseStmtLst(stmtLstNode);
  m_builder.linkParentToChild(t_node, whileNode);
  return 1;
}

bool Parser::isMatchToken(const std::string &t_token) throw(SyntaxErrorException) {
  if (m_nextToken == t_token) {
    m_nextToken = getCurrentLineToken();
    return true;
  }
  return false;
}

bool Parser::isMatchToken(tokenType t_type) throw (SyntaxErrorException) {
  switch (t_type) {
    case tokenType::PROC_NAME:
    case tokenType::VAR_NAME:
      if (!isKeyDelimiter(m_nextToken)) {
        m_nextToken = getCurrentLineToken();
        return true;
      }
      break;
    default:
      assert(true);
      break;
  }
  return false;
}

std::string Parser::getMatchToken(const tokenType &t_token) throw(SyntaxErrorException) {
  std::string output = m_nextToken;
  switch (t_token) {
    case tokenType::PROC_NAME:
    case tokenType::VAR_NAME:
    case tokenType::CONSTANT:
      m_nextToken = getCurrentLineToken();
      break;
    case tokenType::EXPR :
      m_nextToken = getCurrentLineToken();
      break;
    default:
      assert(true);
      break;
  }
  return output;
}

std::string Parser::getCurrentLineToken() {
  if (curTokens.empty()) {
    std::string extractedLine;
    if (getline(m_readStream, extractedLine)) {
      try {
        curTokens = tokeniseLine(extractedLine);
        return getToken();
      } catch (SyntaxEmptyLineException sele) {
        return "";
      }
    }
    return "";
  }
  return getToken();
}

std::string Parser::getToken() throw(SyntaxErrorException) {
  /*assert(!curTokens.empty());*/
  if (curTokens.empty()) {
    throw SyntaxEmptyLineException();
  }
  std::string token = curTokens.front();
  curTokens.erase(curTokens.begin());
  return token;
}

bool Parser::isOperator(const std::string &t_token) {
  return t_token == "+"
    || t_token == "-"
    || t_token == "*"
    || t_token == "=";
}

bool Parser::isBrace(const std::string &t_token) {
  return t_token == "{" || t_token == "}";
}

bool Parser::isKeyDelimiter(const std::string &t_token) {
  return isBrace(t_token) 
    || isOperator(t_token) 
    || t_token == " " 
    || t_token == ";";
}

std::vector<std::string> Parser::tokeniseLine(const std::string &t_line) {
  std::string formatString = StringUtil::reduceString(t_line);
  std::vector<std::string> tokens;
  std::string token = "";
  for (std::string::iterator itr = formatString.begin(); itr != formatString.end(); itr++) {
     const std::string curStrChar = std::string(1, (*itr));
    if (isKeyDelimiter(curStrChar) && token != "") {
      tokens.push_back(token);
      token = "";
      if (curStrChar != " ") {
        tokens.push_back(curStrChar);
      }
      continue;
    }
    if (isOperator(curStrChar)) {
      tokens.push_back(curStrChar);
      token = "";
      continue;
    }
    if (curStrChar != " ") {
      token += (*itr);
    }
  }
  if (token.length() > 0) {
    tokens.push_back(token);
    token = "";
  }
  return tokens;
}

bool Parser::isValidName(std::string& t_token) throw(SyntaxErrorException) {
  if (isdigit(t_token[0]) || !isalpha(t_token[0])) {
    return false;
  }
  for (auto &cToken : t_token) {
    if (!isalpha(cToken) || !isdigit(cToken)) {
      return false;
    }
  }
  return true;
}

bool Parser::isConstant(std::string& t_token) throw(SyntaxErrorException) {
  for (auto& cToken : t_token) {
    if (!isdigit(cToken)) {
      return false;
    }
  }
  return true;
}