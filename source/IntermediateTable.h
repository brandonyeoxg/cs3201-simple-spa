#pragma once
#include "GlobalTypeDef.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class IntermediateTable {
public:
  void insertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS t_results);
  void insertTwoSynonym(SYNONYM_NAME t_synonym1, SYNONYM_NAME t_synonym2, SET_OF_RESULTS t_results);
  
  /*
  * Returns the results based on the synoynms selected.
  * Returns an empty list If no synonyms selected or synonyms selected does not exist in the intermediate table.
  * @param t_synonyms list of synonyms to determine which results is output in the intermediate table.
  */
  LIST_OF_RESULTS getResults(LIST_OF_SYNONYMS t_synonyms);

protected:
  INTERMEDIATE_TABLE m_results;
  MAP_OF_SYNONYM_TO_TABLE_POSITION m_synonymRowChecker;

  /*
  * Returns true if the intermediate table contains the synonym.
  */
  bool hasSynonym(SYNONYM_NAME t_synonym);

private:
  std::vector<bool> m_ignoreColumn;

  INTERMEDIATE_TABLE insertIntoEmptyTable(LIST_OF_RESULTS t_results);
  INTERMEDIATE_TABLE insertIntoEmptyTable(SET_OF_RESULTS t_results);
  INTERMEDIATE_TABLE getCartesianProduct(LIST_OF_RESULTS t_results);
  INTERMEDIATE_TABLE getCartesianProduct(SET_OF_RESULTS t_results);
  LIST_OF_RESULTS getCommonResults(LIST_OF_RESULTS t_results);

  /*
  * Removes all the non common results based on the key.
  */
  INTERMEDIATE_TABLE removeNonCommonResults(const SET_OF_RESULTS& t_results, const SYNONYM_NAME& t_synonym1, const SYNONYM_NAME& t_synonym2);
  
  /*
  * Inserts a synonym into the intermediate table.
  * If inserted synoynm is already in the intermediate table, it will be ignored.
  * @param t_synonym synonym to be inserted to the table.
  * @return the synonym row checker.
  */
  MAP_OF_SYNONYM_TO_TABLE_POSITION insertSynonym(const SYNONYM_NAME& t_synonym);
};