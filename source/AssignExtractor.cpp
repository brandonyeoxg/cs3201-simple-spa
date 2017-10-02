#include "AssignExtractor.h"
#include "nodes\VariableNode.h"
void AssignExtractor::extractDesign() {
  AssignTable* m_assignTable = m_pkb->getAssignTable();
  for (auto& mItr : m_assignTable->getAssignMap()) {
    ;
    for (auto& aItr : mItr.second) {
      VAR_NAME varName = ((VariableNode*)aItr.m_assignNode->getLeftChild())->getVarName();
      m_assignTable->getAssignMapToVar().push_back({ aItr });
      m_assignTable->getAssignMapWithVar().emplace(aItr.m_assignStmt, varName);
    }
  }

  std::unordered_map<VAR_INDEX, std::list<AssignData>>::iterator itr = m_assignTable->getAssignMap().begin();
  for (; itr != m_assignTable->getAssignMap().end(); itr++) {
    std::list<AssignData> startList = (*itr).second;
    std::list<AssignData>::iterator listItr = startList.begin();
    std::list<STMT_NUM> outputStmtNo;
    std::string varName = ((VariableNode*)((*listItr).m_assignNode->getLeftChild()))->getVarName();
    for (; listItr != startList.end(); listItr++) {
      outputStmtNo.push_back((*listItr).m_assignStmt);
    }
    m_assignTable->getAssignVarWithStmtNum().insert({ varName, outputStmtNo });
  }
}
