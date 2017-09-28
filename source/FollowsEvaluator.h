#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"
#include "Evaluator.h"
#include "Relation.h"
#include "PkbReadOnly.h"

class FollowsEvaluator: public Evaluator {
public:
  FollowsEvaluator(PkbReadOnly *t_pkb) 
    : m_pkb(t_pkb) {};

  ~FollowsEvaluator() {};

  SET_OF_RESULTS evaluate(Relation t_relation);

private:
  PkbReadOnly *m_pkb; /**< A PKB pointer. The PKB instance that was created in the TestWrapper.cpp. */
  std::unordered_map<std::string, std::vector<std::string>> results;
};