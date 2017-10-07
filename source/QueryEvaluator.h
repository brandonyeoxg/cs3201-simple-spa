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
  * A public function to filter the keys by the GType.
  * It will filter the keys of the results by the GType.
  * @param t_typeOfStmts A map of all the statements in the simple source program with its type of statement.
  * @param t_results A map of results to be filtered.
  * @return a result filtered by the GType in a string vector.
  */
  std::vector<std::string> filterKeyResults(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::unordered_map<std::string, std::vector<std::string>> t_results);

  /**
  * A public function to filter the values by the GType.
  * It will filter the values of the results by the GType.
  * @param t_typeOfStmts A map of all the statements in the simple source program with its type of statement.
  * @param t_results A map of results to be filtered.
  * @return a result filtered by the GType in a string vector.
  */
  std::vector<std::string> filterValueResults(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::unordered_map<std::string, std::vector<std::string>> t_results);

  /**
  * A public function to format integer vector to string vector.
  * Given a vector of integers, it will format it into a vector of strings.
  * @return the contents of a vector of integers in a vector of strings.
  */
  std::vector<std::string> formatVectorIntToVectorString(std::vector<int> t_vectorInt);

  /**
  * A public function to format string list to string vector.
  * Given a list of strings, it will format it into a vector of strings.
  * @return the contents of a list of string in a vector of strings.
  */
  std::vector<std::string> formatListStringToVectorString(std::list<std::string> t_listStr);

  /**
  * A public function to check if both parameters are underscores.
  * Given two parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a relation clause.
  * @param t_g2 A grammar object which holds the second parameter of a relation clause.
  * @return true if both parameters are underscores else return false.
  */
  bool isAllUnderscores(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if both parameters are not synonyms.
  * Given two parameters, it will check if both parameters are not synonyms.
  * @param t_g1 A grammar object which holds the first parameter of a relation clause.
  * @param t_g2 A grammar object which holds the second parameter of a relation clause.
  * @return true if both parameters are not synonyms else return false.
  */
  bool hasNoSynonyms(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if only the second paramter is a synonym.
  * Given two parameters, it will check if the first parameter is not a synonym
  * and the second parameter is a synonym.
  * @param t_g1 A grammar object which holds the first parameter of a relation clause.
  * @param t_g2 A grammar object which holds the second parameter of a relation clause.
  * @return true if only the second parameter is a synonym else return false.
  */
  bool hasOneRightSynonym(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if only the first paramter is a synonym.
  * Given two parameters, it will check if the first parameter is a synonym
  * and the second parameter is not a synonym.
  * @param t_g1 A grammar object which holds the first parameter of a relation clause.
  * @param t_g2 A grammar object which holds the second parameter of a relation clause.
  * @return true if only the first parameter is a synonym else return false.
  */
  bool hasOneLeftSynonym(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if both paramters are synonyms.
  * Given two parameters, it will check if both parameters are synonyms.
  * @param t_g1 A grammar object which holds the first parameter of a relation clause.
  * @param t_g2 A grammar object which holds the second parameter of a relation clause.
  * @return true if both parameters are synonyms else return false.
  */
  bool hasTwoSynonyms(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is anything with any pattern.
  * Given the pattern parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @return true if both parameters are underscores.
  */
  bool isAnythingWithAnyPattern(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is anything with an exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is true.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is true.
  */
  bool isAnythingWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the pattern clause is anything with a non-exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is false.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is false.
  */
  bool isAnythingWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the pattern clause is anything with any pattern.
  * Given the pattern parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @return true if both parameters are underscores.
  */
  bool isVarWithAnyPattern(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is anything with an exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is true.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is true.
  */
  bool isVarWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the pattern clause is anything with a non-exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is false.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is false.
  */
  bool isVarWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the pattern clause is anything with any pattern.
  * Given the pattern parameters, it will check if both parameters are underscores.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @return true if both parameters are underscores.
  */
  bool isSynonymWithAnyPattern(Grammar t_g1, Grammar t_g2);

  /**
  * A public function to check if the pattern clause is anything with an exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is true.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is true.
  */
  bool isSynonymWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

  /**
  * A public function to check if the pattern clause is anything with a non-exact pattern.
  * Given the pattern parameters, it will check if the first parameter is underscore
  * and the second parameter is a string and the third parameter is false.
  * @param t_g1 A grammar object which holds the first parameter of a pattern clause.
  * @param t_g2 A grammar object which holds the second parameter of a pattern clause.
  * @param t_isExact A boolean to indicate whether the pattern has to be an exact match.
  * @return true if the first parameter is underscore and the second parameter is a string
  * and the third parameter is false.
  */
  bool isSynonymWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact);

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
  void storeResultFromPkb(std::unordered_map<std::string, std::vector<std::string>> t_result, queryType::clauseType t_type);

  /**
  * A private function to store the relation result if it is needed.
  * It takes in the relation and the relation result to check whether there is a need to store it.
  * @param t_relation A relation object which holds the relation clause that was evaluated to the parameter t_result.
  * @param t_result An unordered map which holds the result of the relation clause returned from PKB.
  */
  void storeRelationResultFromPkb(Relation t_relation, std::unordered_map<std::string, std::vector<std::string>> t_result);
  
  /**
  * A private function to store the pattern result if it is needed.
  * It takes in the pattern and the pattern result to check whether there is a need to store it.
  * @param t_pattern A pattern object which holds the pattern clause that was evaluated to the parameter t_result.
  * @param t_result An unordered map which holds the result of the pattern clause returned from PKB.
  */
  void storePatternResultFromPkb(Pattern t_pattern, std::unordered_map<std::string, std::vector<std::string>> t_result);

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