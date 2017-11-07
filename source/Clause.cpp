#pragma once

#include "Clause.h"

queryType::clauseType Clause::getClauseType() {
  return queryType::clauseType::SELECT;
}

bool Clause::isPatternType() {
  return getClauseType() == queryType::clauseType::PATTERN;
}

bool Clause::isRelationType() {
  return getClauseType() == queryType::clauseType::RELATION;
}

