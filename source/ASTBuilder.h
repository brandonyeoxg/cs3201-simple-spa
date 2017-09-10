#pragma once

#include "ASTBuilderAPI.h"

/** Builder class that implements ASTBuilderAPI to build AST
*   Refer to ASTBuilderAPI on how to use functions to build nodes in AST
*   @author jazlyn
*/

class ASTBuilder: public ASTBuilderAPI {
public:
  ASTBuilder();
  AST *createAST();

  ConstantNode *createConstant(int t_lineNum, int t_constantValue);
  VariableNode *createVariable(int t_lineNum, std::string t_varName);
  ProcedureNode *createProcedure(std::string t_procName);
  StmtListNode *createStmtList(int t_lineNum);

  AssignNode *buildAssignment(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
  WhileNode *buildWhile(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
  PlusNode *buildAddition(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
  MinusNode *buildSubtraction(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
  MultiplyNode *buildMultiplication(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);
  DivideNode *buildDivision(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode);

  void linkParentToChild(TNode *t_parent, TNode *t_child);
};