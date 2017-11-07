#pragma once
#include <string>

#include "SyntaxErrorException.h"

class InvalidArgumentException : public SyntaxErrorException {
public:
  InvalidArgumentException(const std::string& ia) :
    SyntaxErrorException(ia) {}
};