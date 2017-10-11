#include "IntermediateTable.h"

bool IntermediateTable::hasSynonym(SYNONYM_NAME t_name) {
  auto rowItr = m_synonymRowChecker.find(t_name);
  return rowItr != m_synonymRowChecker.end();
}

LIST_OF_RESULTS IntermediateTable::getResults(LIST_OF_SYNONYMS t_selectedSyn) {
  LIST_OF_RESULTS output;
  if (t_selectedSyn.size() == 0) {
    return {};
  }
  int depthOfTable = m_results.size();
  for (int i = 0; i < depthOfTable; i++) {
    STRING_TOKEN result = "";
    for (auto& sItr : t_selectedSyn) {
      auto& synInTable = m_synonymRowChecker.find(sItr);
      if (synInTable == m_synonymRowChecker.end()) {
        return {};
      }
      int synPosInTable = synInTable->second;
      result += m_results[i][synPosInTable] + " ";
    }
    result.pop_back();
    output.push_back(result);
  }
  return output;
}

// Both synoynm inside the the intermediate table
INTERMEDIATE_TABLE IntermediateTable::removeNonCommonResults(const SET_OF_RESULTS& t_results, const SYNONYM_NAME& t_name1, const SYNONYM_NAME& t_name2) {
  return m_results;
}

MAP_OF_SYNONYM_TO_TABLE_POSITION IntermediateTable::insertSynonym(const SYNONYM_NAME& t_name) {
  if (m_synonymRowChecker.find(t_name) != m_synonymRowChecker.end()) {
    return m_synonymRowChecker;
  }
  int curSize = m_synonymRowChecker.size();
  m_synonymRowChecker.emplace(t_name, curSize);
  return m_synonymRowChecker;
}