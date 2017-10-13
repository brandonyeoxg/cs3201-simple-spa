#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"
#include "Grammar.h"

class EvaluatorUtil {
public:
  /**
  * A public function to filter the keys by the GType.
  * It will filter the keys of the results by the GType.
  * @param t_typeOfStmts A map of all the statements in the simple source program with its type of statement.
  * @param t_results A map of results to be filtered.
  * @return a result filtered by the GType in a string vector.
  */
  static std::vector<std::string> filterKeyResults(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::unordered_map<std::string, std::vector<std::string>> t_results, queryType::GType t_selectedType);

  /**
  * A public function to filter the values by the GType.
  * It will filter the values of the results by the GType.
  * @param t_typeOfStmts A map of all the statements in the simple source program with its type of statement.
  * @param t_results A map of results to be filtered.
  * @return a result filtered by the GType in a string vector.
  */
  static std::vector<std::string> filterValueResults(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::unordered_map<std::string, std::vector<std::string>> t_results, queryType::GType t_selectedType);

  static std::vector<std::string> filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar);
  static std::vector<std::string> filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::vector<int> t_stmtIntVector, Grammar t_grammar);
  static std::vector<std::string> filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar, std::vector<std::string> t_stmtVector);
};
