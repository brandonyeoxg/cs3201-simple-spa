#include "ProcTable.h"

PROC_INDEX ProcTable::insertProc(PROC_NAME t_name) {
  PROC_INDEX idx = m_procIdxToName.size();
  m_procIdxToName.push_back(t_name);
  m_procNameToIdx.emplace(t_name, idx);
  return idx;
}

std::string ProcTable::getProcNameFromIdx(PROC_INDEX t_idx) {
  return m_procIdxToName[t_idx];
}

PROC_INDEX ProcTable::getProcIdxFromName(PROC_NAME t_name) {
  auto pItr = m_procNameToIdx.find(t_name);
  if (pItr == m_procNameToIdx.end()) {
    return NULL;
  }
  return pItr->second;
}