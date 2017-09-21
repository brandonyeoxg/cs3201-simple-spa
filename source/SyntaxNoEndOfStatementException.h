#pragma once
#include "SyntaxErrorException.h"

class SyntaxNoEndOfStatmentException : public SyntaxErrorException {
public:
  SyntaxNoEndOfStatmentException(const int &t_curLine) :
    SyntaxErrorException("Semi colon \";\" not present at the end of assign statement at line: " + std::to_string(t_curLine)) {}
};