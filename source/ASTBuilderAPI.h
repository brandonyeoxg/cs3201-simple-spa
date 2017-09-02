#pragma once

#include "AST.h"
#include "nodes\TNode.h"
#include "nodes\VariableNode.h"
#include "nodes\ConstantNode.h"
#include "nodes\StmtListNode.h"
#include "nodes\ProcedureNode.h"
#include "nodes\AssignNode.h"
#include "nodes\WhileNode.h"
#include "nodes\PlusNode.h"
#include "nodes\MinusNode.h"

/* API for building AST
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

  /* Create Assignment statement, returns pointer to AssignNode */
  virtual AssignNode *buildAssignment(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;

  /* Create While statement, returns pointer to WhileNode */
  virtual WhileNode *buildWhile(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;

  /* Create Addition expression, returns pointer to PlusNode */
  virtual PlusNode *buildAddition(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;

  /* Create Subtraction expression, returns pointer to MinusNode */
  virtual MinusNode *buildSubtraction(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;


  ///////////////////////////////////////////////////////
  //  Linking Nodes in AST 
  ///////////////////////////////////////////////////////

  /*  Given parent and child nodes, links the 2 nodes together */
  virtual void linkParentToChild(TNode *t_parent, TNode *t_child) = 0;
};