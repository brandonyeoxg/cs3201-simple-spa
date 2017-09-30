#pragma once

#include "Relationship.h"

Evaluator* Relationship::createEvaluator(Relation::RType t_type) {
  switch (t_type) {
  case Relation::RType::FOLLOWS:
    return new FollowsEvaluator();
    break;
  case Relation::RType::FOLLOWS_:
    return new FollowsStarEvaluator();
    break;
  case Relation::RType::PARENT:
    return new ParentEvaluator();
    break;
  case Relation::RType::PARENT_:
    return new ParentStarEvaluator();
    break;
  case Relation::RType::USES:
    return new UsesEvaluator();
    break;
  case Relation::RType::MODIFIES:
    return new ModifiesEvaluator();
    break;
  case Relation::RType::CALLS:
    return new CallsEvaluator();
    break;
  case Relation::RType::CALLS_:
    return new CallsStarEvaluator();
    break;
  case Relation::RType::NEXT:
    return new NextEvaluator();
    break;
  case Relation::RType::NEXT_:
    return new NextStarEvaluator();
    break;
  case Relation::RType::AFFECTS:
    return new AffectsEvaluator();
    break;
  case Relation::RType::AFFECTS_:
    return new AffectsStarEvaluator();
    break;
  default:
    break;
  }
}
