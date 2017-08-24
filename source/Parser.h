#pragma once
#include <stack>
#include "PKB.h"
using namespace std;
/*
  Represents the parser that parser a file.
*/
class Parser {
public:
  Parser(PKB *t_pkb) 
    : m_pkb(t_pkb), 
      m_curLineNum(0),
      m_curProcNum(0) {};

  ~Parser() {};
  
  int parse(const std::string &t_filename); //! < returns 0 if no issue, -1 if there is a problem.

private:
  unsigned int m_curLineNum;
  PKB *m_pkb;
  PROC m_curProcNum;
  std::stack<char> m_bracesStack;

  int parseForProcedure(ifstream &t_readStream, const std::string &t_line);
  int parseCodeInProcedure(ifstream &t_readStream);
  bool parseForBraces(vector<string> &t_tokens);
  std::vector<std::string> tokeniseLine(const std::string &t_line);
};
