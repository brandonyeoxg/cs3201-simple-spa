#include "VariableNode.h"

VariableNode::VariableNode(int t_lineNum, int t_varIndex) {
  m_type = TNode::Type::Variable;
  m_lineNum = t_lineNum;
  m_children = nullptr;
  m_parent = nullptr;
  m_varIndex = t_varIndex;
}

int VariableNode::getVarIndex() {
  return m_varIndex;
}


