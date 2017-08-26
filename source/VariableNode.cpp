#include "VariableNode.h"

VariableNode::VariableNode(int lineNum, std::string varName) {
  m_type = TNode::Type::Variable;
  m_lineNum = lineNum;
  m_varName = varName;
  m_children = NULL;
}
