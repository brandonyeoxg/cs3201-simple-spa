#pragma once

#include "Relationships.h"
#include "FollowsEvaluator.h"

class FollowsRelationship: public Relationships {
public:
  static Evaluator* createEvaluator() {
    return new FollowsEvaluator();
  }
};