#include "IntermediateTable.h"

bool IntermediateTable::insertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS_INDICES t_results) {
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

  if (m_results.empty()) {
    return false;
  }

  return true;
}

bool IntermediateTable::insertTwoSynonym(SYNONYM_NAME t_synonym1, SYNONYM_NAME t_synonym2, SET_OF_RESULTS_INDICES t_results) {
  if (!hasSynonym(t_synonym1) && !hasSynonym(t_synonym2)) {
    insertSynonym(t_synonym1);
    insertSynonym(t_synonym2);
    if (m_results.empty()) {
      if (t_synonym1 == t_synonym2) {
        LIST_OF_RESULTS_INDICES results = Formatter::formatMapIntVectorIntToVectorInt(t_results);
        m_results = insertOneIntoEmptyTable(results);
      } else {
        m_results = insertTwoIntoEmptyTable(t_results);
      }    
    } else {
      if (t_synonym1 == t_synonym2) {
        LIST_OF_RESULTS_INDICES results = Formatter::formatMapIntVectorIntToVectorInt(t_results);
        m_results = getCartesianProduct(results);
      } else {
        m_results = getCartesianProduct(t_results);
      }
    }
  } else if (hasSynonym(t_synonym1) && !hasSynonym(t_synonym2)) {
    insertSynonym(t_synonym2);
    m_results = getCartesianProductOfCommonResultsWithLeft(t_synonym1, t_results);
  } else if (!hasSynonym(t_synonym1) && hasSynonym(t_synonym2)) {
    insertSynonym(t_synonym1);
    m_results = getCartesianProductOfCommonResultsWithRight(t_synonym2, t_results);
  } else if (hasSynonym(t_synonym1) && hasSynonym(t_synonym2)) {
    m_results = getCommonResults(t_synonym1, t_synonym2, t_results);
  }

  if (m_results.empty()) {
    return false;
  }

  return true;
}

BOOLEAN IntermediateTable::mergeTables(std::vector<IntermediateTable*> t_tables) {
  for (auto& table : t_tables) {
    if (table->m_results.empty()) {
      continue;
    }

    for (auto& syn : table->m_synonyms) {
      std::unordered_map<SYNONYM_NAME, SYNONYM_POSITION>::const_iterator got = m_synonymRowChecker.find(syn.second);
      if (got == m_synonymRowChecker.end()) {
        insertSynonym(syn.second);
      }  
    }

    if (m_results.empty()) {
      m_results = table->m_results;
      continue;
    }

    INTERMEDIATE_TABLE intermediateResults;
    int rowNum = 0;
    for (auto& row : table->m_results) {
      for (auto& r : m_results) {
        if (intermediateResults.empty()) {
          intermediateResults.push_back(r);
          intermediateResults[rowNum].insert(intermediateResults[rowNum].end(), row.begin(), row.end());
          rowNum++;
          continue;
        }

        intermediateResults.push_back(r);
        intermediateResults[rowNum].insert(intermediateResults[rowNum].end(), row.begin(), row.end());
        rowNum++;
      }
    }

    m_results = intermediateResults;
  }

  if (m_results.empty()) {
    return false;
  }

  return true;
}

LIST_OF_RESULTS IntermediateTable::getResults(std::vector<Grammar> t_selectedSyn, PkbReadOnly *t_pkb) {
  LIST_OF_RESULTS output;
  if (t_selectedSyn.size() == 0) {
    return {};
  }
  int depthOfTable = m_results.size();
  for (int i = 0; i < depthOfTable; i++) {
    STRING_TOKEN result = "";
    for (auto& sItr : t_selectedSyn) {
      if (!Grammar::isStmtNo(sItr.getType()) && !Grammar::isString(sItr.getType())) {
        auto& synInTable = m_synonymRowChecker.find(sItr.getName());
        if (synInTable == m_synonymRowChecker.end()) {
          return{};
        }

        int synPosInTable = synInTable->second;

        if (Grammar::isCall(sItr.getType()) && Grammar::isProcName(sItr.getAttr())) {
          STMT_NUM callStmt = m_results[i][synPosInTable];
          PROC_NAME procName = t_pkb->getProcNameFromCallStmtNum(callStmt);
          result += procName + " ";
        } else if (Grammar::isProc(sItr.getType())) {
          PROC_NAME procName = t_pkb->getProcNameFromIdx(m_results[i][synPosInTable]);
          result += procName + " ";
        } else if (Grammar::isVar(sItr.getType())) {
          VAR_NAME varName = t_pkb->getVarNameFromIdx(m_results[i][synPosInTable]);
          result += varName + " ";
        } else if (Grammar::isConst(sItr.getType())) {
          CONSTANT_TERM constantTerm = t_pkb->getConstantFromIdx(m_results[i][synPosInTable]);
          result += constantTerm + " ";
        } else {
          result += std::to_string(m_results[i][synPosInTable]) + " ";
        }   
      } else {
        result += sItr.getName() + " ";
      }
    }

    if (!result.empty()) {
      result.pop_back();
      output.push_back(result);
    }   
  }
  return output;
}

bool IntermediateTable::hasSynonyms() {
  return !m_synonymRowChecker.empty();
}

bool IntermediateTable::hasSynonym(SYNONYM_NAME t_synonym) {
  auto rowItr = m_synonymRowChecker.find(t_synonym);
  return rowItr != m_synonymRowChecker.end();
}

bool IntermediateTable::isEmpty() {
  return m_results.empty();
}

void IntermediateTable::clearTable() {
  m_results.clear();
  m_synonymRowChecker.clear();
  m_synonyms.clear();
}

MAP_OF_SYNONYM_TO_TABLE_POSITION IntermediateTable::insertSynonym(const SYNONYM_NAME& t_synonym) {
  if (m_synonymRowChecker.find(t_synonym) != m_synonymRowChecker.end()) {
    return m_synonymRowChecker;
  }
  int curSize = m_synonymRowChecker.size();
  m_synonymRowChecker.emplace(t_synonym, curSize);
  m_synonyms.emplace(curSize, t_synonym);
  return m_synonymRowChecker;
}

SYNONYM_POSITION IntermediateTable::getIndexOfSynonym(SYNONYM_NAME t_synonym) {
  auto rowItr = m_synonymRowChecker.find(t_synonym);
  if (rowItr != m_synonymRowChecker.end()) {
    return rowItr->second;
  }

  return -1;
}

INTERMEDIATE_TABLE IntermediateTable::insertOneIntoEmptyTable(LIST_OF_RESULTS_INDICES t_results) {
  for (int i = 0; i < t_results.size(); ++i) {
    m_results.push_back({});
    m_results[i].push_back(t_results[i]);
  }

  return m_results;
}

INTERMEDIATE_TABLE IntermediateTable::insertTwoIntoEmptyTable(SET_OF_RESULTS_INDICES t_results) {
  int i = 0;
  for (auto& x : t_results) {
    for (auto& y : x.second) {
      m_results.push_back({});
      m_results[i].push_back(x.first);
      m_results[i].push_back(y);
      i++;
    }
  }

  return m_results;
}

INTERMEDIATE_TABLE IntermediateTable::getCartesianProduct(LIST_OF_RESULTS_INDICES t_results) {
  INTERMEDIATE_TABLE newResults;

  int i = 0;
  for (auto& x : t_results) {
    for (auto& row : m_results) {
      newResults.push_back(row);
      newResults[i].push_back(x);
      i++;
    }
  }

  return newResults;
}

INTERMEDIATE_TABLE IntermediateTable::getCartesianProduct(SET_OF_RESULTS_INDICES t_results) {
  INTERMEDIATE_TABLE newResults;
  int i = 0;
  for (auto& x : t_results) {
    for (auto& y : x.second) {
      for (auto& row : m_results) {
        newResults.push_back(row);
        newResults[i].push_back(x.first);
        newResults[i].push_back(y);
        i++;
      }
    }
  }

  return newResults;
}

INTERMEDIATE_TABLE IntermediateTable::getCommonResults(SYNONYM_NAME t_synonym, LIST_OF_RESULTS_INDICES t_results) {
  INTERMEDIATE_TABLE newResults;
  SYNONYM_POSITION synPos = getIndexOfSynonym(t_synonym);

  for (auto& row : m_results) {
    for (auto& x : t_results) {
      if (row[synPos] == x) {
        newResults.push_back(row);
        break;
      }   
    }
  }

  return newResults;
}

// Both synoynm inside the the intermediate table
INTERMEDIATE_TABLE IntermediateTable::getCommonResults(SYNONYM_NAME& t_synonym1, SYNONYM_NAME& t_synonym2, SET_OF_RESULTS_INDICES t_results) {
  INTERMEDIATE_TABLE newResults;
  SYNONYM_POSITION syn1Pos = getIndexOfSynonym(t_synonym1);
  SYNONYM_POSITION syn2Pos = getIndexOfSynonym(t_synonym2);

  for (auto& row : m_results) {
    for (auto& x : t_results) {
      if (row[syn1Pos] == x.first) {
        for (auto& y : x.second) {
          if (row[syn2Pos] == y) {
            newResults.push_back(row);
            break;
          }
        }
        break;
      }
    }
  }

  return newResults;
}

INTERMEDIATE_TABLE IntermediateTable::getCartesianProductOfCommonResultsWithLeft(SYNONYM_NAME& t_synonym, SET_OF_RESULTS_INDICES t_results) {
  INTERMEDIATE_TABLE newResults;
  SYNONYM_POSITION synPos = getIndexOfSynonym(t_synonym);

  int i = 0;
  for (auto& row : m_results) {
    for (auto& x : t_results) {
      if (row[synPos] == x.first) {
        for (auto& y : x.second) {
          newResults.push_back(row);
          newResults[i].push_back(y);
          i++;
        }
        break;
      }
    }
  }

  return newResults;
}

INTERMEDIATE_TABLE IntermediateTable::getCartesianProductOfCommonResultsWithRight(SYNONYM_NAME& t_synonym, SET_OF_RESULTS_INDICES t_results) {
  INTERMEDIATE_TABLE newResults;
  SYNONYM_POSITION synPos = getIndexOfSynonym(t_synonym);

  int i = 0;
  for (auto& row : m_results) {
    for (auto& x : t_results) {   
      for (auto& y : x.second) {
        if (row[synPos] == y) {
          newResults.push_back(row);
          newResults[i].push_back(x.first);
          i++;
          break;
        }
      }
    }
  }

  return newResults;
}