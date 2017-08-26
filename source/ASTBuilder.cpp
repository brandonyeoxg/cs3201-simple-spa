#include <string>

#include "ASTBuilder.h"

/* Creates root TNode of AST and returns pointer
*/
TNode *createAST() {
  return &TNode();
}

/********** Create Singular Abstraction **********/

TNode *createConstant(int lineNum, int constNum) {

  return &TNode();
}

TNode *createVariable(int lineNum, std::string varName) {
  VariableNode *varNode = new VariableNode(lineNum, varName);
  return varNode;
}

/********** Build Abstraction Subtree **********/

TNode *buildAssign() {
  return &TNode();
}

TNode *buildAdd() {
  return &TNode();
}