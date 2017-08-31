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

  /*  Test ConstantNode constructor and methods
  */
  TEST_METHOD(TestConstantNode) {
    Logger::WriteMessage("Running Constant Node test");

    int lineNum = 5;
    int value = 100;
    TNode *tNode = new ConstantNode(lineNum, value);
    isEqualType(tNode, TNode::Type::Constant);
    isEqualLineNum(tNode, lineNum);
    Assert::IsTrue(tNode->getChildren() == nullptr);

    ConstantNode *constNode = (ConstantNode *) tNode;
    Assert::IsTrue(constNode->getValue() == value);
  }

  /*  Test VariableNode constructor and methods
  */
  TEST_METHOD(TestVariableNode) {
    Logger::WriteMessage("Running Variable Node test");

    int lineNum = 5;
    std::string varName = "penguin";
    TNode *tNode = new VariableNode(lineNum, varName);
    isEqualType(tNode, TNode::Type::Variable);
    isEqualLineNum(tNode, lineNum);

    VariableNode *varNode = (VariableNode *) tNode;
    Assert::IsTrue(varNode->getVarName() == varName);
  }

  /*  Test WhileNode constructor and methods
  */
  TEST_METHOD(TestWhileNode) {
    Logger::WriteMessage("Running While Node test");

    // while i { empty stmtList }
    int lineNum = 5;
    std::string varName = "i";
    TNode *varNode = new VariableNode(lineNum, varName);

    TNode *slNode = new StmtListNode();

    WhileNode *whileNode = new WhileNode(lineNum, varNode, slNode);
    isEqualNode(whileNode->getLeftChild(), varNode);
    isEqualNode(whileNode->getRightChild(), slNode);
  }

  /*  Test ASTBuilder
  */
  TEST_METHOD(TestASTBuilder) {
    Logger::WriteMessage("Running ASTBuilder test");
    ASTBuilderAPI *builder = new ASTBuilder();
    AST *ast = builder->createAST();
    isEqualType(ast->getRoot(), TNode::Type::Procedure);  // Check root initialized properly
    isEqualLineNum(ast->getRoot(), TNode::NO_LINE_NUM);

    TNode *slNode = builder->createStmtList();
    // Check parent and child linked properly
    builder->linkParentToChild(ast->getRoot(), slNode);
    isEqualNode(ast->getRoot()->getChildren()->at(0), slNode);
    isEqualNode(ast->getRoot(), slNode->getParent());

    // Build assignment statement: x = 5;
    int lineNum1 = 2;
    std::string varName = "x";
    TNode *varNode = builder->createVariable(lineNum1, varName);

    int value = 5;
    TNode *constNode = builder->createConstant(lineNum1, value);

    TNode *assignNode = builder->buildAssignment(lineNum1, varNode, constNode);

    // Check parent and child linked properly
    builder->linkParentToChild(slNode, assignNode);
    isEqualNode(slNode, assignNode->getParent());
    isEqualNode(slNode->getChildren()->at(0), assignNode);

    AssignNode *assignNodeCopy = (AssignNode *)assignNode;
    isEqualNode(assignNodeCopy->getLeftChild(), varNode);
    isEqualNode(assignNodeCopy->getRightChild(), constNode);
  }

private:

  /*  Given a TNode, checks if its type is equal to given type
  */
  void isEqualType(TNode * t_tNode, TNode::Type t_type) {
    Assert::IsTrue(t_tNode->getType() == t_type);
  }

  /*  Given a TNode, checks if its line number is equal to given line number
  */
  void isEqualLineNum(TNode * t_tNode, int t_lineNum) {
    Assert::IsTrue(t_tNode->getLineNum() == t_lineNum);
  }

  /*  Given 2 TNode pointers, checks if they are both pointing to the same TNode
  */
  void isEqualNode(TNode * t_tNode1, TNode * t_tNode2) {
    Assert::IsTrue(t_tNode1 == t_tNode2);
  }

  };
}