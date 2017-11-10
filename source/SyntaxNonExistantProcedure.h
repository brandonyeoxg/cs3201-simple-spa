#pragma once
#include "SyntaxErrorException.h"
class SyntaxNonExistantProcedure : public SyntaxErrorException {
public:
  SyntaxNonExistantProcedure() : SyntaxErrorException("Detected non existant procedure(s) being called!") {}
};