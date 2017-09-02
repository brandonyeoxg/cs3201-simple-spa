#include <assert.h>

#include "AssignNode.h"

AssignNode::AssignNode(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) {
  assert(t_leftNode->getType() == TNode::Type::Variable);

  m_type = TNode::Type::Assign;
  m_lineNum = t_lineNum;
  m_children = new std::vector<TNode *>();
  m_parent = nullptr;

  addChild(t_leftNode);
  addChild(t_rightNode);
}

