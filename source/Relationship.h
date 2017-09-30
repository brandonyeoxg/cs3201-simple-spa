#pragma once

#include "Relation.h"
#include "Evaluator.h"
#include "FollowsEvaluator.h"
#include "FollowsStarEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentStarEvaluator.h"
#include "UsesEvaluator.h"
#include "ModifiesEvaluator.h"
#include "CallsEvaluator.h"
#include "CallsStarEvaluator.h"
#include "NextEvaluator.h"
#include "NextStarEvaluator.h"
#include "AffectsEvaluator.h"
#include "AffectsStarEvaluator.h"

class Relationship {
public:
  Relationship(PkbReadOnly *t_pkb)
    : m_pkb(t_pkb) {};

  ~Relationship() {};

  static Evaluator* createEvaluator(Relation::RType t_type);

private:
  PkbReadOnly *m_pkb; /**< A PKB pointer. The PKB instance that was created in the TestWrapper.cpp. */
};