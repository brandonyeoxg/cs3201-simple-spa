#pragma once

#include "GlobalTypeDef.h"

class Clause {
public:
  virtual queryType::clauseType getClauseType();
};