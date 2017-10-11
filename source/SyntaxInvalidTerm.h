#pragma once
#include "SyntaxErrorException.h"

class SyntaxInvalidTerm : public SyntaxErrorException {
public:
  SyntaxInvalidTerm(const int &t_curLine) :
    SyntaxErrorException("Term used is neither a valid constant nor name nor an operator, found at line: " + std::to_string(t_curLine)) {}
};