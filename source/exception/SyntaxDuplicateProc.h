#pragma once
#include "SyntaxErrorException.h"

class SyntaxDuplicateProc : public SyntaxErrorException {
public:
  SyntaxDuplicateProc(const int &t_curLine) : 
    SyntaxErrorException("Duplicate procedures detected " + std::to_string(t_curLine)) {}
};