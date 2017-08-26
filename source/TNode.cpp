#include "TNode.h"

TNode::TNode() {} //default empty constructor

/********** Setter Functions **********/

void TNode::setParent(TNode * tNode) {
  m_parent = tNode;
}

void TNode::addChild(TNode * tNode) {
  m_children->push_back(tNode);
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





