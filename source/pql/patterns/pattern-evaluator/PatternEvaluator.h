#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../../../GlobalTypeDef.h"
#include "../../../pkb/PkbReadOnly.h"
#include "../../query-types/Grammar.h"
#include "../../../util/Formatter.h"

class PatternEvaluator {
public:
  virtual SET_OF_RESULTS_INDICES getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS_INDICES getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS_INDICES getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS_INDICES getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS_INDICES getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS_INDICES getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS_INDICES getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS_INDICES getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS_INDICES getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
};