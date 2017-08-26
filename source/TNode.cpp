#include <assert.h>

#include "TNode.h"

/********** Setter Functions **********/

void TNode::setParent(TNode * t_tNode) {
  assert(m_parent == nullptr); // If false, node already has a parent
  m_parent = t_tNode;
}

void TNode::addChild(TNode * t_tNode) {
  assert(m_children != nullptr); // m_children == nullptr means there should be no children
  m_children->push_back(t_tNode);

}

/********** Getter Functions **********/

int TNode::getLineNum() {
  return m_lineNum;
}

TNode::Type TNode::getType() {
  return m_type;
}

TNode *TNode::getParent() {
  return m_parent;
}

std::vector<TNode*>* TNode::getChildren() {
  return m_children;
}







