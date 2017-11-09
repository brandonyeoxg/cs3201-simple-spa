#pragma once

#include "GlobalTypeDef.h"

class Clause {
public:
  virtual queryType::clauseType getClauseType();
  virtual INTEGER getWeights();

protected:
  INTEGER weights;
};