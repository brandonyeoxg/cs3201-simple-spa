#include "VarTable.h"

VAR_INDEX VarTable::insertVar(VAR_NAME t_name) {
  VAR_INDEX idx = m_varIdxToName.size();
  m_varIdxToName.push_back(t_name);
  m_varNameToIdx.emplace(t_name, idx);
  return idx;
}

std::string VarTable::getVarNameFromIdx(VAR_INDEX t_idx) {
  return m_varIdxToName[t_idx];
}

VAR_INDEX VarTable::getVarIdxFromName(VAR_NAME t_name) {
  auto pItr = m_varNameToIdx.find(t_name);
  if (pItr == m_varNameToIdx.end()) {
    return NULL;
  }
  return pItr->second;
}

LIST_OF_VAR_NAMES& VarTable::getAllVarNames() {
  return m_varIdxToName;
}

