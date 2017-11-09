#pragma once

#include "GlobalTypeDef.h"
#include "QueryUtil.h"

class Clause {
public:
  virtual queryType::clauseType getClauseType();
  virtual INTEGER getWeights();

protected:
  INTEGER m_weights;
};