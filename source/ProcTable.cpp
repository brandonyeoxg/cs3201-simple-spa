#include "ProcTable.h"
#include <assert.h>
#include <algorithm>

PROC_INDEX ProcTable::insertProc(PROC_NAME t_name) {
  if (std::find(m_procIdxToName.begin(), m_procIdxToName.end(), t_name) != m_procIdxToName.end()) {
    return INVALID_INDEX;
  }
  
  PROC_INDEX idx = m_procIdxToName.size();
  m_procIdxToName.push_back(t_name);
  m_procNameToIdx.emplace(t_name, idx);
  return idx;
}

PROC_NAME ProcTable::getProcNameFromIdx(PROC_INDEX t_idx) {
  if (t_idx >= m_procIdxToName.size()) {
    return std::string();
  }
  return m_procIdxToName[t_idx];
}

PROC_INDEX ProcTable::getProcIdxFromName(PROC_NAME t_name) {
  auto pItr = m_procNameToIdx.find(t_name);
  if (pItr == m_procNameToIdx.end()) {
    return INVALID_INDEX;
  }
  return pItr->second;
}

LIST_OF_PROC_NAMES& ProcTable::getAllProcsName() {
  return m_procIdxToName;
}