#pragma once

#include "Clause.h"

queryType::clauseType Clause::getClauseType() {
  return queryType::clauseType::SELECT;
}

INTEGER Clause::getWeights() {
  return m_weights;
}