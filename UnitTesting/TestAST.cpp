#include "stdafx.h"
#include "CppUnitTest.h"
#include "TNode.h"
#include "VariableNode.h"
#include "ASTBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestAST) {
public:

  TEST_METHOD(TestMethod1) {
    // TODO: Your test code here
  }

  TEST_METHOD(TestVariableNodeConstructor) {
    Logger::WriteMessage("Running Variable Node constructor test");

    int lineNum = 5;
    std::string varName = "penguin";
    TNode *tNode = new VariableNode(lineNum, varName);
    Assert::IsTrue(tNode->getLineNum() == lineNum);
    Assert::IsTrue(tNode->getType() == TNode::Type::Variable);

    VariableNode *varNode = new VariableNode(lineNum, varName);
    Assert::IsTrue(varNode->getVarName() == varName);

    varNode = new VariableNode();
    Assert::IsFalse(varNode->getLineNum() == lineNum);
  }

  TEST_METHOD(TestASTBuilder) {
    Logger::WriteMessage("Running ASTBuilder test");
    ASTBuilder *builder = new ASTBuilder();
    AST *ast = builder->createAST();
    Assert::IsTrue(ast->getRoot()->getType() == TNode::Type::Procedure);  // Check root initialized properly

    int varLineNum = 2;
    std::string varName = "x";
    TNode *tNode = builder->createVariable(varLineNum, varName);
    builder->linkParentToChild(ast->getRoot(), tNode);
    // Check parent and child linked properly
    Assert::IsTrue(ast->getRoot() == tNode->getParent());
    Assert::IsTrue(ast->getRoot()->getChildren()->at(0) == tNode);
  }

  };
}