#include "TNode.h"

TNode::TNode() {} //default empty constructor

/********** Setter Functions **********/

void TNode::setParent(TNode * t_tNode) {
  m_parent = t_tNode;
}

void TNode::addChild(TNode * t_tNode) {
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





