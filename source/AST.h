#pragma once

#include "nodes/ProgramNode.h"

/* Abstract Syntax Tree class
*/

class AST {
public:
  AST();
  ProgramNode *getRoot();

private:
  ProgramNode *m_rootNode;
};

