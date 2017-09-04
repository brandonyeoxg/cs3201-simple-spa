#pragma once
using namespace std;

/*
  Represents the parser that parser a file.
*/
class Parser {
public:
  Parser(std::string &t_filename) 
    : m_filename(t_filename) {};
  ~Parser() {};
  int parse(); //! < returns 0 if no issue, -1 if there is a problem.

private:
  std::string m_filename;
};
