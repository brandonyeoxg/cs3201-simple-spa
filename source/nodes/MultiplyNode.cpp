#include "MultiplyNode.h"

MultiplyNode::MultiplyNode(int t_lineNum, TNode * t_leftNode, TNode * t_rightNode) {
  m_type = TNode::Type::Multiply;
  m_lineNum = t_lineNum;
  m_children = new std::vector<TNode *>();
  m_parent = nullptr;

  addChild(t_leftNode);
  addChild(t_rightNode);
  t_leftNode->setParent(this);
  t_rightNode->setParent(this);
}
