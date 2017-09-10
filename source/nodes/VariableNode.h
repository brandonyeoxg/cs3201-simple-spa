#pragma once

#include "TNode.h"

class VariableNode: public TNode {
public:
  VariableNode(int t_lineNum, int t_varIndex);
  int getVarIndex();

private:
  int m_varIndex;
};