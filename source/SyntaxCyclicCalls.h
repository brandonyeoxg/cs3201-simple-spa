#pragma once
#include "SyntaxErrorException.h"

class SyntaxCyclicCalls : public SyntaxErrorException {
public:
  SyntaxCyclicCalls() :
    SyntaxErrorException("There are cyclic calls in the program!") {}
};