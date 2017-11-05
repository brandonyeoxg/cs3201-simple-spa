#pragma once

#include "GlobalTypeDef.h"

class Clause {
public:
  queryType::clauseType getClauseType();

  /** Checks if given Clause is of type Pattern.
  *   @author jazlyn
  */
  bool isPatternType();

  /** Checks if given Clause is of type Relation.
  *   @author jazlyn
  */
  bool isRelationType();

protected:
  queryType::clauseType m_clauseType;
};