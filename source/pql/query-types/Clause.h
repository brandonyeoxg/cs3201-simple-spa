#pragma once

#include "../../GlobalTypeDef.h"
#include "../../QueryUtil.h"
#include "../../StringUtil.h"

class Clause {
public:
  virtual queryType::clauseType getClauseType();
  INTEGER getWeights();
  void setWeights(INTEGER t_weights);
  void addWeights(INTEGER t_weights);
  void multiplyWeights(INTEGER t_weights);

  /** Checks if given Clause is of type Pattern.
  *   @author jazlyn
  */
  bool isPatternType();

  /** Checks if given Clause is of type Relation.
  *   @author jazlyn
  */
  bool isRelationType();

protected:
  INTEGER m_weights;
};