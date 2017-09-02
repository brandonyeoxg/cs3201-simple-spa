#pragma once
#include "SyntaxErrorException.h"
#include <string>

class SyntaxUnknownCommandException : public SyntaxErrorException {
public:
  SyntaxUnknownCommandException(const std::string t_cmdInput, const int &t_curLine) : 
    SyntaxErrorException("Unknown command: " + t_cmdInput + " used at line: " + std::to_string(t_curLine)) {}
};