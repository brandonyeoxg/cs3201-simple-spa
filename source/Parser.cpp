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

int Parser::parse (const std::string &t_filename) throw() {
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

int Parser::parseForProcedure() {
  // Construct the AST based on the parsed line
  // Remove unecessary spaces, tabs	
  if (isMatchToken("procedure")) {
    std::string procName = getMatchToken(tokentype::tokenType::PROC_NAME);
    if (!isMatchToken("{")) {
      throw SyntaxOpenBraceException(m_curLineNum);
    }
    StmtListNode* stmtLst = m_pkbWriteOnly->insertProcedure(procName);
    return parseStmtLst(stmtLst);
  }
  return -1;
}

int Parser::parseStmtLst(StmtListNode *t_node) {
  // Parse the rest of the code in the
  parseStmt(t_node);
  if (isMatchToken("}")) {
    // Remove from back
    if (!m_nestedStmtLineNum.empty()) {
      m_nestedStmtLineNum.pop_back();
    }
    return 1;
  }
  return parseStmtLst(t_node);
}

int Parser::parseStmt(TNode *t_node) {
  if (isMatchToken(EMPTY_LINE)) {
    return 1;
  }
  m_curLineNum += 1;
  m_pkbWriteOnly->insertFollowsRelation(t_node, m_curLineNum);
  m_pkbWriteOnly->insertParent(t_node->getLineNum(), m_curLineNum);
  if (isNonContainerStmt()) {
    parseNonContainerStmt(t_node);
  }
  else {
    parseContainerStmt(t_node);
  }
  return 1;
}

int Parser::parseNonContainerStmt(TNode* t_node) {
  parseAssignStmt(t_node);
  if (!isMatchToken(";")) {
    throw SyntaxNoEndOfStatmentException(m_curLineNum);
  }
  return 0;
}

int Parser::parseAssignStmt(TNode* t_node) {
  std::string varName = getMatchToken(tokentype::tokenType::VAR_NAME);
  if (varName == "") {
    throw SyntaxOpenBraceException(m_curLineNum - 1);
  }
  if (!isValidName(varName)) {
    throw SyntaxUnknownCommandException("Error occurred unable to parse command", m_curLineNum);
  }
  if (isConstant(varName) && !isValidName(varName)) {
    throw SyntaxUnknownCommandException("Var name is not valid", m_curLineNum);
  }
  VariableNode* left = m_pkbWriteOnly->insertModifiedVariable(varName, m_curLineNum, m_nestedStmtLineNum);
  if (!isMatchToken("=")) {
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  }
  TNode* exprNode = parseExpr();
  m_pkbWriteOnly->insertAssignStmt(t_node, left, exprNode, m_curLineNum);
  return 1;
}

TNode* Parser::parseExpr() {
  std::stack<TNode *> exprStack;
  std::string name = getMatchToken(tokentype::tokenType::VAR_NAME);
  if (isConstant(name)) {
    ConstantNode* constNode = m_pkbWriteOnly->insertConstant(name, m_curLineNum);    
    m_pkbWriteOnly->insertConstant(name, m_curLineNum);
    exprStack.push(constNode);
  } else if (!isValidName(name)) {
    throw SyntaxUnknownCommandException("Not a valid variable name", m_curLineNum);
  } else {
    VariableNode* varNode = m_pkbWriteOnly->insertUsesVariable(name, m_curLineNum, m_nestedStmtLineNum);
    exprStack.push(varNode);
  }
  while (m_nextToken == "+") {
    if (exprStack.empty() == true || !isMatchToken("+")) {
      break;
    }
    parseEachOperand(exprStack);
  }
  TNode *childNode = exprStack.top();
  return childNode;
}

void Parser::parseEachOperand(std::stack<TNode *>& t_exprStack) {
  std::string name = getMatchToken(tokentype::tokenType::VAR_NAME);
  TNode* right;
  if (isConstant(name)) {
    right = m_pkbWriteOnly->insertConstant(name, m_curLineNum);
  } else if (!isValidName(name)) {
    throw SyntaxUnknownCommandException("Not a valid variable name", m_curLineNum);
  } else {
    right = m_pkbWriteOnly->insertUsesVariable(name, m_curLineNum, m_nestedStmtLineNum);
  }
  TNode* left = t_exprStack.top();
  t_exprStack.pop();
  PlusNode* plusNode = m_pkbWriteOnly->insertPlusOp(left, right, m_curLineNum);
  t_exprStack.push(plusNode);
}

int Parser::parseContainerStmt(TNode* t_node) {
  m_nestedStmtLineNum.push_back(m_curLineNum);
  if (isMatchToken("while")) {
    parseWhileStmt((WhileNode*)t_node);
  } else if (isMatchToken("if")) {
  } else {
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  }
  return 1;
}

int Parser::parseWhileStmt(TNode* t_node) {
  VariableNode* varNode = m_pkbWriteOnly->insertUsesVariable(getMatchToken(tokentype::tokenType::VAR_NAME), m_curLineNum, m_nestedStmtLineNum);
  if (!isMatchToken("{")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  StmtListNode* stmtLstNode = m_pkbWriteOnly->insertWhileStmt(t_node, varNode, m_curLineNum);
  parseStmtLst(stmtLstNode);
  return 1;
}

bool Parser::isMatchToken(const std::string &t_token) {
  if (m_nextToken == t_token) {
    m_nextToken = getCurrentLineToken();
    return true;
  }
  return false;
}

bool Parser::isMatchToken(tokentype::tokenType t_type) {
  switch (t_type) {
    case tokentype::tokenType::PROC_NAME:
    case tokentype::tokenType::VAR_NAME:
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

std::string Parser::getMatchToken(const tokentype::tokenType &t_token) {
  std::string output = m_nextToken;
  switch (t_token) {
    case tokentype::tokenType::PROC_NAME:
    case tokentype::tokenType::VAR_NAME:
    case tokentype::tokenType::CONSTANT:
    case tokentype::tokenType::EXPR :
      m_nextToken = getCurrentLineToken();
      break;
    default:
      assert(true);
      break;
  }
  return output;
}

std::string Parser::getCurrentLineToken() {
  if (!m_curTokens.empty()) {
    return getToken();
  }
  std::string extractedLine;
  if (getline(m_readStream, extractedLine)) {
    try {
      m_curTokens = tokeniseLine(extractedLine);
      return getToken();
    } catch (SyntaxEmptyLineException sele) {
      return EMPTY_LINE;
    }
  }
  return EMPTY_LINE;  
}

std::string Parser::getToken() {
  if (m_curTokens.empty()) {
    throw SyntaxEmptyLineException();
  }
  std::string token = m_curTokens.front();
  m_curTokens.erase(m_curTokens.begin());
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
  std::string token = EMPTY_LINE;
  for (auto itr = formatString.begin(); itr != formatString.end(); itr++) {
    const std::string curStrChar = std::string(1, (*itr));
    if (isKeyDelimiter(curStrChar) && token != EMPTY_LINE) { // Tokenise the words
      tokens.push_back(token);
      token = EMPTY_LINE;
    }
    if (isOperator(curStrChar)) { // Tokenise operator
      tokens.push_back(curStrChar);
      token = EMPTY_LINE;
      continue;
    }
    if (curStrChar != " ") {
      token += (*itr);
    }
  }
  if (token.length() > 0) {
    tokens.push_back(token);
    token = EMPTY_LINE;
  }
  return tokens;
}

bool Parser::isValidName(std::string& t_token) {
  if (t_token.size() == 0) {
    return false;
  }
  if (isdigit(t_token[0]) && !isalpha(t_token[0])) {
    return false;
  }
  for (auto &cToken : t_token) {
    if (!isalpha(cToken) && !isdigit(cToken)) {
      return false;
    }
  }
  return true;
}

bool Parser::isConstant(std::string& t_token) {
  for (auto& cToken : t_token) {
    if (!isdigit(cToken)) {
      return false;
    }
  }
  return true;
}

bool Parser::isNonContainerStmt() {
  return m_nextToken != "while" && m_nextToken != "if";
}