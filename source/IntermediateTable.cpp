#include "IntermediateTable.h"

void IntermediateTable::insertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS t_results) {
  if (!hasSynonym(t_synonym)) {
    //insertSynonym(t_synonym);
    m_results = getCartesianProduct(t_results);
  } else {
    m_results = getCartesianProduct(getCommonResults(t_results));
  }
}

void IntermediateTable::insertTwoSynonym(SYNONYM_NAME t_synonym1, SYNONYM_NAME t_synonym2, SET_OF_RESULTS t_results) {
  if (!hasSynonym(t_synonym1) && !hasSynonym(t_synonym2)) {
    //insertSynonym(t_synonym1);
    //insertSynonym(t_synonym2);
    m_results = getCartesianProduct(t_results);
  } else if (hasSynonym(t_synonym1) && !hasSynonym(t_synonym2)) {
    //insertSynonym(t_synonym2);
    std::vector<std::string> resultsOfFirstSynonym;// = getResultsBySynonym(t_synonym1, t_results);
    m_results = getCartesianProduct((getCommonResults(resultsOfFirstSynonym)));
  } else if (!hasSynonym(t_synonym1) && hasSynonym(t_synonym2)) {
    //insertSynonym(t_synonym1);
    std::vector<std::string> resultsOfSecondSynonym;// = getResultsBySynonym(t_synonym2, t_results);
    m_results = getCartesianProduct((getCommonResults(resultsOfSecondSynonym)));
  } else if (hasSynonym(t_synonym1) && hasSynonym(t_synonym2)) {
    removeNonCommonResults(t_results);
    m_results = getCartesianProduct(t_results);
  }
}

LIST_OF_RESULTS IntermediateTable::getResults(LIST_OF_SYNONYMS t_selectedSyn) {
  LIST_OF_RESULTS output;
  for (SYNONYM_NAME name : t_selectedSyn) {
    // Obtain the index by the syn row
    auto rItr = m_synonymRowChecker.find(name);
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

bool IntermediateTable::hasSynonym(SYNONYM_NAME t_name) {
  auto rowItr = m_synonymRowChecker.find(t_name);
  return rowItr != m_synonymRowChecker.end();
}

INTERMEDIATE_TABLE IntermediateTable::getCartesianProduct(LIST_OF_RESULTS t_results) {

}

INTERMEDIATE_TABLE IntermediateTable::getCartesianProduct(SET_OF_RESULTS t_results) {

}

LIST_OF_RESULTS IntermediateTable::getCommonResults(LIST_OF_RESULTS t_results) {

}