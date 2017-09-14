#pragma once
#include "SyntaxErrorException.h"

class SyntaxNoEndOfStatmentException : public SyntaxErrorException {
public:
  SyntaxNoEndOfStatmentException(const int &t_curLine) : 
    SyntaxErrorException("; not present at the end of assign statement at line: " + t_curLine) {}
};