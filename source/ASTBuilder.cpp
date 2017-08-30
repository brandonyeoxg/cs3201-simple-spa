#include <string>

#include "ASTBuilder.h"

ASTBuilder::ASTBuilder() {}

/* Creates an AST and returns pointer to it
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

TNode * ASTBuilder::createProcedure(std::string t_procName) {
  ProcedureNode *procNode = new ProcedureNode(t_procName);
  return procNode;
}

TNode * ASTBuilder::createStmtList() {
  StmtListNode *slNode = new StmtListNode();
  return slNode;
}

/********** Build Abstraction Subtree **********/

TNode * ASTBuilder::buildAssignment(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) {
  AssignNode *assignNode = new AssignNode(t_lineNum, t_leftNode, t_rightNode);
  return assignNode;
}

TNode * ASTBuilder::buildWhile(int t_lineNum, TNode *t_leftNode, TNode *t_rightNode) {
  WhileNode *whileNode = new WhileNode(t_lineNum, t_leftNode, t_rightNode);
  return whileNode;
}

//TODO implement
TNode *ASTBuilder::buildAddition() {
  return nullptr;
}

//TODO implement
TNode * ASTBuilder::buildSubtraction() {
  return nullptr;
}

/********** Helper Methods **********/

/*  Given parent and child nodes, establishes parent-child relationship.
*/
void ASTBuilder::linkParentToChild(TNode * t_parent, TNode * t_child) {
  t_parent->addChild(t_child);
  t_child->setParent(t_parent);
}