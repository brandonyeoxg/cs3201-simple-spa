#include "IntermediateTableDriver.h"

void IntermediateTableDriver::clearTable() {
  m_results.clear();
  m_synonymRowChecker.clear();
}

void IntermediateTableDriver::insertTable(const INTERMEDIATE_TABLE &t_results) {
  m_results = t_results;
}

void IntermediateTableDriver::insertSynonymRow(std::vector<SYNONYM_NAME> m_synonymRow) {
  int i = 0;
  for (auto& synonym : m_synonymRow) {
    auto itr = m_synonymRowChecker.find(synonym);
    if (itr == m_synonymRowChecker.end()) {
      m_synonymRowChecker.emplace(synonym, i);
    }
    i += 1;
  }
}

bool IntermediateTableDriver::TestHasSynonym(SYNONYM_NAME t_name) {
  return hasSynonym(t_name);
}

INTERMEDIATE_TABLE IntermediateTableDriver::TestInsertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS_INDICES t_results) {
  insertOneSynonym(t_synonym, t_results);
  return m_results;
}

INTERMEDIATE_TABLE IntermediateTableDriver::TestInsertTwoSynonym(SYNONYM_NAME t_synonym1, SYNONYM_NAME t_synonym2, SET_OF_RESULTS_INDICES t_results) {
  insertTwoSynonym(t_synonym1, t_synonym2, t_results);
  return m_results;
}