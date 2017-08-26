#include <string>

#include "ASTBuilder.h"

ASTBuilder::ASTBuilder() {}

/* Creates AST and returns pointer to it
*/
AST *ASTBuilder::createAST() {
  AST *ast = new AST();
  return ast;
}

/********** Create Singular Abstraction **********/

TNode *ASTBuilder::createConstant(int t_lineNum, int t_constantValue) {
  ConstantNode *constNode = new ConstantNode(t_lineNum, t_constantValue);
  return constNode;
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

void ASTBuilder::linkParentToChild(TNode * t_parent, TNode * t_child) {
  t_parent->addChild(t_child);
  t_child->setParent(t_parent);
}