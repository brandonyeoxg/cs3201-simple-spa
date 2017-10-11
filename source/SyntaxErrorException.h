#pragma once
#include <exception>
#include <string.h>
class SyntaxErrorException : public std::exception {
public:
  SyntaxErrorException(const std::string &t_exceptionMsg) : m_exceptionMsg(t_exceptionMsg) {}
  virtual const char* what() const throw() {
    return m_exceptionMsg.c_str();
  }
private:
  const std::string m_exceptionMsg;
};