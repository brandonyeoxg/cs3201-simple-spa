#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

#include "GlobalTypeDef.h"
#include "Grammar.h"
#include "Relation.h"
#include "Pattern.h"
#include "PkbReadOnly.h"
#include "QueryPreProcessor.h"
#include "Relationship.h"
#include "Patterns.h"
#include "Evaluator.h"
#include "IntermediateTable.h"
#include "Formatter.h"
#include "QueryUtil.h"

#ifndef QUERYEVALUATOR_H
#define QUERYEVALUATOR_H

/**
* A class that does the evaluation of the queries.
* Takes in the query in the form of 3 queues which stores all the synonyms being selected,
* all the such that clauses and all the pattern clauses.
* @author Verbena
* @date 03/10/2017
*/
class QueryEvaluator {
public:

  /**
  * A constructor.
  * Initialise essential data members in the class.
  * @param *t_pkb A reference to the address of the instance of PKB created in the TestWrapper.
  * @param t_selects A queue to store all the synonyms being selected in the query.
  * @param t_relations A queue to store all the such that clauses in the query.
  * @param t_patterns A queue to store all the pattern clauses in the query.
  * @param t_synonymsList An unordered_map to store all the different synonyms and the number of times it is used in the query.
  */
  QueryEvaluator(PkbReadOnly *t_pkb, std::queue<Grammar> t_selects, std::queue<Relation> t_relations, std::queue<Pattern> t_patterns, std::unordered_map<std::string, int> t_synonymsList)
    : m_pkb(t_pkb),
      m_selectedSynonym(""),
      m_selects(t_selects),
      m_relations(t_relations),
      m_patterns(t_patterns),
      m_synonymsUsedInQuery(t_synonymsList),
      m_isSelectOnly(true) {
    m_table = new IntermediateTable();
  };

  /**
  * A destructor.
  */
  ~QueryEvaluator() {
    delete m_table;
  };

  /**
  * A public function to evaluate queries.
  * Get the results of all the clauses in the query from PKB, then compare and get the result of the query.
  * @return the result of the query in a vector of strings.
  */
  std::vector<std::string> evaluateQuery();

private:
  PkbReadOnly *m_pkb; /**< A PKB pointer. The PKB instance that was created in the TestWrapper.cpp. */
  std::string m_selectedSynonym; /**< A string. The synonym that the query selects. */
  queryType::GType m_selectedType; /**< A string. The type of the synonym that the query selects. */
  std::unordered_map<std::string, int> m_synonymsUsedInQuery; /**< A map of synonyms used and the number of times it has been used in the query. */
  std::queue<Grammar> m_selects; /**< A grammar queue. It stores the synonyms to be selected in the query. */
  std::queue<Relation> m_relations; /**< A relation queue. It stores the such that clauses in the query. */
  std::queue<Pattern> m_patterns; /**< A pattern queue. It stores the pattern clauses in the query. */
  std::queue<std::vector<std::string>> m_selectResults; /**< A list queue. It stores the results of the selected synonyms in the query. */
  std::queue<std::unordered_map<std::string, std::vector<std::string>>> m_relationResults; /**< An unordered map queue. It stores the results of the such that clauses in the query. */
  std::queue<std::unordered_map<std::string, std::vector<std::string>>> m_patternResults; /**< An unordered map queue. It stores the results of the pattern clauses in the query. */
  IntermediateTable *m_table; /**< A intermediate table pointer. The intermediate table instance to store and merge the results of the clauses in the query. */
  bool m_isSelectOnly; /**< A boolean. It indicates whether the query is only Select without any other clauses*/

  /**
  * A private function to get the results of every clause in the query from the PKB.
  * Loop through the queues and call the API from PKB to get the results of each and every clause in the queues.
  * @return true if all of the clauses have non-empty results.
  * otherwise false if there are one clause which returns an empty result.
  */
  bool getResultFromPkb();

  /**
  * A private function to get the results of the select clause in the query from the PKB.
  * Call the API from PKB to get the results the select clause in the argument.
  * @param t_select A grammar object that stores the synonym to be selected.
  * @return true if the select clause has non-empty results.
  * otherwise false if the select clause returns an empty result.
  */
  bool getSelectResultFromPkb(Grammar t_select);

  /**
  * A private function to get the results of the relation clause in the query from the PKB.
  * Call the API from PKB to get the results of each and every relation clause in the queues.
  * @param t_relation A relation object that stores the relation clause.
  * @return true if the relation clause has non-empty results.
  * otherwise false if the relation clause returns an empty result.
  */
  bool getRelationResultFromPkb(Relation t_relation);

  /**
  * A private function to get the results of the pattern clause in the query from the PKB.
  * Call the API from PKB to get the results of each and every pattern clause in the queues.
  * @param t_pattern A pattern object that stores the pattern clause.
  * @return true if the pattern clause has non-empty results.
  * otherwise false if the pattern clause returns an empty result.
  */
  bool getPatternResultFromPkb(Pattern t_pattern);

  /**
  * A private function to store the select results.
  * It takes in the select result to be store into the queue.
  * @param t_result A string vector which holds the result returned from PKB.
  */
  void storeSelectResultFromPkb(std::vector<std::string> t_result);

  /**
  * A private function to store the relation result if it is needed.
  * It takes in the relation and the relation result to check whether there is a need to store it.
  * @param t_relation A relation object which holds the relation clause that was evaluated to the parameter t_result.
  * @param t_result An unordered map which holds the result of the relation clause returned from PKB.
  */
  bool storeRelationResultFromPkb(Relation t_relation, std::unordered_map<std::string, std::vector<std::string>> t_result);
  
  /**
  * A private function to store the pattern result if it is needed.
  * It takes in the pattern and the pattern result to check whether there is a need to store it.
  * @param t_pattern A pattern object which holds the pattern clause that was evaluated to the parameter t_result.
  * @param t_result An unordered map which holds the result of the pattern clause returned from PKB.
  */
  bool storePatternResultFromPkb(Pattern t_pattern, std::unordered_map<std::string, std::vector<std::string>> t_result);

  /**
  * A private function to evaluate the final result of the query.
  * By comparing all the results with the common synonyms, get the final result of the query.
  * @return A vector of strings as the query result.
  */
  std::vector<std::string> evaluateFinalResult();
};

#endif QUERYEVALUATOR_H