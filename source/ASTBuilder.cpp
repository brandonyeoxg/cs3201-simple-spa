#include <string>

#include "ASTBuilder.h"

ASTBuilder::ASTBuilder() {}

/* Creates AST and returns pointer to it
*/
AST * ASTBuilder::createAST() {
  AST *ast = new AST();
  return ast;
}

/********** Create Singular Abstraction **********/

TNode *ASTBuilder::createConstant(int t_lineNum, int t_constNum) {

  return &TNode();
}

TNode *ASTBuilder::createVariable(int t_lineNum, std::string t_varName) {
  VariableNode *varNode = new VariableNode(t_lineNum, t_varName);
  return varNode;
}

/********** Build Abstraction Subtree **********/

TNode *ASTBuilder::buildAssign() {
  return nullptr;
}

TNode *ASTBuilder::buildAdd() {
  return nullptr;
}