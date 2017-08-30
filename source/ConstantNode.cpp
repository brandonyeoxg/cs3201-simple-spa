#include "ConstantNode.h"

ConstantNode::ConstantNode(int t_lineNum, int t_value) {
  m_type = TNode::Type::Constant;
  m_lineNum = t_lineNum;
  m_children = nullptr;
  m_parent = nullptr;
  m_value = t_value;
}

int ConstantNode::getValue() {
  return m_value;
}
