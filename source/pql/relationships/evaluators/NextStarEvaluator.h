#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../../../GlobalTypeDef.h"
#include "Evaluator.h"
#include "../../query-types/Relation.h"
#include "../../../pkb/PkbReadOnly.h"

class NextStarEvaluator : public Evaluator {
public:
  NextStarEvaluator() {};

  ~NextStarEvaluator() {};

  BOOLEAN isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  BOOLEAN hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);

private:
  SET_OF_RESULTS_INDICES m_result;
};