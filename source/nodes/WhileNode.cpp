#include <assert.h>

#include "WhileNode.h"

WhileNode::WhileNode(int t_lineNum, TNode * t_leftNode, TNode * t_rightNode) {
  assert(t_leftNode->getType() == TNode::Type::Variable);
  assert(t_rightNode->getType() == TNode::Type::StatementList);

  m_type = TNode::Type::While;
  m_lineNum = t_lineNum;
  m_children = new std::vector<TNode *>();
  m_parent = nullptr;

  addChild(t_leftNode);
  addChild(t_rightNode);
  t_leftNode->setParent(this);
  t_rightNode->setParent(this);
}

TNode * WhileNode::getLeftChild() {
  return m_children->at(LEFT_CHILD_INDEX);
}

TNode * WhileNode::getRightChild() {
  return m_children->at(RIGHT_CHILD_INDEX);
}

