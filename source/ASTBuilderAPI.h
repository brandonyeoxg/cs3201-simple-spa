#pragma once

#include "AST.h"
#include "TNode.h"
#include "VariableNode.h"
#include "ConstantNode.h"
#include "StmtListNode.h"
#include "ProcedureNode.h"
#include "AssignNode.h"
#include "WhileNode.h"

/* API for ASTBuilder
*/

class ASTBuilderAPI {
public:

  ///////////////////////////////////////////////////////
  //  Create Singular Abstraction 
  ///////////////////////////////////////////////////////
  // Create singular nodes that can exist on their own

  /* Create AST, create root node for AST, returns pointer to AST */
  virtual AST *createAST() = 0;

  /* Create ConstantNode, returns pointer to node */
  virtual ConstantNode *createConstant(int t_lineNum, int t_constantValue) = 0;

  /* Create VariableNode, returns pointer to node */
  virtual VariableNode *createVariable(int t_lineNum, std::string t_varName) = 0;

  /* Create ProcedureNode, returns pointer to node */
  virtual ProcedureNode *createProcedure(std::string t_procName) = 0;

  /* Create StmtListNode, returns pointer to node */
  virtual StmtListNode *createStmtList() = 0;


  ///////////////////////////////////////////////////////
  //  Build Abstraction Subtree  
  ///////////////////////////////////////////////////////
  // Create node with subtree (i.e. require children to make logical sense)

  virtual AssignNode *buildAssignment(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;
  virtual WhileNode *buildWhile(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;
  virtual TNode *buildAddition() = 0;
  virtual TNode *buildSubtraction() = 0;

  virtual void linkParentToChild(TNode *t_parent, TNode *t_child) = 0;
};