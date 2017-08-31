#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

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

private:
  std::vector<std::string> m_queryResult; /**< result of the query */
  std::queue<Grammar> m_selects; /**< queue which stores the grammars to be selected in the query */
  std::queue<DesignAbstraction> m_relations; /**< queue which stores the relation clauses in the query */
  std::queue<Pattern> m_patterns; /**< queue which stores the pattern clauses in the query */
  std::queue<std::vector<std::string>> m_selectResults; /**< queue which stores the results of the select grammars in the query */
  std::queue<std::vector<std::string>> m_relationResults; /**< queue which stores the results of the relation clauses in the query */
  std::queue<std::vector<std::string>> m_patternResults; /**< queue which stores the results of the pattern clauses in the query */

  void getQueryClauses();
  void storeQueryClauses();
  std::vector<std::string> getResultFromPkb(std::vector<std::string> t_query);
  void storeResultFromPkb(std::vector<std::string> t_result);
  std::vector<std::string> evaluateFinalResult(std::vector<std::string> t_listOfResults);
  void storeFinalResult(std::vector<std::string> t_result);
};

#endif QUERYEVALUATOR_H