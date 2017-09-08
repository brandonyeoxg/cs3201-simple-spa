#include "AssignTable.h"
#include "nodes\VariableNode.h"

VAR_INDEX_NO AssignTable::insertAssignRelation(const VAR_INDEX_NO& t_index, AssignNode* t_node) {
  // Check of the variable has already been added
  std::unordered_map<VAR_INDEX_NO, std::list<AssignData>>::iterator itr = m_data.find(t_index);
  if (itr != m_data.end()) {
    // Append
    AssignData assignData(t_node, (STMT_NO)t_node->getLineNum());
    (*itr).second.push_back(assignData);
    return t_index;
  }
  AssignData assignNodeData(t_node, (STMT_NO)t_node->getLineNum());
  std::list<AssignData> assignList;
  assignList.push_back(assignNodeData);
  m_data.insert({ t_index, assignList });
  return t_index;
}

std::list<STMT_NO> AssignTable::getAllStmtListByVar(VAR_INDEX_NO t_index) {
  std::list<STMT_NO> output;
  std::unordered_map<VAR_INDEX_NO, std::list<AssignData>>::iterator itr = m_data.find(t_index);
  if (itr == m_data.end()) {
    return output;
  }
  std::list<AssignData>::iterator listItr = (*itr).second.begin();
  for (; listItr != (*itr).second.end(); listItr++) {
    output.push_back((*listItr).m_assignStmt);
  }
  return output;
}

std::list<STMT_NO> AssignTable::getAllStmtList() {
  std::list<STMT_NO> output;
  std::unordered_map<VAR_INDEX_NO, std::list<AssignData>>::iterator itr = m_data.begin();
  for (; itr != m_data.end(); itr++) {
    std::list<AssignData>::iterator listItr = (*itr).second.begin();
    for (; listItr != (*itr).second.end(); listItr++) {
      output.push_back((*listItr).m_assignStmt);
    }
  }
  return output;
}

std::unordered_map<std::string, std::list<STMT_NO>> AssignTable::getAllAssignStmtWithVar() {
  std::unordered_map<std::string, std::list<STMT_NO>> output;
  std::unordered_map<VAR_INDEX_NO, std::list<AssignData>>::iterator itr = m_data.begin();
  for (; itr != m_data.end(); itr++) {
    std::list<AssignData> startList = (*itr).second;
    std::list<AssignData>::iterator listItr = startList.begin();
    std::list<STMT_NO> outputStmtNo;
    std::string varName = ((VariableNode*)((*listItr).m_assignNode->getLeftChild()))->getVarName();
    for (; listItr != startList.end(); listItr++) {
      outputStmtNo.push_back((*listItr).m_assignStmt);
    }
    output.insert({ varName, outputStmtNo });
  }
  return output;
}