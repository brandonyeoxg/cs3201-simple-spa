#include "UsesP.h"

void UsesP::insertUsesP(PROC_INDEX t_procIdx, const PROC_NAME& t_procName, VAR_INDEX t_varIdx, const VAR_NAME& t_varName) {
  insertUsesWithProcAsKey(t_procIdx, t_varName, t_varIdx);
  insertUsesWithProcAsKeyWithVarHash(t_procIdx, t_varIdx);
  insertUsesWithVarAsKey(t_varIdx, t_procName, t_procIdx);
  insertUsesProcName(t_procName, t_procIdx);
  insertUsesProcAndVarName(t_procName, t_varName);
  insertUsesProcAndVarIndices(t_procIdx, t_varIdx);
}

void UsesP::insertUsesWithProcAsKey(PROC_INDEX t_procIdx, const VAR_NAME& t_varName, VAR_INDEX t_varIdx) {
  auto mItr = m_procToVarNames.find(t_procIdx);
  auto iterator = m_procToVarIndices.find(t_procIdx);
  if (mItr == m_procToVarNames.end()) {
    std::vector<VAR_NAME> tempV = { t_varName };
    LIST_OF_VAR_INDICES tempVarIndices = { t_varIdx };
    m_procToVarNames.emplace(t_procIdx, tempV);
    m_procToVarIndices.emplace(t_procIdx, tempVarIndices);
    return;
  }
  for (auto& vItr : mItr->second) {
    if (vItr == t_varName) {
      return;
    }
  }
  mItr->second.push_back(t_varName);
  iterator->second.push_back(t_varIdx);
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

void UsesP::insertUsesWithVarAsKey(VAR_INDEX t_varIdx, const PROC_NAME& t_procName, PROC_INDEX t_procIdx) {
  auto vItr = m_varToProcNames.find(t_varIdx);
  auto iterator = m_varToProcIndices.find(t_varIdx);
  if (vItr == m_varToProcNames.end() && iterator == m_varToProcIndices.end()) {
    std::vector<PROC_NAME> tempV = { t_procName };
    LIST_OF_PROC_INDICES tempProcIndices = { t_procIdx };
    m_varToProcNames.emplace(t_varIdx, tempV);
    m_varToProcIndices.emplace(t_varIdx, tempProcIndices);
    return;
  }
  for (auto& pItr : vItr->second) {
    if (pItr == t_procName) {
      return;
    }
  }
  vItr->second.push_back(t_procName);
  iterator->second.push_back(t_procIdx);
}

void UsesP::insertUsesProcName(const PROC_NAME& t_name, PROC_INDEX t_procIdx) {
  for (auto& mItr : m_procNames) {
    if (mItr == t_name) {
      return;
    }
  }
  m_procNames.push_back(t_name);
  m_procIndices.push_back(t_procIdx);
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

void UsesP::insertUsesProcAndVarIndices(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx) {
  auto itr = m_allProcIndicesToVarIndices.equal_range(t_procIdx);
  for (auto it = itr.first; it != itr.second; ++it) {
    if (it->second == t_varIdx) {
      return;
    }
  }
  m_allProcIndicesToVarIndices.emplace(t_procIdx, t_varIdx);
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

LIST_OF_VAR_INDICES UsesP::getVarIndicesWithProcIdx(PROC_INDEX t_idx) {
  auto pItr = m_procToVarIndices.find(t_idx);
  if (pItr == m_procToVarIndices.end()) {
    return LIST_OF_VAR_INDICES();
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

LIST_OF_PROC_INDICES UsesP::getProcIndicesWithVarIdx(VAR_INDEX t_idx) {
  auto vItr = m_varToProcIndices.find(t_idx);
  if (vItr == m_varToProcIndices.end()) {
    return LIST_OF_PROC_INDICES();
  }
  return m_varToProcIndices.find(t_idx)->second;
}

MAP_OF_PROC_TO_VAR& UsesP::getAllProcToVar() { /*< Modifies(p, x) */
  return m_allProcNamesToVarNames;
}

MAP_OF_PROC_INDEX_TO_VAR_INDEX& UsesP::getAllProcToVarByIdx() {
  return m_allProcIndicesToVarIndices;
}

LIST_OF_PROC_NAMES& UsesP::getAllProcNames() { /*< Modifies(p, _) */
  return m_procNames;
}

LIST_OF_PROC_INDICES& UsesP::getAllProcIndices() {
  return m_procIndices;
}