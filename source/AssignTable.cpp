#include "AssignTable.h"
#include "nodes\VariableNode.h"

void AssignTable::insertAssignStmt(STMT_NUM t_stmtNum, VAR_INDEX t_varIdx, VAR_NAME t_varName) {
  m_assignStmts.push_back(t_stmtNum);
  auto vItr = m_assignVarWithAssignStmtNum.find(t_varIdx);
  if (vItr == m_assignVarWithAssignStmtNum.end()) {
    std::list<STMT_NUM> temp;
    temp.push_back(t_stmtNum);
    m_assignVarWithAssignStmtNum.emplace(t_varIdx, temp);
  } else {
    vItr->second.push_back(t_stmtNum);
  }

  auto vNameItr = m_assignVarNameWithAssignStmtNum.find(t_varName);
  if (vNameItr == m_assignVarNameWithAssignStmtNum.end()) {
    LIST_OF_STMT_NUMS temp;
    temp.push_back(t_stmtNum);
    m_assignVarNameWithAssignStmtNum.emplace(t_varName, temp);
  } else {
    vNameItr->second.push_back(t_stmtNum);
  }

  m_assignMapWithVar.emplace(t_stmtNum, t_varName);
}

LIST_OF_STMT_NUMS& AssignTable::getAllAssignStmt() {
  return m_assignStmts;
}

std::list<STMT_NUM> AssignTable::getAllAssignStmtListByVar(VAR_INDEX t_index) {
  auto aItr = m_assignVarWithAssignStmtNum.find(t_index);
  if (aItr == m_assignVarWithAssignStmtNum.end()) {
    return {};
  }
  return aItr->second;
}


std::unordered_map<STMT_NUM, VAR_NAME> AssignTable::getAllAssignStmtWithVar() {
  return m_assignMapWithVar;
}

LIST_OF_STMT_NUMS AssignTable::getAllAssignStmtList() {
  return m_assignStmts;
}

std::unordered_map<VAR_INDEX, std::list<STMT_NUM>>& AssignTable::getAllVarInWithAssignStmtNum() {
  return m_assignVarWithAssignStmtNum;
}

std::unordered_map<STMT_NUM, VAR_NAME>& AssignTable::getAssignMapWithVar() {
  return m_assignMapWithVar;
}

std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS>& AssignTable::getAllAssignVarNameWithStmtNum() {
  return m_assignVarNameWithAssignStmtNum;
}