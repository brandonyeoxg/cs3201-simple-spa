#include "AssignTable.h"
#include "nodes\VariableNode.h"

VAR_INDEX AssignTable::insertAssignRelation(const VAR_INDEX& t_index, AssignNode* t_node) {
  // Check of the variable has already been added
  std::unordered_map<VAR_INDEX, std::list<AssignData>>::iterator itr = m_data.find(t_index);
  if (itr != m_data.end()) {
    // Append
    AssignData assignData(t_node, (STMT_NUM)t_node->getLineNum());
    (*itr).second.push_back(assignData);
    return t_index;
  }
  AssignData assignNodeData(t_node, (STMT_NUM)t_node->getLineNum());
  std::list<AssignData> assignList;
  assignList.push_back(assignNodeData);
  m_data.insert({ t_index, assignList });
  return t_index;
}

std::list<STMT_NUM> AssignTable::getAllStmtListByVar(VAR_INDEX t_index) {
  std::list<STMT_NUM> output;
  std::unordered_map<VAR_INDEX, std::list<AssignData>>::iterator itr = m_data.find(t_index);
  if (itr == m_data.end()) {
    return output;
  }
  std::list<AssignData>::iterator listItr = (*itr).second.begin();
  for (; listItr != (*itr).second.end(); listItr++) {
    output.push_back((*listItr).m_assignStmt);
  }
  return output;
}

std::list<AssignData>& AssignTable::getAssignDataByVar(VAR_INDEX t_index) {
  auto mItr = m_data.find(t_index);
  if (mItr == m_data.end()) {
    return std::list<AssignData>();
  }
  return mItr->second;
}

std::list<STMT_NUM> AssignTable::getAllStmtList() {
  std::list<STMT_NUM> output;
  std::unordered_map<VAR_INDEX, std::list<AssignData>>::iterator itr = m_data.begin();
  for (; itr != m_data.end(); itr++) {
    std::list<AssignData>::iterator listItr = (*itr).second.begin();
    for (; listItr != (*itr).second.end(); listItr++) {
      output.push_back((*listItr).m_assignStmt);
    }
  }
  return output;
}

std::unordered_map<std::string, std::list<STMT_NUM>> AssignTable::getAllAssignStmtWithVar() {
  std::unordered_map<std::string, std::list<STMT_NUM>> output;
  std::unordered_map<VAR_INDEX, std::list<AssignData>>::iterator itr = m_data.begin();
  for (; itr != m_data.end(); itr++) {
    std::list<AssignData> startList = (*itr).second;
    std::list<AssignData>::iterator listItr = startList.begin();
    std::list<STMT_NUM> outputStmtNo;
    std::string varName = ((VariableNode*)((*listItr).m_assignNode->getLeftChild()))->getVarName();
    for (; listItr != startList.end(); listItr++) {
      outputStmtNo.push_back((*listItr).m_assignStmt);
    }
    output.insert({ varName, outputStmtNo });
  }
  return output;
}

std::list<AssignData>  AssignTable::getAssignData() {
  return m_assignMapToVar;
}

void AssignTable::populateAssignToVarMap(VarTable* t_varTable) {
  for (auto& mItr : m_data) {;
    for (auto& aItr : mItr.second) {
      m_assignMapToVar.push_back(aItr);
    }
  }
}