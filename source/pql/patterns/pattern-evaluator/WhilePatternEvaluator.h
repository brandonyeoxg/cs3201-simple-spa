#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../../../GlobalTypeDef.h"
#include "PatternEvaluator.h"
#include "../../../pkb/PkbReadOnly.h"

class WhilePatternEvaluator: public PatternEvaluator {
public:
  WhilePatternEvaluator() {};

  ~WhilePatternEvaluator() {};

  SET_OF_RESULTS_INDICES getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2);
  SET_OF_RESULTS_INDICES getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2);

private:
  SET_OF_RESULTS_INDICES m_result;
};