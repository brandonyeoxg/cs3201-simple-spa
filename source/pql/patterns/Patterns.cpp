#pragma once

#include "Patterns.h"

PatternEvaluator* Patterns::createEvaluator(queryType::GType t_type) {
  switch (t_type) {
  case queryType::GType::ASGN:
    return new AssignPatternEvaluator();
    break;
  case queryType::GType::WHILE:
    return new WhilePatternEvaluator();
    break;
  case queryType::GType::IF:
    return new IfPatternEvaluator();
    break;
  default:
    break;
  }
}
