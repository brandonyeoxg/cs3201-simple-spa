#pragma once

#include "TNode.h"

class WhileNode : public TNode {
public:
  WhileNode(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
  TNode *getLeftChild();
  TNode *getRightChild();

private:
  const int LEFT_CHILD_INDEX = 0;
  const int RIGHT_CHILD_INDEX = 1;
};