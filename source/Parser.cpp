#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <stack>

#include "Parser.h"
#include "StringUtil.h"
#include "TNode.h"
#include "SyntaxOpenBraceException.h"
#include "SyntaxNoEndOfStatementException.h"
#include "SyntaxUnknownCommandException.h"
#include "SyntaxEmptyLineException.h"

int Parser::parse (const std::string &t_filename) throw(SyntaxErrorException) {
  m_readStream = ifstream (t_filename);
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
    TNode *procNode = m_builder.createProcedure(getMatchToken(tokenType::PROC_NAME));
    m_pkb->setProcToAST(m_curProcNum, procNode);
    TNode *stmtLst = m_builder.createStmtList();
    m_builder.linkParentToChild(procNode, stmtLst);
    if (!isMatchToken("{")) {
      throw SyntaxOpenBraceException(m_curLineNum);
    }
    return parseStmtLst(stmtLst);
  }
  return -1;
}

int Parser::parseStmtLst(TNode *t_node) throw (SyntaxErrorException) {
  // Parse the rest of the code in the
  parseStmt(t_node);
  if (isMatchToken("}")) {
    return 1;
  }
  return parseStmtLst(t_node);
}

int Parser::parseStmt(TNode *t_node) throw (SyntaxErrorException) {
  if (isMatchToken("")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  m_curLineNum += 1;
  // Var name
  if (m_nextToken != "while" && m_nextToken != "if") {
    parseAssignStmt(t_node);
    if (!isMatchToken(";")) {
      throw SyntaxNoEndOfStatmentException(m_curLineNum);
    }
  }
  else {
    // Parse container stmts
    parseContainerStmt(t_node);
  }
  return 1;
}

int Parser::parseAssignStmt(TNode *t_node) throw(SyntaxErrorException) {
  TNode *left = m_builder.createVariable(m_curLineNum, getMatchToken(tokenType::VAR_NAME));
  if (!isMatchToken("=")) {
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  }
  TNode *right = m_builder.createVariable(m_curLineNum, getMatchToken(tokenType::EXPR));
  TNode *stmt = m_builder.buildAssignment(m_curLineNum, left, right);
  m_builder.linkParentToChild(t_node, stmt);

  return 1;
}

int Parser::parseExpr(TNode* t_node) throw (SyntaxErrorException) {
  std::stack<TNode *> exprStack;
  while (isMatchToken("+") || isMatchToken(tokenType::VAR_NAME)) {
    if (((VariableNode*)exprStack.top())->getVarName() == "+") {
      // Create plus Node
      // PlusNode right = TNode* varNode = m_builder.createVariable(m_curLineNum, m_nextToken);
      // Pop plus node
      // pop the prev var node
      // push back
      continue;
    }
    TNode* varNode = m_builder.createVariable(m_curLineNum, m_nextToken);
    exprStack.push(varNode);
  }

  TNode *childNode = exprStack.top();
  m_builder.linkParentToChild(t_node, childNode);
  return 1;
}

int Parser::parseContainerStmt(TNode *t_node) throw(SyntaxErrorException) {
  if (isMatchToken("while")) {
    parseWhileStmt(t_node);
  } else if (isMatchToken("if")) {
  } else {
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  }
  return 1;
}

int Parser::parseWhileStmt(TNode *t_node) throw(SyntaxErrorException) {
  TNode *varNode = m_builder.createVariable(m_curLineNum, getMatchToken(tokenType::VAR_NAME));
  if (!isMatchToken("{")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  TNode *stmtLstNode = m_builder.createStmtList();
  parseStmtLst(stmtLstNode);
  TNode *whileNode = m_builder.buildWhile(m_curLineNum, varNode, stmtLstNode);
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
    const std::string curStrChar = string(1, (*itr));
    if (isKeyDelimiter(curStrChar) && token != "") {
      tokens.push_back(token);
      token = "";
      if (curStrChar != " ") {
        tokens.push_back(curStrChar);
      }
      continue;
    }
    token += (*itr);
  }
  if (token.length() > 0) {
    tokens.push_back(token);
    token = "";
  }
  return tokens;
}

