#pragma once

#include "TNode.h"
#include "ProcedureNode.h"

class AST {
public:
  AST();
  TNode *getRoot();

private:
  TNode *m_rootNode;
};

