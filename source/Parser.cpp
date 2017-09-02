#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "Parser.h"
#include "StringUtil.h"
#include "TNode.h"

int Parser::parse (const std::string &t_filename) {
  m_readStream = ifstream (t_filename);
  if (!m_readStream.is_open()) {
    return -1;
  }
  m_nextToken = getCurrentLineToken();
  while (!m_readStream.eof()) {
    parseForProcedure();
    m_nextToken = getCurrentLineToken();
  }
  return 0;
}

int Parser::parseForProcedure() {
  // Construct the AST based on the parsed line
  // Remove unecessary spaces, tabs	
  if (matchToken("procedure")) {
    TNode *procNode = m_builder.createProcedure(m_nextToken);
    m_pkb->setProcToAST(m_curProcNum, procNode);
    TNode *stmtLst = m_builder.createStmtList();
    m_builder.linkParentToChild(procNode, stmtLst);

    matchToken(tokenType::PROC_NAME);
    matchToken("{");
    return parseStmtLst(stmtLst);
  }
  return -1;
}

int Parser::parseStmtLst(TNode *t_node) {
  // Parse the rest of the code in the
  parseStmt(t_node);
  matchToken(";");
  if (m_nextToken == "}") {
    return 1;
  }
  return parseStmtLst(t_node);
}

int Parser::parseStmt(TNode *t_node) {
  m_curLineNum += 1;
  // Var name
  if (m_nextToken != "while" || m_nextToken != "if") {
    parseAssignStmt(t_node);
  }
  else {
    // Parse container stmts
    parseContainerStmt(t_node);
  }
  return 1;
}

int Parser::parseAssignStmt(TNode *t_node) {
  TNode *left = m_builder.createVariable(m_curLineNum, m_nextToken);
  matchToken(tokenType::VAR_NAME);
  matchToken("=");
  TNode *right = m_builder.createVariable(m_curLineNum, m_nextToken);
  matchToken(tokenType::EXPR);
  TNode *stmt = m_builder.buildAssignment(m_curLineNum, left, right);
  m_builder.linkParentToChild(t_node, stmt);

  return 1;
}

int Parser::parseContainerStmt(TNode *t_node) {

}

bool Parser::parseForBraces(const std::string &t_token) {
  // Iterate through the tokens to identify braces
  if (t_token == "{") {
    m_bracesStack.push(t_token);
    return true;
  }
  if (m_bracesStack.empty()) {
    return false;
  }
  m_bracesStack.pop();
  return true;
}

bool Parser::matchToken(const std::string &t_token) {
  if (m_nextToken == t_token) {
    m_nextToken = getCurrentLineToken();
    return true;
  }
  return false;
}

bool Parser::matchToken(const tokenType &t_token) {
  switch (t_token) {
    case tokenType::PROC_NAME:
      // Update proc name with line num
      cout << "Proc name: " << m_nextToken << "\n";
      m_nextToken = getCurrentLineToken();
      m_pkb->setProcToAST(m_curProcNum++, new TNode());
      break;
    case tokenType::VAR_NAME:
      // Var name with line num
      cout << "Var name: " << m_nextToken << " = ";
      m_nextToken = getCurrentLineToken();
      break;
    case tokenType::EXPR :
      // const val with line num
      cout << "Const val: " << m_nextToken << "\n";
      m_nextToken = getCurrentLineToken();
      break;
    default:
      assert(true);
      break;
  }
  return true;
}

std::string Parser::getCurrentLineToken() {
  if (curTokens.empty()) {
    std::string extractedLine;
    if (getline(m_readStream, extractedLine)) {
      curTokens = tokeniseLine(extractedLine);
      return getToken();
    }
    return "";
  }
  return getToken();
}

std::string Parser::getToken() {
  assert(!curTokens.empty());
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

