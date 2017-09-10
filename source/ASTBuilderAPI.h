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
#include "nodes\MultiplyNode.h"

/** API for building AST
*   @author jazlyn
*/

class ASTBuilderAPI {
public:

  ///////////////////////////////////////////////////////
  //  Create Singular Abstraction 
  ///////////////////////////////////////////////////////
  // Create singular nodes that can exist on their own

  /** Create AST, create root node for AST
  *   @return pointer to AST 
  */
  virtual AST *createAST() = 0;

  /** Create ConstantNode
  *   @param t_lineNum statement number
  *   @param t_constantValue value of constant
  *   @return pointer to ConstantNode
  */
  virtual ConstantNode *createConstant(int t_lineNum, int t_constantValue) = 0;

  /** Create VariableNode
  *   @param t_lineNum statement number
  *   @param t_varName variable name
  *   @return pointer to VariableNode
  */
  virtual VariableNode *createVariable(int t_lineNum, std::string t_varName) = 0;

  /** Create ProcedureNode
  *   @param t_lineNum statement number
  *   @param t_procName procedure name
  *   @return pointer to ProcedureNode
  */
  virtual ProcedureNode *createProcedure(std::string t_procName) = 0;

  /** Create StmtListNode
  *   @return pointer to StmtListNode
  */
  virtual StmtListNode *createStmtList(int t_lineNum) = 0;


  ///////////////////////////////////////////////////////
  //  Build Abstraction Subtree  
  ///////////////////////////////////////////////////////
  //  Create node with subtree (i.e. require children to make logical sense)
  //  Note: all parent-child relationships within subtree will be done by API
  //  i.e. children of AssignNode will know AssignNode is parent and vice versa.

  /** Create Assignment statement
  *   @param t_lineNum statement number
  *   @param t_leftNode left child (should be variable)
  *   @param t_rightNode right child (should be expression/value)
  *   @return pointer to AssignNode
  */
  virtual AssignNode *buildAssignment(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;

  /** Create While statement
  *   @param t_lineNum statement number
  *   @param t_leftNode left child (should be variable)
  *   @param t_rightNode right child (should be statement list)
  *   @return pointer to WhileNode
  */
  virtual WhileNode *buildWhile(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;

  /** Create Addition expression
  *   @param t_lineNum statement number
  *   @param t_leftNode left child (should be expression/value)
  *   @param t_rightNode right child (should be expression/value)
  *   @return pointer to PlusNode
  */
  virtual PlusNode *buildAddition(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;

  /** Create Subtraction expression
  *   @param t_lineNum statement number
  *   @param t_leftNode left child (should be expression/value)
  *   @param t_rightNode right child (should be expression/value)
  *   @return pointer to MinusNode
  */
  virtual MinusNode *buildSubtraction(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;

  /** Create Multiplication expression
  *   @param t_lineNum statement number
  *   @param t_leftNode left child (should be expression/value)
  *   @param t_rightNode right child (should be expression/value)
  *   @return pointer to MultiplyNode
  */
  virtual MultiplyNode *buildMultiplication(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;


  ///////////////////////////////////////////////////////
  //  Linking Nodes in AST 
  ///////////////////////////////////////////////////////

  /** Links 2 nodes together such that both nodes are aware of parent-child relationship
  *   @param t_parent parent node
  *   @param t_child child node
  */
  virtual void linkParentToChild(TNode *t_parent, TNode *t_child) = 0;
};