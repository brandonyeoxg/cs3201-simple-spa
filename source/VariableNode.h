#pragma once

#include "TNode.h"

class VariableNode: public TNode {
public:
  VariableNode(int lineNum, std::string varName);

private:
  std::string m_varName;
};