#pragma once
#include "TNode.h"

class AST {
public:
  AST();
  ~AST();

private:
  TNode *rootNode;
};

