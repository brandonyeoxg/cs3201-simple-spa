#pragma once

#include "TwoChildrenNode.h"

class MinusNode : public TwoChildrenNode {
public:
  MinusNode(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
};