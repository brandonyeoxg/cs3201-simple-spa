#pragma once

#include "TwoChildrenNode.h"

class DivideNode : public TwoChildrenNode {
public:
  DivideNode(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
};