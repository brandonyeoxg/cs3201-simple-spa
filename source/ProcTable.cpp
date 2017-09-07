#include "ProcTable.h"

PROC_INDEX_NO ProcTable::insertProcByProcNode(ProcedureNode* t_node) {
  PROC_INDEX_NO idxNum = m_data.size();
  m_data.push_back(std::pair<std::string, ProcedureNode*>(t_node->getName(), t_node));
  return idxNum;
}

ProcedureNode* ProcTable::getProcWithIndex(PROC_INDEX_NO t_index) {
  if (m_data.size() == 0 || t_index >= m_data.size()) {
    return NULL;
  }
  return (*(m_data.begin() + t_index)).second;
}
