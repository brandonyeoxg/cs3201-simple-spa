#pragma once

#include "ASTBuilderAPI.h"

#include "AST.h"
#include "TNode.h"
#include "VariableNode.h"
#include "ConstantNode.h"
#include "StmtListNode.h"
#include "ProcedureNode.h"
#include "AssignNode.h"

/* Builds AST
*/

class ASTBuilder: public ASTBuilderAPI {
public:
  ASTBuilder();
  AST *createAST();
  TNode *createConstant(int t_lineNum, int t_constantValue);
  TNode *createVariable(int t_lineNum, std::string t_varName);
  TNode *createProcedure(std::string t_procName);
  TNode *createStmtList();
  TNode *buildAssignment(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
  TNode *buildWhile();
  TNode *buildAddition();
  TNode *buildSubtraction();

  void linkParentToChild(TNode *t_parent, TNode *t_child);
};