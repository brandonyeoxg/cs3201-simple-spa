#pragma once

#include "nodes\ProgramNode.h"

/** Abstract Syntax Tree class
*   @author jazlyn
*/

class AST {
public:
  AST();
  ProgramNode *getRoot();

private:
  ProgramNode *m_rootNode;
};

