#pragma once

#include "TwoChildrenNode.h"

class PlusNode : public TwoChildrenNode {
public:
  PlusNode(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
};