#pragma once
#include "GlobalTypeDef.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class IntermediateTable {
public:
  void insertOneSynonym(SYNONYM_NAME t_name, LIST_OF_RESULTS);
  void insertTwoSynonym(SYNONYM_NAME, SYNONYM_NAME, SET_OF_RESULTS);
  
  /*
  * Returns the results based on the synoynms selected.
  * Returns an empty list If no synonyms selected or synonyms selected does not exist in the intermediate table.
  * 
  * @param t_synonyms list of synonyms to determine which results is output in the intermediate table.
  */
  LIST_OF_RESULTS getResults(LIST_OF_SYNONYMS t_synonyms);

protected:
  INTERMEDIATE_TABLE m_results;
  MAP_OF_SYNONYM_TO_TABLE_POSITION m_synonymRowChecker;

  /*
  * Returns true if the intermediate table contains the synonym
  */
  bool hasSynonym(SYNONYM_NAME);

private:
  std::vector<bool> m_ignoreColumn;

  INTERMEDIATE_TABLE getCartesianProduct(LIST_OF_RESULTS);
  INTERMEDIATE_TABLE getCartesianProduct(SET_OF_RESULTS);
  LIST_OF_RESULTS getCommonResults(LIST_OF_RESULTS);

  /*
  * Removes all the non common results based on the key.
  */
  INTERMEDIATE_TABLE removeNonCommonResults(const SET_OF_RESULTS& t_results, const SYNONYM_NAME& t_name1, const SYNONYM_NAME& t_name2);
  
  /*
  * Inserts a synonym into the intermediate table.
  * If inserted synoynm is already in the intermediate table, it will be ignored
  *
  * @param t_name name of the synonym cannot be empty string
  */
  MAP_OF_SYNONYM_TO_TABLE_POSITION insertSynonym(const SYNONYM_NAME& t_name);
};