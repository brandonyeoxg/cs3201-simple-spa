#pragma once
#include "GlobalTypeDef.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class IntermediateTable {
public:
  void insertOneSynonym(SYNONYM_NAME t_name, LIST_OF_RESULTS);
  void insertTwoSynonym(SYNONYM_NAME, SYNONYM_NAME, SET_OF_RESULTS);
  LIST_OF_RESULTS getResults(LIST_OF_SYNONYMS);

protected:
  INTERMEDIATE_TABLE m_results;
  std::unordered_map<SYNONYM_NAME, SYNONYM_POSITION> m_synonymRowChecker;

  bool hasSynonym(SYNONYM_NAME);

private:
  std::vector<bool> m_ignoreColumn;

  INTERMEDIATE_TABLE getCartesianProduct(LIST_OF_RESULTS);
  INTERMEDIATE_TABLE getCartesianProduct(SET_OF_RESULTS);
  LIST_OF_RESULTS getCommonResults(LIST_OF_RESULTS);
  void removeNonCommonResults(SET_OF_RESULTS);
};