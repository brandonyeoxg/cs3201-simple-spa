#pragma once

#include "AST.h"
#include "TNode.h"
#include "VariableNode.h"
#include "ConstantNode.h"

/* Builds AST
*/

class ASTBuilder {
public:
  ASTBuilder();
  AST *createAST();
  TNode *createConstant(int t_lineNum, int t_constantValue);
  TNode *createVariable(int t_lineNum, std::string t_varName);
  TNode *buildAssign();
  TNode *buildAdd();
};