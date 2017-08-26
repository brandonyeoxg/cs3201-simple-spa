#include "ProcedureNode.h"

ProcedureNode::ProcedureNode(int t_lineNum) {
  m_type = TNode::Type::Procedure;
  m_lineNum = t_lineNum;
  m_parent = nullptr;
  m_children = new std::vector<TNode *>();
}
