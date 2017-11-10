#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"
#include "Evaluator.h"
#include "Relation.h"
#include "PkbReadOnly.h"
#include "InvalidArgumentException.h"

class FollowsEvaluator: public Evaluator {
public:
  FollowsEvaluator() {};

  ~FollowsEvaluator() {};

  BOOLEAN isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  BOOLEAN hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);

private:
  SET_OF_RESULTS_INDICES m_result;
};