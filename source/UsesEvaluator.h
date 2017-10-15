#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"
#include "Evaluator.h"
#include "Relation.h"
#include "PkbReadOnly.h"

class UsesEvaluator: public Evaluator {
public:
  UsesEvaluator() {};

  ~UsesEvaluator() {};

  BOOLEAN isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  BOOLEAN hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);

private:
  SET_OF_RESULTS m_result;
};