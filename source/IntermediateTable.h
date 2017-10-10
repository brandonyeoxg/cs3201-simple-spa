#pragma once
#include "GlobalTypeDef.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class IntermediateTable {
public:
  void insertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS t_results);
  void insertTwoSynonym(SYNONYM_NAME t_synonym1, SYNONYM_NAME t_synonym2, SET_OF_RESULTS t_results);
  LIST_OF_RESULTS getResults(LIST_OF_SYNONYMS t_synonymList);

protected:
  INTERMEDIATE_TABLE m_results;
  std::unordered_map<SYNONYM_NAME, SYNONYM_POSITION> m_synonymRowChecker;

  bool hasSynonym(SYNONYM_NAME t_synonym);

private:
  std::vector<bool> m_ignoreColumn;

  INTERMEDIATE_TABLE getCartesianProduct(LIST_OF_RESULTS t_results);
  INTERMEDIATE_TABLE getCartesianProduct(SET_OF_RESULTS t_results);
  LIST_OF_RESULTS getCommonResults(LIST_OF_RESULTS t_results);
  void removeNonCommonResults(SET_OF_RESULTS t_results);
};