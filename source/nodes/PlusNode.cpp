#include "PlusNode.h"

PlusNode::PlusNode(int t_lineNum, TNode * t_leftNode, TNode * t_rightNode) {
  m_type = TNode::Type::Plus;
  m_lineNum = t_lineNum;
  m_children = new std::vector<TNode *>();
  m_parent = nullptr;

  addChild(t_leftNode);
  addChild(t_rightNode);
}
