#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <stack>

#include "Parser.h"
#include "StringUtil.h"
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
    m_nextToken = getCurrentLineToken();
  }
  return 0;
}

void Parser::parseForProcedure() {
  if (isMatchToken("procedure")) {
    std::string procName = getMatchToken(tokentype::tokenType::PROC_NAME);
    if (!isMatchToken("{")) {
      throw SyntaxOpenBraceException(m_curLineNum);
    }
    m_curProcIdx = m_pkbWriteOnly->insertProcedure(procName);
    LIST_OF_STMT_NUMS stmtLst;
    parseStmtLst(stmtLst);
  }
}

void Parser::parseStmtLst(LIST_OF_STMT_NUMS& t_stmtInStmtLst) {
  // Parse the rest of the code in the
  parseStmt(t_stmtInStmtLst);
  if (isMatchToken("}")) {
    // Remove from back
    if (!m_nestedStmtLineNum.empty()) {
      m_nestedStmtLineNum.pop_back();
    }
    return;
  }
  parseStmtLst(t_stmtInStmtLst);
}

void Parser::parseStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst) {
  if (isMatchToken(EMPTY_LINE)) {
    return;
  }
  m_curLineNum += 1;
  m_pkbWriteOnly->insertFollowsRelation(t_stmtInStmtLst, m_curLineNum);
  m_pkbWriteOnly->insertParentRelation(m_nestedStmtLineNum, m_curLineNum);
  t_stmtInStmtLst.push_back(m_curLineNum);
  if (t_stmtInStmtLst.size() == 1) {
    m_pkbWriteOnly->insertStmtList(m_curLineNum);
  }
  if (isNonContainerStmt(m_nextToken)) {
    parseNonContainerStmt(t_stmtInStmtLst);
  } else {
    parseContainerStmt(t_stmtInStmtLst);
  }
}

void Parser::parseNonContainerStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst) {
  if (isMatchToken("call")) {
    parseCallStmt();
  } else {
    parseAssignStmt();
  }
  if (!isMatchToken(";")) {
    throw SyntaxNoEndOfStatmentException(m_curLineNum);
  }
}

void Parser::parseAssignStmt() {
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
  m_pkbWriteOnly->insertModifies(m_curProcIdx, varName, m_nestedStmtLineNum, m_curLineNum);
  if (!isMatchToken("=")) {
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  } 
  LIST_OF_TOKENS tokenisedExpr = tokeniseExpr();
  m_pkbWriteOnly->insertAssignStmt(m_curLineNum, varName, tokenisedExpr);
}

void Parser::parseCallStmt() {
  std::string procName = getMatchToken(tokentype::PROC_NAME);
  m_pkbWriteOnly->insertCallStmt(m_curProcIdx, procName, m_curLineNum);
}

std::vector<std::string> Parser::tokeniseExpr() {
  std::vector<std::string> output;
  std::string term = getMatchToken(tokentype::tokenType::VAR_NAME);
  if (!isConstant(term) && !isValidName(term)) {
    throw SyntaxUnknownCommandException("Assignment terms must be an operator or a constant or a variable", m_curLineNum);
  }
  output.push_back(term);
  if (isConstant(term)) {
    m_pkbWriteOnly->insertConstant(term);
  } else if (isValidName(term)) {
    m_pkbWriteOnly->insertUses(m_curProcIdx, term, m_nestedStmtLineNum, m_curLineNum);
  }
  while (isOperator(m_nextToken)) {
    std::string opr = getMatchToken(tokentype::tokenType::EXPR);
    output.push_back(opr);
    term = getMatchToken(tokentype::tokenType::VAR_NAME);
    if (isConstant(term)) {
      m_pkbWriteOnly->insertConstant(term);
    } else if (isValidName(term)) {
      m_pkbWriteOnly->insertUses(m_curProcIdx, term, m_nestedStmtLineNum, m_curLineNum);
    }
    output.push_back(term);
  }
  return output;
}

void Parser::parseContainerStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst) {
  m_nestedStmtLineNum.push_back(m_curLineNum);
  if (isMatchToken("while")) {
    parseWhileStmt(t_stmtInStmtLst);
  } else if (isMatchToken("if")) {
    parseIfStmt(t_stmtInStmtLst);
    parseElseStmt(t_stmtInStmtLst);
  } else {
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  }
}

void Parser::parseWhileStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst) {
  std::string varName = getMatchToken(tokentype::tokenType::VAR_NAME);
  if (!isMatchToken("{")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  m_pkbWriteOnly->insertWhileStmt(m_curProcIdx, varName, m_nestedStmtLineNum, m_curLineNum);
  LIST_OF_STMT_NUMS whileStmtLst;
  parseStmtLst(whileStmtLst);
}

void Parser::parseIfStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst) {
  std::string varName = getMatchToken(tokentype::tokenType::VAR_NAME);
  if (!isMatchToken("then")) {
    throw SyntaxUnknownCommandException("If statements require 'then' keyword", m_curLineNum);
  }
  if (!isMatchToken("{")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  m_pkbWriteOnly->insertIfStmt(m_curProcIdx, varName, m_nestedStmtLineNum, m_curLineNum);
  LIST_OF_STMT_NUMS ifStmtLst;
  parseStmtLst(ifStmtLst);
}

void Parser::parseElseStmt(LIST_OF_STMT_NUMS& t_stmtInStmtLst) {
  if (!isMatchToken("else")) {
    throw SyntaxUnknownCommandException("If statements require 'else' keyword", m_curLineNum);
  }
  if (!isMatchToken("{")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  LIST_OF_STMT_NUMS elseStmtLst;
  parseStmtLst(elseStmtLst);
}

bool Parser::isMatchToken(const STRING_TOKEN& t_token) {
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
    case tokentype::tokenType::EXPR:
      if (isOperator(m_nextToken)) {
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

STRING_TOKEN Parser::getMatchToken(const tokentype::tokenType &t_token) {
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

STRING_TOKEN Parser::getCurrentLineToken() {
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

bool Parser::isOperator(const STRING_TOKEN& t_token) {
  return t_token == "+"
    || t_token == "-"
    || t_token == "*"
    || t_token == "=";
}

bool Parser::isBrace(const STRING_TOKEN& t_token) {
  return t_token == "{" || t_token == "}";
}

bool Parser::isKeyDelimiter(const STRING_TOKEN& t_token) {
  return isBrace(t_token) 
    || isOperator(t_token) 
    || t_token == " " 
    || t_token == ";";
}

LIST_OF_TOKENS Parser::tokeniseLine(const STRING_TOKEN& t_line) {
  std::string formatString = StringUtil::reduceString(t_line);
  LIST_OF_TOKENS tokens;
  STRING_TOKEN token = EMPTY_LINE;
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

bool Parser::isValidName(const STRING_TOKEN& t_token) {
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

bool Parser::isConstant(const STRING_TOKEN& t_token) {
  for (auto& cToken : t_token) {
    if (!isdigit(cToken)) {
      return false;
    }
  }
  return true;
}

bool Parser::isNonContainerStmt(const STRING_TOKEN& t_token) {
  return t_token != "while" && t_token != "if";
}