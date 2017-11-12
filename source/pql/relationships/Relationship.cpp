#pragma once

#include "Relationship.h"

Evaluator* Relationship::createEvaluator(queryType::RType t_type) {
  switch (t_type) {
  case queryType::RType::FOLLOWS:
    return new FollowsEvaluator();
    break;
  case queryType::RType::FOLLOWS_:
    return new FollowsStarEvaluator();
    break;
  case queryType::RType::PARENT:
    return new ParentEvaluator();
    break;
  case queryType::RType::PARENT_:
    return new ParentStarEvaluator();
    break;
  case queryType::RType::USES:
    return new UsesEvaluator();
    break;
  case queryType::RType::MODIFIES:
    return new ModifiesEvaluator();
    break;
  case queryType::RType::CALLS:
    return new CallsEvaluator();
    break;
  case queryType::RType::CALLS_:
    return new CallsStarEvaluator();
    break;
  case queryType::RType::NEXT:
    return new NextEvaluator();
    break;
  case queryType::RType::NEXT_:
    return new NextStarEvaluator();
    break;
  case queryType::RType::AFFECTS:
    return new AffectsEvaluator();
    break;
  case queryType::RType::AFFECTS_:
    return new AffectsStarEvaluator();
    break;
  default:
    break;
  }
}
