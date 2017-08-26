#pragma once

#include "AST.h"
#include "TNode.h"
#include "VariableNode.h"

/* Builds AST
*/

class ASTBuilder {
public:
  ASTBuilder();
  AST *createAST();
  TNode *createConstant(int t_lineNum, int t_constNum);
  TNode *createVariable(int t_lineNum, std::string t_varName);
  TNode *buildAssign();
  TNode *buildAdd();
};