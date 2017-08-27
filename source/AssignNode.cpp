#include "AssignNode.h"

AssignNode::AssignNode(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) {
  m_type = TNode::Type::Assign;
  m_lineNum = t_lineNum;
  m_children = new std::vector<TNode *>();
  m_parent = nullptr;

  m_children->push_back(t_leftNode);
  m_children->push_back(t_rightNode);
}

TNode * AssignNode::getLeftChild() {
  return m_children->at(LEFT_CHILD_INDEX);
}

TNode * AssignNode::getRightChild() {
  return m_children->at(RIGHT_CHILD_INDEX);
}
