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

#include "Grammar.h"
#include "Relation.h"
#include "Pattern.h"
#include "PKB.h"
#include "QueryPreProcessor.h"

#ifndef QUERYEVALUATOR_H
#define QUERYEVALUATOR_H

/**
* A class that does the evaluation of the queries.
* Takes in the query in the form of 3 queues which stores all the synonyms being selected,
* all the such that clauses and all the pattern clauses.
* @author Verbena
* @date 05/09/2017
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
  QueryEvaluator(PKB *t_pkb, std::queue<Grammar> t_selects, std::queue<Relation> t_relations, std::queue<Pattern> t_patterns, std::unordered_map<std::string, int> t_synonymsList)
    : m_pkb(t_pkb),
      m_selectedSynonym(""),
      m_selects(t_selects),
      m_relations(t_relations),
      m_patterns(t_patterns),
      m_synonymsUsedInQuery(t_synonymsList) {};

  /**
  * A destructor.
  */
  ~QueryEvaluator() {};

  /**
  * A public function to evaluate queries.
  * Get the results of all the clauses in the query from PKB, then compare and get the result of the query.
  * @return the result of the query in a vector of strings.
  */
  std::vector<std::string> evaluateQuery();

  /**
  * A public function to format integer vector to string vector.
  * Given a vector of integers, it will format it into a vector of strings.
  * @return the contents of a vector of integers in a vector of strings.
  */
  std::vector<std::string> formatVectorIntToVectorString(std::vector<int> t_vectorInt);

private:
  PKB *m_pkb; /**< A PKB pointer. The PKB instance that was created in the TestWrapper.cpp. */
  std::string m_selectedSynonym; /**< A string. The synonym that the query selects. */
  Grammar::GType m_selectedType; /**< A string. The type of the synonym that the query selects. */
  std::unordered_map<std::string, int> m_synonymsUsedInQuery; /**< A map of synonyms used and the number of times it has been used in the query. */
  std::queue<Grammar> m_selects; /**< A grammar queue. It stores the synonyms to be selected in the query. */
  std::queue<Relation> m_relations; /**< A relation queue. It stores the such that clauses in the query. */
  std::queue<Pattern> m_patterns; /**< A pattern queue. It stores the pattern clauses in the query. */
  std::queue<std::vector<std::string>> m_selectResults; /**< A list queue. It stores the results of the selected synonyms in the query. */
  std::queue<std::unordered_map<std::string, std::vector<std::string>>> m_relationResults; /**< An unordered map queue. It stores the results of the such that clauses in the query. */
  std::queue<std::unordered_map<std::string, std::vector<std::string>>> m_patternResults; /**< An unordered map queue. It stores the results of the pattern clauses in the query. */

  /**
  * Represents a queryType.
  * An enum type representing the queryType.
  */
  enum queryType {
    SELECT, /**< enum value SELECT. */
    RELATION, /**< enum value RELATION. */
    PATTERN /**< enum value PATTERN. */
  };

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
  * A private function to store the results which have common synonyms returned from PKB.
  * It takes in the result and the type of query to determine which queue to store the result in.
  * @param t_result An unordered map which holds the result returned from PKB.
  * @param t_type A enum value to indicate the type of query that the result belongs to.
  */
  void storeResultFromPkb(std::unordered_map<std::string, std::vector<std::string>> t_result, queryType t_type);

  /**
  * A private function to evaluate the final result of the query.
  * By comparing all the results with the common synonyms, get the final result of the query.
  * @return A vector of strings as the query result.
  */
  std::vector<std::string> evaluateFinalResult();

  /**
  * A private function to get the common results between two result vectors.
  * Given two vectors of string, it will compare and return the common results.
  * @param t_resultVector1 A string vector which holds a set of results.
  * @param t_resultVector2 A string vector which holds another set of results.
  * @return A vector of strings as the common results between two result vectors.
  */
  std::vector<std::string> getCommonResults(std::vector<std::string> t_resultVector1, std::vector<std::string> t_resultVector2);
};

#endif QUERYEVALUATOR_H