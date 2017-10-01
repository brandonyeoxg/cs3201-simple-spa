#include "ModifiesP.h"

void ModifiesP::insertModifiesP(PROC_INDEX t_procIdx, PROC_NAME t_procName, VAR_INDEX t_varIdx, VAR_NAME t_varName) {
  insertModifiesWithProcAsKey(t_procIdx, t_varName);
  insertModifiesWithProcAsKeyWithVarHash(t_procIdx, t_varIdx);
  insertModifiesWithVarAsKey(t_varIdx, t_procName);
  insertModifiesProcName(t_procName);
  insertModifiesProcAndVarName(t_procName, t_varName);
}

void ModifiesP::insertModifiesWithProcAsKey(PROC_INDEX t_procIdx, VAR_NAME t_varName) {
  auto mItr = m_procToVarNames.find(t_procIdx);
  if (mItr == m_procToVarNames.end()) {
    std::vector<VAR_NAME> tempV = { t_varName };
    m_procToVarNames.emplace(t_procIdx, tempV);
    return;
  }
  mItr->second.push_back(t_varName);
}

void ModifiesP::insertModifiesWithProcAsKeyWithVarHash(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx) {
  auto vItr = m_procToVarSet.find(t_procIdx);
  if (vItr == m_procToVarSet.end()) {
    VAR_HASH_SET tempVSet;
    tempVSet.insert(t_varIdx);
    m_procToVarSet.emplace(t_procIdx, tempVSet);
    return;
  }
  vItr->second.insert(t_varIdx);
}

void ModifiesP::insertModifiesWithVarAsKey(VAR_INDEX t_varIdx, PROC_NAME t_procName) {
  auto vItr = m_varToProcNames.find(t_varIdx);
  if (vItr == m_varToProcNames.end()) {
    std::vector<PROC_NAME> tempV = { t_procName };
    m_varToProcNames.emplace(t_varIdx, tempV);
    return;
  }
  vItr->second.push_back(t_procName);
}

void ModifiesP::insertModifiesProcName(PROC_NAME& t_name) {
  for (auto& mItr : m_procNames) {
    if (mItr == t_name) {
      return;
    }
  }
  m_procNames.push_back(t_name);
}

void ModifiesP::insertModifiesProcAndVarName(PROC_NAME t_procName, VAR_NAME t_varName) {
  m_allProcNamesToVarNames.emplace(t_procName, t_varName);
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

LIST_OF_VAR_NAMES ModifiesP::getVarNamesWithProcIdx(PROC_INDEX t_idx) { /*< Modifies("First", x) */
  auto pItr = m_procToVarNames.find(t_idx);
  if (pItr == m_procToVarNames.end()) {
    return LIST_OF_VAR_NAMES();
  }
  return pItr->second;
}

LIST_OF_PROC_NAMES ModifiesP::getProcNamesWithVarIdx(VAR_INDEX t_idx) { /*< Modifies(p, "x") */
  auto vItr = m_varToProcNames.find(t_idx);
  if (vItr == m_varToProcNames.end()) {
    return LIST_OF_PROC_NAMES();
  }
  return m_varToProcNames.find(t_idx)->second;
}

MAP_OF_PROC_TO_VAR& ModifiesP::getAllProcToVar() { /*< Modifies(p, x) */
  return m_allProcNamesToVarNames;
}
LIST_OF_PROC_NAMES& ModifiesP::getAllProcNames() { /*< Modifies(p, _) */
  return m_procNames;
}