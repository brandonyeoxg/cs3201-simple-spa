#pragma once

#include "TNode.h"

class ProcedureNode: public TNode {
public:
  ProcedureNode(std::string t_procName);
  std::string getName();

private:
  std::string m_procName;
};