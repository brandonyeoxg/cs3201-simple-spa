#pragma once

#include "Clause.h"

queryType::clauseType Clause::getClauseType() {
  return queryType::clauseType::SELECT;
}

INTEGER Clause::getWeights() {
  return m_weights;
}

void Clause::setWeights(INTEGER t_weights) {
  m_weights = t_weights;
}

void Clause::addWeights(INTEGER t_weights) {
  m_weights += t_weights;
}

void Clause::multiplyWeights(INTEGER t_weights) {
  m_weights *= t_weights;
}