#pragma once

#include "Clause.h"

queryType::clauseType Clause::getClauseType() {
  return m_clauseType;
}

bool Clause::isPatternType() {
  return getClauseType() == queryType::clauseType::PATTERN;
}

bool Clause::isRelationType() {
  return getClauseType() == queryType::clauseType::RELATION;
}

