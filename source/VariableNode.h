#pragma once

#include "TNode.h"

class VariableNode: public TNode {
public:
  VariableNode();
  VariableNode(int lineNum, std::string varName);
  std::string getVarName();

private:
  std::string m_varName;
};