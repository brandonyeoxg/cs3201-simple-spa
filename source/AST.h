#pragma once

#include "TNode.h"
#include "ProcedureNode.h"

class AST {
public:
  const std::string DEFAULT_PROC_NAME = "main";
  AST();
  TNode *getRoot();

private:
  TNode *m_rootNode;
};

