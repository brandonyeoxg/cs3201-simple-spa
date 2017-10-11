#pragma once

#include "Pattern.h"
#include "PatternEvaluator.h"
#include "AssignPatternEvaluator.h"
#include "WhilePatternEvaluator.h"
#include "IfPatternEvaluator.h"


class Patterns {
public:
  Patterns(PkbReadOnly *t_pkb)
    : m_pkb(t_pkb) {};

  ~Patterns() {};

  static PatternEvaluator* createEvaluator(queryType::GType t_type);

private:
  PkbReadOnly *m_pkb; /**< A PKB pointer. The PKB instance that was created in the TestWrapper.cpp. */
};