#include "stdafx.h"
#include "CppUnitTest.h"
#include "TNode.h"
#include "VariableNode.h"
#include "ConstantNode.h"
#include "ASTBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestAST) {
public:

  TEST_METHOD(TestConstantNode) {
    Logger::WriteMessage("Running Constant Node test");

    int lineNum = 5;
    int value = 100;
    TNode *tNode = new ConstantNode(lineNum, value);
    Assert::IsTrue(tNode->getType() == TNode::Type::Constant);
    Assert::IsTrue(tNode->getLineNum() == lineNum);
    Assert::IsTrue(tNode->getChildren() == nullptr);

    ConstantNode *constNode = (ConstantNode *) tNode;
    Assert::IsTrue(constNode->getValue() == value);
  }

  TEST_METHOD(TestVariableNode) {
    Logger::WriteMessage("Running Variable Node test");

    int lineNum = 5;
    std::string varName = "penguin";
    TNode *tNode = new VariableNode(lineNum, varName);
    Assert::IsTrue(tNode->getType() == TNode::Type::Variable);
    Assert::IsTrue(tNode->getLineNum() == lineNum);

    VariableNode *varNode = (VariableNode *) tNode;
    Assert::IsTrue(varNode->getVarName() == varName);
  }

  TEST_METHOD(TestWhileNode) {
    Logger::WriteMessage("Running While Node test");

    // while i {}
    int lineNum = 5;
    std::string varName = "i";
    TNode *varNode = new VariableNode(lineNum, varName);

    TNode *slNode = new StmtListNode();

    WhileNode *whileNode = new WhileNode(lineNum, varNode, slNode);
    Assert::IsTrue(whileNode->getLeftChild() == varNode);
    Assert::IsTrue(whileNode->getRightChild() == slNode);
  }

  TEST_METHOD(TestASTBuilder) {
    Logger::WriteMessage("Running ASTBuilder test");
    ASTBuilder *builder = new ASTBuilder();
    AST *ast = builder->createAST();
    Assert::IsTrue(ast->getRoot()->getType() == TNode::Type::Procedure);  // Check root initialized properly

    TNode *slNode = builder->createStmtList();
    // Check parent and child linked properly
    builder->linkParentToChild(ast->getRoot(), slNode);
    Assert::IsTrue(ast->getRoot()->getChildren()->at(0) == slNode);
    Assert::IsTrue(ast->getRoot() == slNode->getParent());

    // Build assignment statement: x = 5;
    int lineNum1 = 2;
    std::string varName = "x";
    TNode *varNode = builder->createVariable(lineNum1, varName);

    int value = 5;
    TNode *constNode = builder->createConstant(lineNum1, value);

    TNode *assignNode = builder->buildAssignment(lineNum1, varNode, constNode);

    // Check parent and child linked properly
    builder->linkParentToChild(slNode, assignNode);
    Assert::IsTrue(slNode == assignNode->getParent());
    Assert::IsTrue(slNode->getChildren()->at(0) == assignNode);

    AssignNode *assignNodeCopy = (AssignNode *)assignNode;
    Assert::IsTrue(assignNodeCopy->getLeftChild() == varNode);
    Assert::IsTrue(assignNodeCopy->getRightChild() == constNode);
  }

  };
}