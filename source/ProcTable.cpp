#include "ProcTable.h"

PROC_INDEX ProcTable::insertProcByProcNode(ProcedureNode* t_node) {
  // Check if the procedure is already included
  auto procItr = m_data.begin();
  while (procItr != m_data.end()) {
    if (procItr->getProcNode()->getName() == t_node->getName()) {
      return procItr - m_data.begin();
    }
    ++procItr;
  }
  // Add a new entry into the table
  ProcTableData procTableData(t_node);
  PROC_INDEX idxNum = m_data.size();
  m_data.push_back(procTableData);
  m_procNames.push_back(t_node->getName());
  return idxNum;
}

ProcedureNode* ProcTable::getProcNodeWithIdx(PROC_INDEX& t_procIdx) {
  auto mItr = m_data.begin() + t_procIdx;
  if (mItr != m_data.end()) {
    return mItr->getProcNode();
  }
  return nullptr;
}

ProcTableData* ProcTable::getProcTableDataWithName(std::string& procName) {
  auto procItr = m_data.begin();
  while (procItr != m_data.end()) {
    if (procItr->getProcNode()->getName() == procName) {
      return &(*procItr);
    }
    procItr++;
  }
  return nullptr;
}

PROC_INDEX ProcTable::getProcIdxNumWithName(std::string& procName) {
  auto procItr = m_data.begin();
  while (procItr != m_data.end()) {
    if (procItr->getProcNode()->getName() == procName) {
      return procItr - m_data.begin();
    }
    procItr++;
  }
  return INVALID_PROC_NO;
}

std::string ProcTable::getProcNameWithIdx(PROC_INDEX& t_procIdx) {
  auto procItr = m_data.begin() + t_procIdx;
  if (procItr == m_data.end()) {
    return NULL;
  }
  return procItr->getProcNode()->getName();
}

std::list<string> ProcTable::getAllProcNameInProgram() {
  return m_procNames;
}

bool ProcTable::insertModifies(PROC_INDEX& t_procIdxNo, std::string& t_varIdx) {
  auto procItr = m_data.begin() + t_procIdxNo;
  if (procItr == m_data.end()) {
    return false;
  }
  procItr->insertModifiesVariable(t_varIdx);
  return true;
}

bool ProcTable::isModifies(std::string& t_procName, std::string& t_varIdx) {
  auto procItr = m_procWithVarNamesModifies.find(t_procName);
  if (procItr == m_procWithVarNamesModifies.end()) {
    return false;
  }
  for (auto& innerItr : procItr->second) {
    if (innerItr == t_varIdx) {
      return true;
    }
  }
  return false;
}

std::list<std::string>& ProcTable::getVarFromProcModifies(PROC_INDEX& t_procIdx) {
  auto pItr = m_data.begin() + t_procIdx;
  if (pItr == m_data.end()) {
    return std::list<std::string>();
  }
  return pItr->getModifiesVar();
}

std::list<std::string>& ProcTable::getProcNameThatModifiesVar(std::string& t_varName) {
  auto vItr = m_varToProcLookUpForModifies.find(t_varName);
  if (vItr == m_varToProcLookUpForModifies.end()) {
    return std::list<std::string>();
  }
  return vItr->second;
}

std::unordered_map<std::string, std::list<std::string>>& ProcTable::getProcAndVarModifies() {
  return m_procWithVarNamesModifies;
}

bool ProcTable::isModifiesInProc(std::string& t_procName) {
  auto pItr = m_procWithVarNamesModifies.find(t_procName);
  if (pItr == m_procWithVarNamesModifies.end()) {
    return false;
  }
  return !pItr->second.empty();
}

std::list<std::string>& ProcTable::getProcNameThatModifies() {
  return m_procThatModifies;
}

bool ProcTable::insertUses(PROC_INDEX& t_procIdxNo, std::string& t_varIdx) {
  auto procItr = m_data.begin() + t_procIdxNo;
  if (procItr == m_data.end()) {
    return false;
  }
  procItr->insertUsesVariable(t_varIdx);
  return true;
}

bool ProcTable::isUses(std::string& t_procName, std::string& t_varIdx) {
  auto procItr = m_procWithVarNamesUses.find(t_procName);
  if (procItr == m_procWithVarNamesUses.end()) {
    return false;
  }
  for (auto& innerItr : procItr->second) {
    if (innerItr == t_varIdx) {
      return true;
    }
  }
  return false;
}

std::list<std::string>& ProcTable::getVarFromProcUses(PROC_INDEX& t_procIdx) {
  auto pItr = m_data.begin();
  if (pItr == m_data.end()) {
    return std::list<std::string>();
  }
  return pItr->getUsesVar();
}

std::list<std::string>& ProcTable::getProcNameThatUsesVar(std::string& t_varName) {
  auto vItr = m_varToProcLookUpForUses.find(t_varName);
  if (vItr == m_varToProcLookUpForUses.end()) {
    return std::list<std::string>();
  }
  return vItr->second;
}

std::unordered_map<std::string, std::list<std::string>>& ProcTable::getProcAndVarUses() {
  return m_procWithVarNamesUses;
}

bool ProcTable::isUsesInProc(std::string& t_procName) {
  auto uItr = m_procWithVarNamesUses.find(t_procName);
  if (uItr == m_procWithVarNamesUses.end()) {
    return false;
  }
  return !uItr->second.empty();
}

std::list<std::string>& ProcTable::getProcNameThatUses() {
  return m_procThatUses;
}

void ProcTable::convertProcTableSetToList() {
  for (auto& dItr : m_data) {
    dItr.convertAnswersInSetToList();
  }

  // Populate the m_varToProcLookUpForModifies;
  for (auto& dItr : m_data) {
    std::list<std::string> modifiesList = dItr.getModifiesVar();
    std::list<std::string> usesList = dItr.getUsesVar();
    std::string procName = dItr.getProcNode()->getName();

    if (!dItr.getModifiesVar().empty()) {
      m_procThatModifies.push_back(dItr.getProcNode()->getName());
    }

    if (!dItr.getUsesVar().empty()) {
      m_procThatUses.push_back(dItr.getProcNode()->getName());
    }

    // Updates the variable to procedure mapping for modifies
    for (auto& mItr : modifiesList) {
      auto varToProcItr = m_varToProcLookUpForModifies.find(mItr);
      if (varToProcItr == m_varToProcLookUpForModifies.end()) {
        m_varToProcLookUpForModifies.insert({ mItr, {procName} });
      } else {
        varToProcItr->second.push_back(procName);
      }
    }

    // Updates the variable to procedure mapping for modifies
    for (auto& uItr : usesList) {
      auto varToProcItr = m_varToProcLookUpForUses.find(uItr);
      if (varToProcItr == m_varToProcLookUpForUses.end()) {
        m_varToProcLookUpForUses.insert({ uItr, {procName} });
      } else {
        varToProcItr->second.push_back(procName);
      }
    }
    m_procWithVarNamesModifies.insert({ procName, dItr.getModifiesVar() });
    m_procWithVarNamesUses.insert({ procName, dItr.getUsesVar() });
  }
}

