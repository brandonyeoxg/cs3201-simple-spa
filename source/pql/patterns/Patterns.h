#pragma once

#include "../../Pattern.h"
#include "pattern-evaluator/PatternEvaluator.h"
#include "pattern-evaluator/AssignPatternEvaluator.h"
#include "pattern-evaluator/WhilePatternEvaluator.h"
#include "pattern-evaluator/IfPatternEvaluator.h"


class Patterns {
public:
  Patterns(PkbReadOnly *t_pkb)
    : m_pkb(t_pkb) {};

  ~Patterns() {};

  static PatternEvaluator* createEvaluator(queryType::GType t_type);

private:
  PkbReadOnly *m_pkb; /**< A PKB pointer. The PKB instance that was created in the TestWrapper.cpp. */
};