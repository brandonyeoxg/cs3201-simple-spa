#include "AssignTable.h"
#include <assert.h>

void AssignTable::insertAssignStmt(STMT_NUM t_stmtNum, VAR_INDEX t_varIdx, VAR_NAME t_varName) {
  assert(t_stmtNum != 0);
  
  m_assignStmts.push_back(t_stmtNum);
  auto vItr = m_assignVarWithAssignStmtNum.find(t_varIdx);
  if (vItr == m_assignVarWithAssignStmtNum.end()) {
    LIST_OF_STMT_NUMS temp;
    temp.push_back(t_stmtNum);
    m_assignVarWithAssignStmtNum.emplace(t_varIdx, temp);
  } else {
    vItr->second.push_back(t_stmtNum);
  }

  auto vNameItr = m_assignVarNameWithAssignStmtNum.find(t_varName);
  auto vIdxItr = m_assignVarIndexWithAssignStmtNum.find(t_varIdx);
  if (vNameItr == m_assignVarNameWithAssignStmtNum.end() || vIdxItr == m_assignVarIndexWithAssignStmtNum.end()) {
    LIST_OF_STMT_NUMS temp;
    temp.push_back(t_stmtNum);
    m_assignVarNameWithAssignStmtNum.emplace(t_varName, temp);
  } else {
    vNameItr->second.push_back(t_stmtNum);
    vIdxItr->second.push_back(t_stmtNum);
  }

  m_assignMapWithVar.emplace(t_stmtNum, t_varName);
  m_assignMapWithVarIdx.emplace(t_stmtNum, t_varIdx);
}


LIST_OF_STMT_NUMS AssignTable::getAllAssignStmtListByVar(VAR_INDEX t_index) {
  auto aItr = m_assignVarWithAssignStmtNum.find(t_index);
  if (aItr == m_assignVarWithAssignStmtNum.end()) {
    return {};
  }
  return aItr->second;
}

MAP_OF_STMT_NUM_TO_VAR_NAME& AssignTable::getAllAssignStmtWithVar() {
  return m_assignMapWithVar;
}
MAP_OF_STMT_NUM_TO_VAR_INDEX& AssignTable::getAllAssignStmtWithVarByIdx() {
  return m_assignMapWithVarIdx;
}
LIST_OF_STMT_NUMS& AssignTable::getAllAssignStmts() {
  return m_assignStmts;
}

MAP_OF_VAR_INDEX_TO_STMT_NUMS& AssignTable::getAllVarIndexWithAssignStmtNum() {
  return m_assignVarWithAssignStmtNum;
}

MAP_OF_VAR_NAME_TO_STMT_NUMS& AssignTable::getAllAssignVarNameWithStmtNum() {
  return m_assignVarNameWithAssignStmtNum;
}

MAP_OF_VAR_INDEX_TO_STMT_NUMS& AssignTable::getAllAssignVarIndexWithStmtNum() {
  return m_assignVarIndexWithAssignStmtNum;
}