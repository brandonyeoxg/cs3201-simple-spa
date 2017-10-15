#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"
#include "PkbReadOnly.h"
#include "Grammar.h"
#include "EvaluatorUtil.h"

class Evaluator {
public:
  virtual BOOLEAN isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) = 0;
  virtual BOOLEAN hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) = 0;
};