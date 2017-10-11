#include "IntermediateTable.h"

void IntermediateTable::insertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS t_results) {
  if (!hasSynonym(t_synonym)) {
    insertSynonym(t_synonym);
    if (m_results.empty()) {
      m_results = insertOneIntoEmptyTable(t_results);
    } else {
      m_results = getCartesianProduct(t_results);
    }
  } else {
    m_results = getCommonResults(t_synonym, t_results);
  }
}

void IntermediateTable::insertTwoSynonym(SYNONYM_NAME t_synonym1, SYNONYM_NAME t_synonym2, SET_OF_RESULTS t_results) {
  if (!hasSynonym(t_synonym1) && !hasSynonym(t_synonym2)) {
    insertSynonym(t_synonym1);
    insertSynonym(t_synonym2);
    if (m_results.empty()) {
      m_results = insertTwoIntoEmptyTable(t_results);
    } else {
      m_results = getCartesianProduct(t_results);
    }
  } else if (hasSynonym(t_synonym1) && !hasSynonym(t_synonym2)) {
    insertSynonym(t_synonym2);
    std::vector<std::string> resultsOfFirstSynonym;// = getResultsBySynonym(t_synonym1, t_results);
    m_results;// = getCartesianProduct((getCommonResults(resultsOfFirstSynonym)));
  } else if (!hasSynonym(t_synonym1) && hasSynonym(t_synonym2)) {
    insertSynonym(t_synonym1);
    std::vector<std::string> resultsOfSecondSynonym;// = getResultsBySynonym(t_synonym2, t_results);
    m_results;// = getCartesianProduct((getCommonResults(resultsOfSecondSynonym)));
  } else if (hasSynonym(t_synonym1) && hasSynonym(t_synonym2)) {
    m_results = removeNonCommonResults(t_results, t_synonym1, t_synonym2);
  }
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

bool IntermediateTable::hasSynonym(SYNONYM_NAME t_synonym) {
  auto rowItr = m_synonymRowChecker.find(t_synonym);
  return rowItr != m_synonymRowChecker.end();
}

MAP_OF_SYNONYM_TO_TABLE_POSITION IntermediateTable::insertSynonym(const SYNONYM_NAME& t_synonym) {
  if (m_synonymRowChecker.find(t_synonym) != m_synonymRowChecker.end()) {
    return m_synonymRowChecker;
  }
  int curSize = m_synonymRowChecker.size();
  m_synonymRowChecker.emplace(t_synonym, curSize);
  return m_synonymRowChecker;
}

SYNONYM_POSITION IntermediateTable::getIndexOfSynonym(SYNONYM_NAME t_synonym) {
  auto rowItr = m_synonymRowChecker.find(t_synonym);
  if (rowItr != m_synonymRowChecker.end()) {
    return rowItr->second;
  }

  return -1;
}

INTERMEDIATE_TABLE IntermediateTable::insertOneIntoEmptyTable(LIST_OF_RESULTS t_results) {
  for (int i = 0; i < t_results.size(); ++i) {
    m_results[i].push_back(t_results[i]);
  }

  return m_results;
}

INTERMEDIATE_TABLE IntermediateTable::insertTwoIntoEmptyTable(SET_OF_RESULTS t_results) {
  int i = 0;
  for (auto& x : t_results) {
    for (auto& y : x.second) {
      m_results[i].push_back(x.first);
      m_results[i].push_back(y);
      i++;
    }
  }

  return m_results;
}

INTERMEDIATE_TABLE IntermediateTable::getCartesianProduct(LIST_OF_RESULTS t_results) {
  INTERMEDIATE_TABLE newResults;
  int i = 0;
  for (auto& x : t_results) {
    for (auto& row : m_results) {
      newResults[i] = row;
      newResults[i].push_back(x);
      i++;
    }
  }

  return newResults;
}

INTERMEDIATE_TABLE IntermediateTable::getCartesianProduct(SET_OF_RESULTS t_results) {
  INTERMEDIATE_TABLE newResults;
  int i = 0;
  for (auto& x : t_results) {
    for (auto& y : x.second) {
      for (auto& row : m_results) {
        newResults[i] = row;
        newResults[i].push_back(x.first);
        newResults[i].push_back(y);
        i++;
      }
    }
  }

  return newResults;
}

INTERMEDIATE_TABLE IntermediateTable::getCommonResults(SYNONYM_NAME t_synonym, LIST_OF_RESULTS t_results) {
  INTERMEDIATE_TABLE newResults;
  SYNONYM_POSITION synPos = getIndexOfSynonym(t_synonym);

  int i = 0;
  for (auto& row : m_results) {
    for (auto& x : t_results) {
      if (row[synPos] == x) {
        newResults[i] = row;
        i++;
        break;
      }   
    }
  }

  return newResults;
}

// Both synoynm inside the the intermediate table
INTERMEDIATE_TABLE IntermediateTable::getCommonResults(SYNONYM_NAME& t_synonym1, SYNONYM_NAME& t_synonym2, SET_OF_RESULTS t_results) {
  INTERMEDIATE_TABLE newResults;
  SYNONYM_POSITION syn1Pos = getIndexOfSynonym(t_synonym1);
  SYNONYM_POSITION syn2Pos = getIndexOfSynonym(t_synonym2);

  int i = 0;
  for (auto& row : m_results) {
    for (auto& x : t_results) {
      if (row[syn1Pos] == x.first) {
        for (auto& y : x.second) {
          if (row[syn2Pos] == y) {
            newResults[i] = row;
            i++;
            break;
          }
        }
        break;
      }
    }
  }

  return newResults;
}