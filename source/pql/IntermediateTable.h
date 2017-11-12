#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "../GlobalTypeDef.h"
#include "query-types/Grammar.h"
#include "../pkb/PkbReadOnly.h"
#include "../Formatter.h"

class IntermediateTable {
public:

  BOOLEAN insertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS_INDICES t_results);
  BOOLEAN insertTwoSynonym(SYNONYM_NAME t_synonym1, SYNONYM_NAME t_synonym2, SET_OF_RESULTS_INDICES t_results);
  BOOLEAN mergeTables(std::vector<IntermediateTable*> t_tables);
  
  /*
  * Returns the results based on the synoynms selected.
  * Returns an empty list If no synonyms selected or synonyms selected does not exist in the intermediate table.
  * @param t_synonyms list of synonyms to determine which results is output in the intermediate table.
  */
  LIST_OF_RESULTS getResults(std::vector<Grammar> t_synonyms, PkbReadOnly *t_pkb);
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