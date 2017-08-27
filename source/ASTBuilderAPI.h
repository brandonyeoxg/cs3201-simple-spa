#pragma once

#include "AST.h"
#include "TNode.h"

/* API for ASTBuilder
*/

class ASTBuilderAPI {
public:

  virtual AST *createAST() = 0;
  virtual TNode *createConstant(int t_lineNum, int t_constantValue) = 0;
  virtual TNode *createVariable(int t_lineNum, std::string t_varName) = 0;
  virtual TNode *createProcedure(std::string t_procName) = 0;
  virtual TNode *createStmtList() = 0;
  virtual TNode *buildAssignment(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;
  virtual TNode *buildWhile(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) = 0;
  virtual TNode *buildAddition() = 0;
  virtual TNode *buildSubtraction() = 0;

  virtual void linkParentToChild(TNode *t_parent, TNode *t_child) = 0;
};