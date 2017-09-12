#pragma once

#include "TwoChildrenNode.h"

class MultiplyNode : public TwoChildrenNode {
public:
  MultiplyNode(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
};