#include "AssignTable.h"

VAR_INDEX_NO AssignTable::insertAssignRelation(const VAR_INDEX_NO& t_index, AssignNode* t_node) {
  // Check of the variable has already been added
  std::unordered_map<VAR_INDEX_NO, std::list<AssignNode*>>::iterator itr = m_data.find(t_index);
  if (itr != m_data.end()) {
    // Append
    (*itr).second.push_back(t_node);
    return t_index;
  }
  std::list<AssignNode*> nodeList;
  nodeList.push_back(t_node);
  m_data.insert({ t_index, nodeList });

  return t_index;
}

std::list<STMT_NO> AssignTable::getAllStmtListByVar(VAR_INDEX_NO t_index) {
  (*m_data.find(t_index)).second;
}

std::list<STMT_NO> AssignTable::getAllStmtList() {

}

std::unordered_map<VAR_INDEX_NO, std::list<STMT_NO>> AssignTable::getAllAssignStmtWithVar() {

}