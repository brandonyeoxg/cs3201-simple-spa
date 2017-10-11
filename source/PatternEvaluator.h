#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"
#include "PkbReadOnly.h"
#include "Grammar.h"

class PatternEvaluator {
public:
  virtual SET_OF_RESULTS getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) = 0;
  std::vector<std::string> formatListIntToVectorStr(std::list<int> t_listInt);
  std::vector<std::string> formatVectorIntToVectorStr(std::vector<int> t_vectorInt);
  SET_OF_RESULTS formatMapIntStrToMapStrVectorStr(std::unordered_map<int, std::string> t_mapIntStr);
};