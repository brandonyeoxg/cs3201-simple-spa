#pragma once
#include "SyntaxErrorException.h"

class SyntaxEmptyLineException : public SyntaxErrorException {
public:
  SyntaxEmptyLineException() : SyntaxErrorException("") {}
};