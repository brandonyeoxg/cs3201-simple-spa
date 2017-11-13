#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <stack>

#include "Parser.h"
#include "util/StringUtil.h"
#include "util/TokeniserUtil.h"
#include "exception/SyntaxOpenBraceException.h"
#include "exception/SyntaxNoEndOfStatementException.h"
#include "exception/SyntaxUnknownCommandException.h"
#include "exception/SyntaxEmptyLineException.h"
#include "exception/SyntaxInvalidTerm.h"
#include "exception/SyntaxDuplicateProc.h"

int Parser::parse (NAME t_filename) throw() {
  m_readStream = std::ifstream (t_filename);
  if (!m_readStream.is_open()) {
    return -1;
  }
  m_nextToken = getCurrentLineToken();
  while (!m_readStream.eof()) { // iterates through the program lines
    parseForProcedure(); 
    isMatchToken(EMPTY_LINE); // handles for empty lines
    if (m_nextToken == TokeniserUtil::CLOSE_BRACKET) { // checks if there is an open brace
      throw SyntaxOpenBraceException(m_curLineNum);
    }
  }
  return 0;
}

void Parser::parseForProcedure() {
  if (isMatchToken("procedure")) {
    PROC_NAME procName = getMatchToken(TOKEN_TYPE::PROC_NAME_TYPE); // ge the procedure name
    if (!isMatchToken("{")) {
      throw SyntaxOpenBraceException(m_curLineNum);
    }
    m_curProcIdx = m_pkbWriteOnly->insertProcedure(procName); // get procedure index
    if (m_curProcIdx == INVALID_INDEX) { // handles for procedure duplicate
      throw SyntaxDuplicateProc(m_curLineNum);
    }
    LIST_OF_STMT_NUMS stmtLst;
    LIST_OF_PROG_LINES progLine;
    parseStmtLst(stmtLst, progLine); // parses the statement list
  }
}

void Parser::parseStmtLst(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, MUTABLE_LIST_OF_PROG_LINES t_progLine) {
  // Parse the rest of the code in the
  parseStmt(t_stmtInStmtLst, t_progLine);
  if (isMatchToken("}")) { // checks for the end of a statement list
    if (!m_nestedStmtLineNum.empty()) { // checks if there is any nested statement line
      m_nestedStmtLineNum.pop_back();
    }
    return;
  }
  parseStmtLst(t_stmtInStmtLst, t_progLine); // parses into statement list
}

void Parser::parseStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, MUTABLE_LIST_OF_PROG_LINES t_progLine) {
  if (isMatchToken(EMPTY_LINE)) { // check if there is an empty line
    return;
  }
  m_curLineNum += 1; // increment the current statement line tht we are in
  if (!t_stmtInStmtLst.empty()) { // handles for the normal case of inserting next relation
    if (m_ifLookUp.find(t_stmtInStmtLst.back()) == m_ifLookUp.end()) {
      m_pkbWriteOnly->insertNextRelation(t_stmtInStmtLst.back(), m_curLineNum); // inserts next relations
    }
  }
  if (!t_progLine.empty()) { // handles for adding prog lines that transcends nesting levels
    for (auto& itr : t_progLine) {
      if (itr == t_stmtInStmtLst.back()) {
        continue;
      }
      m_pkbWriteOnly->insertNextRelation(itr, m_curLineNum);
    }
    t_progLine.clear();
  }
  m_pkbWriteOnly->insertFollowsRelation(t_stmtInStmtLst, m_curLineNum);
  m_pkbWriteOnly->insertParentRelation(m_nestedStmtLineNum, m_curLineNum);
  t_stmtInStmtLst.push_back(m_curLineNum);
  if (t_stmtInStmtLst.size() == 1) { // insertion of stmt lst
    m_pkbWriteOnly->insertStmtList(m_curLineNum);
  }
  if (isNonContainerStmt(m_nextToken)) { // parse into other statement type
    parseNonContainerStmt(t_stmtInStmtLst);
  } else {
    parseContainerStmt(t_stmtInStmtLst, t_progLine);
  }
}

void Parser::parseNonContainerStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst) {
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
  VAR_NAME varName = getMatchToken(TOKEN_TYPE::VAR_NAME_TYPE); // Get the variable name
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
  if (!isMatchToken("=")) { // make sure that the assignmnet statement is well formed
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  } 
  LIST_OF_TOKENS tokenisedExpr = parseExpr();
  m_pkbWriteOnly->insertAssignStmt(m_curLineNum, varName, tokenisedExpr, m_curProcIdx);
}

void Parser::parseCallStmt() {
  PROC_NAME procName = getMatchToken(PROC_NAME_TYPE);
  m_pkbWriteOnly->insertCallStmt(m_curProcIdx, procName, m_curLineNum);
}

LIST_OF_TOKENS Parser::parseExpr() {
  LIST_OF_TOKENS output;
  STRING_TOKEN term = getMatchToken(TOKEN_TYPE::VAR_NAME_TYPE); // get the term
  if (!isConstant(term) && !isValidName(term) && !TokeniserUtil::isBracket(term)) {
    throw SyntaxInvalidTerm(m_curLineNum);
  }
  if (term == TokeniserUtil::CLOSE_BRACKET) { // handles for nesting parenthesis
    return output;
  }
  output.push_back(term);
  if (term == TokeniserUtil::OPEN_BRACKET) { // if there is an opening bracket, we will recurse in
    parseBrackets(output);
  } else {
    handleInsertionOfTermByPkb(term); // inserts the term into the PKB
  }
  while (TokeniserUtil::isOperator(m_nextToken) 
      && m_nextToken != TokeniserUtil::CLOSE_BRACKET) { // parse for the rest of term as long as it is not any of the key operators
    parseEachTerm(output);
  }
  return output;
}

void Parser::parseEachTerm(MUTABLE_LIST_OF_TOKENS t_tokens) {
  STRING_TOKEN opr = getMatchToken(TOKEN_TYPE::EXPR_TYPE); // obtain the operator
  if (!TokeniserUtil::isOperator(opr)) { // check for malformed assignment expression, a++c
    throw SyntaxInvalidTerm(m_curLineNum);
  }
  t_tokens.push_back(opr); // append the  operator
  if (opr == TokeniserUtil::OPEN_BRACKET) { // if there is an open bracket, we recurse into it
    parseBrackets(t_tokens);
    return;
  }
  STRING_TOKEN term = getMatchToken(TOKEN_TYPE::VAR_NAME_TYPE); // get the term
  if (!isConstant(term) && !isValidName(term) && !TokeniserUtil::isBracket(term)) { // check for invalid term
    throw SyntaxInvalidTerm(m_curLineNum);
  }
  t_tokens.push_back(term); // append the term
  if (term == TokeniserUtil::OPEN_BRACKET) { // check for open bracket to recurse into
    parseBrackets(t_tokens);
    if (m_nextToken == TokeniserUtil::OPEN_BRACKET) { // check for malformed brackets
      throw SyntaxUnknownCommandException("Cannot put \")(\" ", m_curLineNum);
    }
    return;
  } 
  handleInsertionOfTermByPkb(term);
}

void Parser::parseBrackets(MUTABLE_LIST_OF_TOKENS t_tokens) {
  if (m_nextToken == TokeniserUtil::OPEN_BRACKET) { // check if the next token is an open bracket
    STRING_TOKEN term = getMatchToken(TOKEN_TYPE::VAR_NAME_TYPE);
    t_tokens.push_back(term); // append the term
    parseBrackets(t_tokens);  // recurse into the bracket level
    while (TokeniserUtil::isOperator(m_nextToken) 
        && m_nextToken != TokeniserUtil::CLOSE_BRACKET) { // parse each term until we find a key operator
      parseEachTerm(t_tokens);
    }
  } else {
    LIST_OF_TOKENS subExprTokens = parseExpr(); // parse the expression within the brackets
    t_tokens.insert(t_tokens.end(), subExprTokens.begin(), subExprTokens.end()); // update our total token list
  }
  if (isMatchToken(TokeniserUtil::CLOSE_BRACKET)) {
    t_tokens.push_back(TokeniserUtil::CLOSE_BRACKET); // append into the tokeniser
  } 
  else {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
}

void Parser::parseContainerStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, MUTABLE_LIST_OF_PROG_LINES t_progLine) {
  m_nestedStmtLineNum.push_back(m_curLineNum);
  if (isMatchToken("while")) {
    parseWhileStmt(t_stmtInStmtLst, t_progLine);
  } else if (isMatchToken("if")) {
    parseIfElseStmt(t_stmtInStmtLst, t_progLine);
  } else {
    throw SyntaxUnknownCommandException(m_nextToken, m_curLineNum);
  }
}

void Parser::parseWhileStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, MUTABLE_LIST_OF_PROG_LINES t_progLine) {
  STRING_TOKEN varName = getMatchToken(TOKEN_TYPE::VAR_NAME_TYPE);
  if (!isMatchToken("{")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  STMT_NUM curLine = m_curLineNum; // save the current state of the line number of the while statement
  m_pkbWriteOnly->insertWhileStmt(m_curProcIdx, varName, m_nestedStmtLineNum, m_curLineNum);
  LIST_OF_STMT_NUMS whileStmtLst;
  parseStmtLst(whileStmtLst, t_progLine); // parse the statement list of the while statement
  m_pkbWriteOnly->insertNextRelation(curLine, whileStmtLst.front());
  if (m_ifLookUp.find(whileStmtLst.back()) == m_ifLookUp.end()) { // checks if we have any other container stmt after the while
    m_pkbWriteOnly->insertNextRelation(whileStmtLst.back(), curLine);
  }
  for (auto& itr : t_progLine) { // add the prog lines that we gave found so far the ends in this line
    m_pkbWriteOnly->insertNextRelation(itr, curLine);
  }
  t_progLine.clear();
  t_progLine.push_back(curLine);
}

void Parser::parseIfElseStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, MUTABLE_LIST_OF_PROG_LINES t_progLine) {
  STMT_NUM ifStmtNum = m_curLineNum;
  m_ifLookUp.insert(ifStmtNum); // inserts if statement we have encountered

  LIST_OF_STMT_NUMS tempProgLineForIf;
  parseIfStmt(t_stmtInStmtLst, ifStmtNum, tempProgLineForIf); // parses the if statement list

  m_nestedStmtLineNum.push_back(ifStmtNum); // update the currently nesting line

  LIST_OF_STMT_NUMS tempProgLineForElse;
  parseElseStmt(t_stmtInStmtLst, ifStmtNum, tempProgLineForElse); // parses the else statement list

  // inserts the prog line that we have found so far 
  t_progLine.insert(t_progLine.end(), tempProgLineForIf.begin(), tempProgLineForIf.end()); 
  t_progLine.insert(t_progLine.end(), tempProgLineForElse.begin(), tempProgLineForElse.end());
}

void Parser::parseIfStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, STMT_NUM t_ifStmtNum, MUTABLE_LIST_OF_PROG_LINES t_progLine) {
  STRING_TOKEN varName = getMatchToken(TOKEN_TYPE::VAR_NAME_TYPE); // get the uses variable name
  if (!isMatchToken("then")) { // check that the key word is there
    throw SyntaxUnknownCommandException("If statements require 'then' keyword", m_curLineNum);
  }
  if (!isMatchToken("{")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  m_pkbWriteOnly->insertIfStmt(m_curProcIdx, varName, m_nestedStmtLineNum, m_curLineNum); // inserts the if statement into the pkb
  LIST_OF_STMT_NUMS ifStmtLst;
  parseStmtLst(ifStmtLst, t_progLine); // parse into statement list
  m_pkbWriteOnly->insertNextRelation(t_ifStmtNum, ifStmtLst.front()); // insert the relation into the next relation for the if stmt to its first line
  if (m_ifLookUp.find(ifStmtLst.back()) == m_ifLookUp.end()) {
    if (!t_progLine.empty() && ifStmtLst.back() == t_progLine.back()) {
      return;
    }
    t_progLine.push_back(ifStmtLst.back()); // store as the prog line we will look into
  }
}

void Parser::parseElseStmt(MUTABLE_LIST_OF_STMT_NUMS t_stmtInStmtLst, STMT_NUM t_ifStmtNum, MUTABLE_LIST_OF_PROG_LINES t_progLine) {
  if (!isMatchToken("else")) { // check for the keyword for else statement
    throw SyntaxUnknownCommandException("If statements require 'else' keyword", m_curLineNum);
  }
  if (!isMatchToken("{")) {
    throw SyntaxOpenBraceException(m_curLineNum);
  }
  LIST_OF_STMT_NUMS elseStmtLst;
  parseStmtLst(elseStmtLst, t_progLine); // parse into the else statement 
  m_pkbWriteOnly->insertNextRelation(t_ifStmtNum, elseStmtLst.front()); // insert the relation into the next relation for the else stmt to its first line
  if (m_ifLookUp.find(elseStmtLst.back()) == m_ifLookUp.end()) { 
    t_progLine.push_back(elseStmtLst.back()); // store as the prog line we will look into 
  }
}

BOOLEAN Parser::isMatchToken(STRING_TOKEN t_token) {
  if (m_nextToken == t_token) { // if the token match
    m_nextToken = getCurrentLineToken(); // we find the next token
    return true;
  }
  return false;
}

BOOLEAN Parser::isMatchToken(TOKEN_TYPE t_type) {
  switch (t_type) {
    case TOKEN_TYPE::PROC_NAME_TYPE:
    case TOKEN_TYPE::VAR_NAME_TYPE:
      if (!TokeniserUtil::isKeyDelimiter(m_nextToken)) { // if matches PROC_NAME or VAR_NAME then return true
        m_nextToken = getCurrentLineToken();
        return true;
      }
      break;
    case TOKEN_TYPE::EXPR_TYPE:
      if (TokeniserUtil::isOperator(m_nextToken)) { // if matches EXPR_TYPE return true
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

STRING_TOKEN Parser::getMatchToken(TOKEN_TYPE t_token) {
  STRING_TOKEN output = m_nextToken; // Gets the nextToken
  switch (t_token) {
    case TOKEN_TYPE::PROC_NAME_TYPE:
    case TOKEN_TYPE::VAR_NAME_TYPE:
    case TOKEN_TYPE::CONSTANT_TYPE:
    case TOKEN_TYPE::EXPR_TYPE :
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
  if (getline(m_readStream, extractedLine)) { // obtain a line from the program
    try {
      m_curTokens = TokeniserUtil::tokeniseLine(extractedLine); // Tokenise the line
      return getToken(); // obtain the first token
    } catch (SyntaxEmptyLineException sele) {
      return EMPTY_LINE;
    }
  }
  return EMPTY_LINE;  
}

STRING_TOKEN Parser::getToken() {
  if (m_curTokens.empty()) {
    throw SyntaxEmptyLineException();
  }
  STRING_TOKEN token = m_curTokens.front();
  m_curTokens.erase(m_curTokens.begin());
  return token;
}

BOOLEAN Parser::isValidName(STRING_TOKEN t_token) {
  if (t_token.size() == 0) {
    return false;
  }
  if (isdigit(t_token[0]) && !isalpha(t_token[0])) { 
    return false;
  }
  for (auto &cToken : t_token) { // iterates through each of the tokens
    if (!isalpha(cToken) && !isdigit(cToken)) {
      return false;
    }
  }
  return true;
}

BOOLEAN Parser::isConstant(STRING_TOKEN t_token) {
  for (auto& cToken : t_token) {
    if (!isdigit(cToken)) {
      return false;
    }
  }
  return true;
}

BOOLEAN Parser::isNonContainerStmt(STRING_TOKEN t_token) {
  return t_token != "while" && t_token != "if";
}

void Parser::handleInsertionOfTermByPkb(STRING_TOKEN t_term) {
  if (isConstant(t_term)) {
    m_pkbWriteOnly->insertConstant(t_term);
  } else if (isValidName(t_term)) {
    m_pkbWriteOnly->insertUses(m_curProcIdx, t_term, m_nestedStmtLineNum, m_curLineNum);
  }
}