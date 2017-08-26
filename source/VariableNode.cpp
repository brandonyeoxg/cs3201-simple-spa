#include "VariableNode.h"

VariableNode::VariableNode() {
  m_type = TNode::Type::Variable;
  m_lineNum = 0;
  m_varName = "default variable constructed";
  m_children = nullptr;
  m_parent = nullptr;
}

VariableNode::VariableNode(int t_lineNum, std::string t_varName) {
  m_type = TNode::Type::Variable;
  m_lineNum = t_lineNum;
  m_varName = t_varName;
  m_children = nullptr;
  m_parent = nullptr;
}

std::string VariableNode::getVarName() {
  return m_varName;
}
