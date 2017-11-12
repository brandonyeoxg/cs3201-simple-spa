#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

#include "GlobalTypeDef.h"
#include "pql/query-types/Grammar.h"
#include "pkb/PkbReadOnly.h"

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

  /**
  * A public function to filter a statement by the statement type.
  * It will return the stmt in a vector if it matches a specific stmt type else return an empty vector.
  * @param t_typeOfStmts A map of all the stmts in the simple source program with its type of stmt.
  * @param t_stmtNo The stmt number to be filtered. 
  * @param t_grammar A grammar object with the specific stmt type.
  * @return a vector of the stmt if it matches a specific stmt type else returns an empty vector.
  */
  static LIST_OF_STMT_NUMS filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar);
  
  /**
  * A public function to filter the statements in a vector by the statement type.
  * It will return the vector of stmts that matches the specific stmt type.
  * @param t_typeOfStmts A map of all the stmts in the simple source program with its type of stmt.
  * @param t_stmtIntVector The vector of stmts to be filtered.
  * @param t_grammar A grammar object with the specific stmt type.
  * @return a vector of stmts that matches a specific stmt type.
  */
  static LIST_OF_STMT_NUMS filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, LIST_OF_STMT_NUMS t_stmtIntVector, Grammar t_grammar);
  
  /**
  * A public function to filter a statement by the statement type.
  * It will return the given vector if it matches a specific stmt type else return an empty vector.
  * @param t_typeOfStmts A map of all the stmts in the simple source program with its type of stmt.
  * @param t_stmtNo The stmt number to be filtered.
  * @param t_grammar A grammar object with the specific stmt type.
  * @param t_stmtIntVector The vector of stmts to be stored.
  * @return the given vector if the stmt matches a specific stmt type else returns an empty vector.
  */
  static LIST_OF_STMT_NUMS filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar, LIST_OF_STMT_NUMS t_stmtVector);

  /**
  * A public function to get the common results between two result vectors.
  * Given two vectors of string, it will compare and return the common results.
  * @param t_resultVector1 A string vector which holds a set of results.
  * @param t_resultVector2 A string vector which holds another set of results.
  * @return A vector of strings as the common results between two result vectors.
  */
  static LIST_OF_RESULTS getCommonResults(LIST_OF_RESULTS t_resultVector1, LIST_OF_RESULTS t_resultVector2);

  /**
  * A public function to get the common program lines and constant values.
  * Return all the constants that are less than or equals to the maximum program line.
  * @param t_allConstants A string vector which holds all the constants.
  * @param t_maxProgLine A number that indicates the maximum program line.
  * @return An unordered map of constants that are less than or equals to the maximum program line.
  */
  static SET_OF_RESULTS_INDICES getCommonProgLineAndConstant(LIST_OF_CONSTANT_INDICES t_allConstants, int t_maxProgLine, PkbReadOnly *t_pkb);

  /**
  * A public function to rewrite synonym of the given grammar object.
  * Checks whether the synonym of the given grammar object is in the rewriteList,
  * if yes then return the new grammar else return the original grammar.
  * @param t_grammar A grammar object.
  * @param t_rewriteList A list with the synonyms to be rewritten.
  * @return A new grammar object if the synonym is in the rewriteList else return the original grammar object.
  */
  static Grammar rewriteSynonym(Grammar t_grammar, std::unordered_map<SYNONYM_NAME, Grammar> t_rewriteList);

  /**
  * A public function to filter same results for same synonyms in Next*(s, s), Affects(s, s) and Affects*(s, s) clauses.
  * Get only results that are the same for both parameters.
  * @param t_results The results to filter.
  * @return The filtered results.
  */
  static MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS filterSameResultsForSameSynonyms(MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS t_results);
};
