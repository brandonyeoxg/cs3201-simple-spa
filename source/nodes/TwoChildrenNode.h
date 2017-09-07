#pragma once

#include <assert.h>

#include "TNode.h"

/** Extends TNode class to only have 2 children for nodes like Assign, Plus, Minus
*   @author jazlyn
*/
class TwoChildrenNode : public TNode {
public:
  TNode *getLeftChild() { return m_children->at(LEFT_CHILD_INDEX);};
  TNode *getRightChild() { return m_children->at(RIGHT_CHILD_INDEX);};

  /** Overrides TNode addChild() to ensure only 2 children are added.
  *   @param t_tNode - child to be added
  */
  void addChild(TNode *t_tNode) {
    assert(m_children->size() < 2);
    TNode::addChild(t_tNode); // Call parent function to add
  };

private:
  const int LEFT_CHILD_INDEX = 0;
  const int RIGHT_CHILD_INDEX = 1;
};