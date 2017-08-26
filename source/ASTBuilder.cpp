#include <string>

#include "ASTBuilder.h"

/* Creates AST and returns pointer to it
*/
AST *createAST() {
  AST *ast = new AST();
  return ast;
}

/********** Create Singular Abstraction **********/

TNode *createConstant(int t_lineNum, int t_constNum) {

  return &TNode();
}

TNode *createVariable(int t_lineNum, std::string t_varName) {
  VariableNode *varNode = new VariableNode(t_lineNum, t_varName);
  return varNode;
}

/********** Build Abstraction Subtree **********/

TNode *buildAssign() {
  return &TNode();
}

TNode *buildAdd() {
  return &TNode();
}