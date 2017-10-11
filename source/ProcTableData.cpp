#pragma once
#include "ProcTableData.h"

ProcedureNode* ProcTableData::getProcNode() {
  return m_rootOfProcNode;
}

void ProcTableData::insertModifiesVariable(std::string& t_modifiesVar) {
  m_modifiesVariablesSet.insert(t_modifiesVar);
}

void ProcTableData::insertUsesVariable(std::string& t_usesVar) {
  m_usesVariablesSet.insert(t_usesVar);
}

bool ProcTableData::isModifies(std::string& t_modifiesVar) {
  auto mItr = m_modifiesVariablesSet.find(t_modifiesVar);
  return mItr != m_modifiesVariablesSet.end();
}

std::list<std::string>& ProcTableData::getModifiesVar() {
  return m_modifiesVariables;
}

std::list<std::string>& ProcTableData::getUsesVar() {
  return m_usesVariables;
}

bool ProcTableData::isUses(std::string& t_usesVar) {
  auto uItr = m_usesVariablesSet.find(t_usesVar);
  return uItr != m_usesVariablesSet.end();
}

void ProcTableData::convertAnswersInSetToList() {
  auto mItr = m_modifiesVariablesSet.begin();
  for (; mItr != m_modifiesVariablesSet.end(); ++mItr) {
    m_modifiesVariables.push_back(*mItr);
  }
  auto uItr = m_usesVariablesSet.begin();
  for (; uItr != m_usesVariablesSet.end(); ++uItr) {
    m_usesVariables.push_back(*uItr);
  }
}