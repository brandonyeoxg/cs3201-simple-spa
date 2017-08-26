#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

#include "Parser.h"
#include "StringUtil.h"
#include "TNode.h"

int Parser::parse (const std::string &t_filename) {
  // Open file
  ifstream readStream(t_filename);
  if (!readStream.is_open()) {
    return -1;
  }
  std::string stmtLine;
  while (getline(readStream, stmtLine)) {
    // Search for procedure
    parseForProcedure(readStream, stmtLine);
  }

  return 0;
}

int Parser::parseForProcedure(ifstream &t_readStream, const std::string &t_line) {
  // Construct the AST based on the parsed line
  // Remove unecessary spaces, tabs	
  std::vector<std::string> tokens = tokeniseLine(t_line);
  if (tokens.at(0) == "procedure") {
    parseForBraces(tokens);
    return parseCodeInProcedure(t_readStream);
  }
  return -1;
}

int Parser::parseCodeInProcedure(ifstream &t_readStream) {
  // Parse the rest of the code in the
  std::string stmtLine;
  while (getline(t_readStream, stmtLine)) {
    std::vector<std::string> tokens = tokeniseLine(stmtLine);
    parseForBraces(tokens);
    m_pkb->varTable;
  }
  return -1;
}

bool Parser::parseForBraces(vector<string> &t_tokens) {
  // Iterate through the tokens to identify braces
  std::vector<string>::iterator itr = t_tokens.begin();
  for (; itr != t_tokens.end(); itr++) {
    if (*itr == "{") {
      m_bracesStack.push('{');
    }
    else if (*itr == "}") {
      if (m_bracesStack.empty()) {
        return false;
      }
      m_bracesStack.pop();
    }
  }
  return true;
}

std::vector<std::string> Parser::tokeniseLine(const std::string &t_line) {
  std::string lowerStr = t_line;
  StringUtil::toLower(lowerStr);
  lowerStr = StringUtil::reduceString(lowerStr);
  return StringUtil::splitString(lowerStr);
}