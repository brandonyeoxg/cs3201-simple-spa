#include "ProcedureNode.h"

ProcedureNode::ProcedureNode(std::string t_procName) {
  m_type = TNode::Type::Procedure;
  m_lineNum = NO_LINE_NUM;
  m_parent = nullptr;
  m_children = new std::vector<TNode *>();
  m_procName = t_procName;
}

std::string ProcedureNode::getName() {
  return m_procName;
}


