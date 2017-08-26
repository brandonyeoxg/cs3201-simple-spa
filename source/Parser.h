#pragma once
#include <stack>
#include <fstream>
#include <sstream>
#include <iostream>

#include "PKB.h"
using namespace std;

/**
 * Represents a parser. Parses the SIMPLE program and builds an ast.
 *
 * @author Brandon
 * @date 8/26/2017
 *
 */
class Parser {
public:
  Parser(PKB *t_pkb) 
    : m_pkb(t_pkb), 
      m_curLineNum(0),
      m_curProcNum(0),
      m_nextToken(""),
      m_isParsingProcedureContent(false) {};

  ~Parser() {};
  
  int parse(const std::string &t_filename); //! < returns 0 if no issue, -1 if there is a problem.

private:
  unsigned int m_curLineNum;
  PKB *m_pkb;
  PROC m_curProcNum;
  std::string m_nextToken;
  std::stack<std::string> m_bracesStack;
  std::vector<std::string> curTokens;
  ifstream m_readStream;
  bool m_isParsingProcedureContent;

  enum tokenType {
    PROC_NAME,
    VAR_NAME,
    EXPR,
  };

  int parseForProcedure();
  int parseStmtLst();
  int parseStmt();
  bool parseForBraces(const string &t_token);
  bool parseForVariable(const string &t_token);

  bool matchToken(const std::string &t_token);
  bool matchToken(const tokenType &t_token);

  bool isOperator(const std::string &t_token);

  std::string getCurrentLineToken();
  std::string getToken();
  std::vector<std::string> tokeniseLine(const std::string &t_line);
};
