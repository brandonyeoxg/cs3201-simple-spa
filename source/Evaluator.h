#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"
#include "PkbReadOnly.h"
#include "Grammar.h"

class Evaluator {
public:
  virtual bool isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) = 0;
  virtual bool hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) = 0;
  virtual SET_OF_RESULTS evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) = 0;
  std::vector<std::string> filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar);
  std::vector<std::string> filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::vector<int> t_stmtIntVector, Grammar t_grammar);
  std::vector<std::string> filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar, std::vector<std::string> t_stmtVector);
};