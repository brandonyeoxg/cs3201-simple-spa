#include "VariableNode.h"

VariableNode::VariableNode() {
  m_type = TNode::Type::Variable;
  m_lineNum = 0;
  m_varName = "default variable constructed";
  m_children = NULL;
}

VariableNode::VariableNode(int lineNum, std::string varName) {
  m_type = TNode::Type::Variable;
  m_lineNum = lineNum;
  m_varName = varName;
  m_children = NULL;
}

std::string VariableNode::getVarName() {
  return m_varName;
}
