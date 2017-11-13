#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "../GlobalTypeDef.h"
#include "query-types/Grammar.h"
#include "../pkb/PkbReadOnly.h"
#include "../util/Formatter.h"

class IntermediateTable {
public:

  /*
  * Insert the results of one synonym.
  * Insert the results of one synonym into the intermediate result table.
  * @param t_synonym the synonym to be inserted into the result table.
  * @param t_results the results of the synonym to be inserted into the result table.
  * @return true if the results of the synonym is inserted into the intermediate result table successfully
  * else return false if the results are not inserted into the intermediate result table.
  */
  BOOLEAN insertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS_INDICES t_results);

  /*
  * Insert the results of two synonyms.
  * Insert the results of two synonyms into the intermediate result table.
  * @param t_synonym1 the synonym to be inserted into the result table.
  * @param t_synonym1 the synonym to be inserted into the result table.
  * @param t_results the results of the synonym to be inserted into the result table.
  * @return true if the results of the synonym is inserted into the intermediate result table successfully
  * else return false if the results are not inserted into the intermediate result table.
  */
  BOOLEAN insertTwoSynonym(SYNONYM_NAME t_synonym1, SYNONYM_NAME t_synonym2, SET_OF_RESULTS_INDICES t_results);

  /*
  * Merge the vector of intermediate result tables into one intermediate result table. 
  * Loop through all the intermediate result table to merge together.
  * @param t_tables the intermediate result tables of all the groups to be merged.
  * @return true if the results of the tables have been merged successfully
  * else return false if the tables are not merged successfully.
  */
  BOOLEAN mergeTables(std::vector<IntermediateTable*> t_tables);
  
  /*
  * Returns the results based on the synoynms selected.
  * Returns an empty list If no synonyms selected or synonyms selected does not exist in the intermediate table.
  * @param t_synonyms list of synonyms to determine which results is output in the intermediate table.
  */
  LIST_OF_RESULTS getResults(std::vector<Grammar> t_synonyms, PkbReadOnly *t_pkb);

  /*
  * Returns true if the intermediate table has synonyms in the table.
  */
  BOOLEAN hasSynonyms();

  /*
  * Returns true if the intermediate table contains the synonym.
  */
  BOOLEAN hasSynonym(SYNONYM_NAME t_synonym);
  BOOLEAN isEmpty();
  void clearTable();

protected:
  INTERMEDIATE_TABLE m_results;
  MAP_OF_SYNONYM_TO_TABLE_POSITION m_synonymRowChecker;
  MAP_OF_TABLE_POSITION_TO_SYNONYM m_synonyms;

private:
  /*
  * Inserts a synonym into the intermediate table.
  * If inserted synoynm is already in the intermediate table, it will be ignored.
  * @param t_synonym synonym to be inserted to the table.
  * @return the synonym row checker.
  */
  MAP_OF_SYNONYM_TO_TABLE_POSITION insertSynonym(const SYNONYM_NAME& t_synonym);
  SYNONYM_POSITION getIndexOfSynonym(SYNONYM_NAME t_synonym);
  INTERMEDIATE_TABLE insertOneIntoEmptyTable(LIST_OF_RESULTS_INDICES t_results);
  INTERMEDIATE_TABLE insertTwoIntoEmptyTable(SET_OF_RESULTS_INDICES t_results);
  INTERMEDIATE_TABLE getCartesianProduct(LIST_OF_RESULTS_INDICES t_results);
  INTERMEDIATE_TABLE getCartesianProduct(SET_OF_RESULTS_INDICES t_results);
  INTERMEDIATE_TABLE getCommonResults(SYNONYM_NAME t_synonym, LIST_OF_RESULTS_INDICES t_results);
  INTERMEDIATE_TABLE getCommonResults(SYNONYM_NAME& t_synonym1, SYNONYM_NAME& t_synonym2, SET_OF_RESULTS_INDICES t_results);
  INTERMEDIATE_TABLE getCartesianProductOfCommonResultsWithLeft(SYNONYM_NAME& t_synonym, SET_OF_RESULTS_INDICES t_results);
  INTERMEDIATE_TABLE getCartesianProductOfCommonResultsWithRight(SYNONYM_NAME& t_synonym, SET_OF_RESULTS_INDICES t_results);
};