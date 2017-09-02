#include <assert.h>

#include "TNode.h"

/********** Setter Functions **********/

/*  Sets given TNode as this particular TNode's parent
    This TNode should not already have a parent TNode
*/
void TNode::setParent(TNode * t_tNode) {
  assert(m_parent == nullptr); // If false, node already has a parent
  m_parent = t_tNode;
}

/*  Adds given TNode as this particular TNode's children list
    This TNode should be able to contain children 
    (i.e. Variable and Constant nodes are not supposed to have children nodes)
*/
void TNode::addChild(TNode * t_tNode) {
  // m_children == nullptr means there should be no children for this node
  assert(m_children != nullptr);

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









