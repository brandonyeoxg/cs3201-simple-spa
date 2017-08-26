#include <assert.h>
#include <iostream>

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

void TNode::printDataMembers() {
  std::cout << "Type: " << m_type << std::endl;
  std::cout << "Line: " << m_lineNum << std::endl;
  std::cout << "Parent: " << std::endl;
  m_parent->printDataMembers();
  std::cout << std::endl;
  std::cout << "Children: " << std::endl;
  for (int i = 0; i < m_children->size(); i++) {
    m_children->at(i)->printDataMembers();
    std::cout << std::endl;
  }
}







