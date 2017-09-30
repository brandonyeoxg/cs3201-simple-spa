#pragma once

#include "FollowsRelationship.h"

Evaluator* FollowsRelationship::createEvaluator() {
  return new FollowsEvaluator(m_pkb);
}

