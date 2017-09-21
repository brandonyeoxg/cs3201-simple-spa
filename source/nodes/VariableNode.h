#pragma once

#include "TNode.h"

class VariableNode: public TNode {
public:
  VariableNode(int t_lineNum, std::string t_varName, int t_varIndex);
  std::string getVarName();
  int getVarIndex();

private:
  std::string m_varName;
  int m_varIndex;
};