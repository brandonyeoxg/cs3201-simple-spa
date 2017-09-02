#pragma once

#include "TNode.h"

class VariableNode: public TNode {
public:
  VariableNode(int t_lineNum, std::string t_varName);
  std::string getVarName();

private:
  std::string m_varName;
};