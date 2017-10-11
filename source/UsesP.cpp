#include "UsesP.h"

void UsesP::insertUsesP(PROC_INDEX t_procIdx, const PROC_NAME& t_procName, VAR_INDEX t_varIdx, const VAR_NAME& t_varName) {
  insertUsesWithProcAsKey(t_procIdx, t_varName);
  insertUsesWithProcAsKeyWithVarHash(t_procIdx, t_varIdx);
  insertUsesWithVarAsKey(t_varIdx, t_procName);
  insertUsesProcName(t_procName);
  insertUsesProcAndVarName(t_procName, t_varName);
}

void UsesP::insertUsesWithProcAsKey(PROC_INDEX t_procIdx, const VAR_NAME& t_varName) {
  auto mItr = m_procToVarNames.find(t_procIdx);
  if (mItr == m_procToVarNames.end()) {
    std::vector<VAR_NAME> tempV = { t_varName };
    m_procToVarNames.emplace(t_procIdx, tempV);
    return;
  }
  for (auto& vItr : mItr->second) {
    if (vItr == t_varName) {
      return;
    }
  }
  mItr->second.push_back(t_varName);
}

void UsesP::insertUsesWithProcAsKeyWithVarHash(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx) {
  auto vItr = m_procToVarSet.find(t_procIdx);
  if (vItr == m_procToVarSet.end()) {
    VAR_HASH_SET tempVSet;
    tempVSet.insert(t_varIdx);
    m_procToVarSet.emplace(t_procIdx, tempVSet);
    return;
  }
  for (auto& iItr : vItr->second) {
    if (iItr == t_varIdx) {
      return;
    }
  }
  vItr->second.insert(t_varIdx);
}

void UsesP::insertUsesWithVarAsKey(VAR_INDEX t_varIdx, const PROC_NAME& t_procName) {
  auto vItr = m_varToProcNames.find(t_varIdx);
  if (vItr == m_varToProcNames.end()) {
    std::vector<PROC_NAME> tempV = { t_procName };
    m_varToProcNames.emplace(t_varIdx, tempV);
    return;
  }
  for (auto& pItr : vItr->second) {
    if (pItr == t_procName) {
      return;
    }
  }
  vItr->second.push_back(t_procName);
}

void UsesP::insertUsesProcName(const PROC_NAME& t_name) {
  for (auto& mItr : m_procNames) {
    if (mItr == t_name) {
      return;
    }
  }
  m_procNames.push_back(t_name);
}

void UsesP::insertUsesProcAndVarName(const PROC_NAME& t_procName, const VAR_NAME& t_varName) {
  auto aItr = m_allProcNamesToVarNames.equal_range(t_procName);
  for (auto it = aItr.first; it != aItr.second; ++it) {
    if (it->second == t_varName) {
      return;
    }
  }
  m_allProcNamesToVarNames.emplace(t_procName, t_varName);
}

bool UsesP::isUsesP(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx) {
  auto pItr = m_procToVarSet.find(t_procIdx);
  if (pItr == m_procToVarSet.end()) {
    return false;
  }
  auto vItr = pItr->second.find(t_varIdx);
  return vItr != pItr->second.end();
}

bool UsesP::isUsesInProc(PROC_INDEX t_idx) {
  return m_procToVarNames.find(t_idx) != m_procToVarNames.end();
}

LIST_OF_VAR_NAMES UsesP::getVarNamesWithProcIdx(PROC_INDEX t_idx) { /*< Modifies("First", x) */
  auto pItr = m_procToVarNames.find(t_idx);
  if (pItr == m_procToVarNames.end()) {
    return LIST_OF_VAR_NAMES();
  }
  return pItr->second;
}

LIST_OF_PROC_NAMES UsesP::getProcNamesWithVarIdx(VAR_INDEX t_idx) { /*< Modifies(p, "x") */
  auto vItr = m_varToProcNames.find(t_idx);
  if (vItr == m_varToProcNames.end()) {
    return LIST_OF_PROC_NAMES();
  }
  return m_varToProcNames.find(t_idx)->second;
}

MAP_OF_PROC_TO_VAR& UsesP::getAllProcToVar() { /*< Modifies(p, x) */
  return m_allProcNamesToVarNames;
}
LIST_OF_PROC_NAMES& UsesP::getAllProcNames() { /*< Modifies(p, _) */
  return m_procNames;
}