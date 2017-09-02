#pragma once

#include "TwoChildrenNode.h"

class AssignNode : public TwoChildrenNode {
public:
  AssignNode(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
};