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
    m_pkb->setProcToAST(m_curProcNum++, new TNode());
    parseForBraces(tokens.at(2));
    return parseCodeInProcedure(t_readStream);
  }
  return -1;
}

int Parser::parseCodeInProcedure(ifstream &t_readStream) {
  // Parse the rest of the code in the
  std::string stmtLine;
  while (getline(t_readStream, stmtLine)) {
    std::vector<std::string> tokens = tokeniseLine(stmtLine);
    m_curLineNum++;
    parseLine(tokens);
  }
  return -1;
}

int Parser::parseLine(std::vector<std::string> t_tokens) {
  std::vector<string>::iterator itr = t_tokens.begin();
  bool varFlag = false;
  for (; itr != t_tokens.end(); ++itr) {
    if (*itr == "{" || *itr == "}") {
      parseForBraces(*itr);
      continue;
    }
    if (!varFlag) {
      varFlag = parseForVariable(*itr);
    }
  }

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
  m_pkb->varTable;
  std::cout << "The Var in this line: " << t_token << "\n";
  return true;
}

std::vector<std::string> Parser::tokeniseLine(const std::string &t_line) {
  std::string lowerStr = t_line;
  StringUtil::toLower(lowerStr);
  lowerStr = StringUtil::reduceString(lowerStr);
  return StringUtil::splitString(lowerStr);
}