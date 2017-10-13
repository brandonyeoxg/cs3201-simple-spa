#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"

class IntermediateTable {
public:
  bool insertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS t_results);
  bool insertTwoSynonym(SYNONYM_NAME t_synonym1, SYNONYM_NAME t_synonym2, SET_OF_RESULTS t_results);
  
  /*
  * Returns the results based on the synoynms selected.
  * Returns an empty list If no synonyms selected or synonyms selected does not exist in the intermediate table.
  * @param t_synonyms list of synonyms to determine which results is output in the intermediate table.
  */
  LIST_OF_RESULTS getResults(LIST_OF_SYNONYMS t_synonyms);
  bool isEmpty();
  void clearTable();

protected:
  INTERMEDIATE_TABLE m_results;
  MAP_OF_SYNONYM_TO_TABLE_POSITION m_synonymRowChecker;

  /*
  * Returns true if the intermediate table contains the synonym.
  */
  bool hasSynonym(SYNONYM_NAME t_synonym);

private:
  /*
  * Inserts a synonym into the intermediate table.
  * If inserted synoynm is already in the intermediate table, it will be ignored.
  * @param t_synonym synonym to be inserted to the table.
  * @return the synonym row checker.
  */
  MAP_OF_SYNONYM_TO_TABLE_POSITION insertSynonym(const SYNONYM_NAME& t_synonym);
  SYNONYM_POSITION getIndexOfSynonym(SYNONYM_NAME t_synonym);
  INTERMEDIATE_TABLE insertOneIntoEmptyTable(LIST_OF_RESULTS t_results);
  INTERMEDIATE_TABLE insertTwoIntoEmptyTable(SET_OF_RESULTS t_results);
  INTERMEDIATE_TABLE getCartesianProduct(LIST_OF_RESULTS t_results);
  INTERMEDIATE_TABLE getCartesianProduct(SET_OF_RESULTS t_results);
  INTERMEDIATE_TABLE getCommonResults(SYNONYM_NAME t_synonym, LIST_OF_RESULTS t_results);
  INTERMEDIATE_TABLE getCommonResults(SYNONYM_NAME& t_synonym1, SYNONYM_NAME& t_synonym2, SET_OF_RESULTS t_results);
  INTERMEDIATE_TABLE getCartesianProductOfCommonResultsWithLeft(SYNONYM_NAME& t_synonym, SET_OF_RESULTS t_results);
  INTERMEDIATE_TABLE getCartesianProductOfCommonResultsWithRight(SYNONYM_NAME& t_synonym, SET_OF_RESULTS t_results);
};