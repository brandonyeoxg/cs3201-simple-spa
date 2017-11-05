#include "ModifiesP.h"

void ModifiesP::insertModifiesP(PROC_INDEX t_procIdx, PROC_NAME t_procName, VAR_INDEX t_varIdx, VAR_NAME t_varName) {
  insertModifiesWithProcAsKey(t_procIdx, t_varName, t_varIdx);
  insertModifiesWithProcAsKeyWithVarHash(t_procIdx, t_varIdx);
  insertModifiesWithVarAsKey(t_varIdx, t_procName, t_procIdx);
  insertModifiesProcName(t_procName, t_procIdx);
  insertModifiesProcAndVarName(t_procName, t_varName);
  insertModifiesProcAndVarIndices(t_procIdx, t_varIdx);
}

void ModifiesP::insertModifiesWithProcAsKey(PROC_INDEX t_procIdx, const VAR_NAME& t_varName, VAR_INDEX t_varIdx) {
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

void ModifiesP::insertModifiesWithProcAsKeyWithVarHash(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx) {
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

void ModifiesP::insertModifiesWithVarAsKey(VAR_INDEX t_varIdx, const PROC_NAME& t_procName, PROC_INDEX t_procIdx) {
  auto vItr = m_varToProcNames.find(t_varIdx);
  auto iterator = m_varToProcIndices.find(t_varIdx);
  if (vItr == m_varToProcNames.end() || iterator == m_varToProcIndices.end()) {
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

void ModifiesP::insertModifiesProcName(const PROC_NAME& t_name, PROC_INDEX t_procIdx) {
  for (auto& mItr : m_procNames) {
    if (mItr == t_name) {
      return;
    }
  }
  m_procNames.push_back(t_name);
  m_procIndices.push_back(t_procIdx);
}

void ModifiesP::insertModifiesProcAndVarName(const PROC_NAME& t_procName, const VAR_NAME& t_varName) {
  auto aItr = m_allProcNamesToVarNames.equal_range(t_procName);
  for (auto it = aItr.first; it != aItr.second; ++it) {
    if (it->second == t_varName) {
      return;
    }
  }
  m_allProcNamesToVarNames.emplace(t_procName, t_varName);
}

void ModifiesP::insertModifiesProcAndVarIndices(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx) {
  auto itr = m_allProcIndicesToVarIndices.equal_range(t_procIdx);
  for (auto it = itr.first; it != itr.second; ++it) {
    if (it->second == t_varIdx) {
      return;
    }
  }
  m_allProcIndicesToVarIndices.emplace(t_procIdx, t_varIdx);
}

bool ModifiesP::isModifiesP(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx) {
  auto pItr = m_procToVarSet.find(t_procIdx);
  if (pItr == m_procToVarSet.end()) {
    return false;
  }
  auto vItr = pItr->second.find(t_varIdx);
  return vItr != pItr->second.end();
}

bool ModifiesP::isModifiesInProc(PROC_INDEX t_idx) {
  return m_procToVarNames.find(t_idx) != m_procToVarNames.end();
}

LIST_OF_VAR_NAMES ModifiesP::getVarNamesWithProcIdx(PROC_INDEX t_idx) {
  auto pItr = m_procToVarNames.find(t_idx);
  if (pItr == m_procToVarNames.end()) {
    return LIST_OF_VAR_NAMES();
  }
  return pItr->second;
}
LIST_OF_VAR_INDICES ModifiesP::getVarIndicesWithProcIdx(PROC_INDEX t_idx) {
  auto pItr = m_procToVarIndices.find(t_idx);
  if (pItr == m_procToVarIndices.end()) {
    return LIST_OF_VAR_INDICES();
  }
  return pItr->second;
}

LIST_OF_PROC_NAMES ModifiesP::getProcNamesWithVarIdx(VAR_INDEX t_idx) {
  auto vItr = m_varToProcNames.find(t_idx);
  if (vItr == m_varToProcNames.end()) {
    return LIST_OF_PROC_NAMES();
  }
  return m_varToProcNames.find(t_idx)->second;
}
LIST_OF_PROC_INDICES ModifiesP::getProcIndicesWithVarIdx(VAR_INDEX t_idx) {
  auto vItr = m_varToProcIndices.find(t_idx);
  if (vItr == m_varToProcIndices.end()) {
    return LIST_OF_PROC_INDICES();
  }
  return m_varToProcIndices.find(t_idx)->second;
}

MAP_OF_PROC_TO_VAR ModifiesP::getAllProcToVar() {
  return m_allProcNamesToVarNames;
}
MAP_OF_PROC_INDEX_TO_VAR_INDEX& ModifiesP::getAllProcToVarByIdx() {
  return m_allProcIndicesToVarIndices;
}

LIST_OF_PROC_NAMES ModifiesP::getAllProcNames() {
  return m_procNames;
}
LIST_OF_PROC_INDICES& ModifiesP::getAllProcIndices() {
  return m_procIndices;
}