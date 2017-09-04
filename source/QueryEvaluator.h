#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

#include "Grammar.h"
#include "DesignAbstraction.h"
#include "Pattern.h"

#ifndef QUERYEVALUATOR_H
#define QUERYEVALUATOR_H

/**
* A class that does the evaluation of the queries.
*/

class QueryEvaluator {
public:
  QueryEvaluator();
  void evaluateQuery();
  std::vector<std::string> getQueryResult();

  // Todo: Find a better place to put these 3 strings.
  const string SELECT = "Select";
  const string RELATION = "Relation";
  const string PATTERN = "Pattern";

private:
  std::string selectedSynonym; /**< the synonym that the query selects */
  std::vector<std::string> m_queryResult; /**< result of the query */
  std::queue<Grammar> m_selects; /**< queue which stores the grammars to be selected in the query */
  std::queue<DesignAbstraction> m_relations; /**< queue which stores the relation clauses in the query */
  std::queue<Pattern> m_patterns; /**< queue which stores the pattern clauses in the query */
  std::queue<std::vector<std::string>> m_selectResults; /**< queue which stores the results of the select grammars in the query */
  std::queue<std::unordered_map<int, std::vector<int>>> m_relationResults; /**< queue which stores the results of the relation clauses in the query */
  std::queue<std::unordered_map<int, std::vector<int>>> m_patternResults; /**< queue which stores the results of the pattern clauses in the query */

  void getQueryClauses();
  bool getResultFromPkb();
  void storeResultFromPkb(std::unordered_map<int, std::vector<int>> t_result, std::string t_type);
  std::vector<std::string> evaluateFinalResult();
  void storeFinalResult(std::vector<std::string> t_result);
};

#endif QUERYEVALUATOR_H