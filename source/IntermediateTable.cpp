#include "IntermediateTable.h"

bool IntermediateTable::hasSynonym(SYNONYM_NAME t_name) {
  auto rowItr = m_synonymRowChecker.find(t_name);
  return rowItr != m_synonymRowChecker.end();
}

LIST_OF_RESULTS IntermediateTable::getResults(LIST_OF_SYNONYMS t_selectedSyn) {
  LIST_OF_RESULTS output;
  for (SYNONYM_NAME name : t_selectedSyn) {
    // Obtain the index by the syn row
    auto rItr =  m_synonymRowChecker.find(name);
    if (rItr == m_synonymRowChecker.end()) {
      continue;
    }
    SYNONYM_POSITION synPos = rItr->second;
    auto resultList = m_results[synPos];
    if (output.size() != resultList.size()) {
      for (auto& result : resultList) {
        output.push_back(result);
      }
    } else {
      // Obtains from the results and append
      int i = 0;
      for (auto& result : resultList) {
        std::string newStr = output[i] + " " + result;
        output[i] = newStr;
        i += 1;
      }
    }
  }
  return output;
}

