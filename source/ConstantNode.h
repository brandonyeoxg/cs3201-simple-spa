#pragma once

#include "TNode.h"

class ConstantNode: public TNode {
public:
  ConstantNode(int t_lineNum, int t_value);
  int getValue();

private:
  int m_value;
};