#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;

#include "Parser.h"
#include "StringUtil.h"
#include "nodes/TNode.h"


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
    matchToken(tokenType::PROC_NAME);
    matchToken("{");
    return parseStmtLst();
  }
  return -1;
}

int Parser::parseStmtLst() {
  // Parse the rest of the code in the
  parseStmt();
  matchToken(";");
  if (m_nextToken == "}") {
    return 1;
  }
  return parseStmtLst();
}

int Parser::parseStmt() {
  m_curLineNum += 1;
  // Var name
  matchToken(tokenType::VAR_NAME);
  matchToken("=");
  matchToken(tokenType::EXPR);
  return 1;
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

bool Parser::parseForVariable(const string &t_token) {
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
  assert(!curTokens.empty(), "Token list must not be empty");
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

