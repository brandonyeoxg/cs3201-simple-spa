#pragma once
#include "SyntaxErrorException.h"

class SyntaxOpenBraceException : public SyntaxErrorException {
public:
  SyntaxOpenBraceException(const int &t_curLine) : 
    SyntaxErrorException("Open Braces error encountered at line: " + std::to_string(t_curLine)) {}
};