#include "VariableNode.h"

VariableNode::VariableNode(int t_lineNum, std::string t_varName, int t_varIndex) {
  m_type = TNode::Type::Variable;
  m_lineNum = t_lineNum;
  m_children = nullptr;
  m_parent = nullptr;

  m_varName = t_varName;
  m_varIndex = t_varIndex;
}

std::string VariableNode::getVarName() {
  return m_varName;
}

int VariableNode::getVarIndex() {
  return m_varIndex;
}


