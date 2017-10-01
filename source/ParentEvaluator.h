#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"
#include "Evaluator.h"
#include "Relation.h"
#include "PkbReadOnly.h"

class ParentEvaluator : public Evaluator {
public:
  ParentEvaluator() {};

  ~ParentEvaluator() {};

  bool isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  bool hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2);

private:
  std::unordered_map<std::string, std::vector<std::string>> m_result;
};