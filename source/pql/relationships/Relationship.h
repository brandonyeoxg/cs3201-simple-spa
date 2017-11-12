#pragma once

#include "../query-types/Relation.h"
#include "evaluators/Evaluator.h"
#include "evaluators/FollowsEvaluator.h"
#include "evaluators/FollowsStarEvaluator.h"
#include "evaluators/ParentEvaluator.h"
#include "evaluators/ParentStarEvaluator.h"
#include "evaluators/UsesEvaluator.h"
#include "evaluators/ModifiesEvaluator.h"
#include "evaluators/CallsEvaluator.h"
#include "evaluators/CallsStarEvaluator.h"
#include "evaluators/NextEvaluator.h"
#include "evaluators/NextStarEvaluator.h"
#include "evaluators/AffectsEvaluator.h"
#include "evaluators/AffectsStarEvaluator.h"

class Relationship {
public:
  Relationship(PkbReadOnly *t_pkb)
    : m_pkb(t_pkb) {};

  ~Relationship() {};

  static Evaluator* createEvaluator(queryType::RType t_type);

private:
  PkbReadOnly *m_pkb; /**< A PKB pointer. The PKB instance that was created in the TestWrapper.cpp. */
};