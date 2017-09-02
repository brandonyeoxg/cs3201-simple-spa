#pragma once

#include "ASTBuilderAPI.h"

/*  Builder class that implements ASTBuilderAPI to build AST
    Refer to ASTBuilderAPI on how to use functions to build nodes in AST
*/

class ASTBuilder: public ASTBuilderAPI {
public:
  ASTBuilder();
  AST *createAST();
  ConstantNode *createConstant(int t_lineNum, int t_constantValue);
  VariableNode *createVariable(int t_lineNum, std::string t_varName);
  ProcedureNode *createProcedure(std::string t_procName);
  StmtListNode *createStmtList();
  AssignNode *buildAssignment(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
  WhileNode *buildWhile(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
  TNode *buildAddition();
  TNode *buildSubtraction();

  void linkParentToChild(TNode *t_parent, TNode *t_child);
};