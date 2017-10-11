#include "IntermediateTableDriver.h"

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