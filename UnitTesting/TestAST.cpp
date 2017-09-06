#include "stdafx.h"
#include "CppUnitTest.h"
#include "ASTBuilder.h"

/** Unit testing for AST related classes
*   @author jazlyn
*/

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
    assertIsEqualType(tNode, TNode::Type::Constant);
    assertIsEqualLineNum(tNode, lineNum);
    Assert::IsTrue(tNode->getChildren() == nullptr);
    assertIsEqualNode(tNode->getParent(), nullptr);

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
    assertIsEqualType(tNode, TNode::Type::Variable);
    assertIsEqualLineNum(tNode, lineNum);
    assertIsEqualNode(tNode->getParent(), nullptr);

    VariableNode *varNode = (VariableNode *) tNode;
    Assert::IsTrue(varNode->getVarName() == varName);
  }

  /*  Test AssignNode constructor and methods
  */
  TEST_METHOD(TestAssignNode) {
    Logger::WriteMessage("Running Assign Node test");

    // x = y;
    int lineNum = 5;
    std::string varNameX = "x", varNameY = "y";
    TNode *varNodeX = new VariableNode(lineNum, varNameX);
    TNode *varNodeY = new VariableNode(lineNum, varNameY);

    AssignNode *assignNode = new AssignNode(lineNum, varNodeX, varNodeY);
    assertIsEqualNode(assignNode->getLeftChild(), varNodeX);
    assertIsEqualNode(assignNode->getRightChild(), varNodeY);
    assertIsEqualNode(assignNode->getLeftChild()->getParent(), assignNode);
    assertIsEqualNode(assignNode->getRightChild()->getParent(), assignNode);
    assertIsEqualLineNum(assignNode, lineNum);
  }

  /*  Test PlusNode constructor and methods
  */
  TEST_METHOD(TestPlusNode) {
    Logger::WriteMessage("Running Plus Node test");

    // x + y;
    int lineNum = 30;
    std::string varNameX = "x", varNameY = "y";
    TNode *varNodeX = new VariableNode(lineNum, varNameX);
    TNode *varNodeY = new VariableNode(lineNum, varNameY);

    PlusNode *plusNode = new PlusNode(lineNum, varNodeX, varNodeY);
    assertIsEqualLineNum(plusNode, lineNum);
    assertIsEqualNode(plusNode->getLeftChild(), varNodeX);
    assertIsEqualNode(plusNode->getRightChild(), varNodeY);
    assertIsEqualNode(varNodeX->getParent(), plusNode);
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
    assertIsEqualNode(whileNode->getLeftChild(), varNode);
    assertIsEqualNode(whileNode->getRightChild(), slNode);
    assertIsEqualNode(whileNode->getLeftChild()->getParent(), whileNode);
  }

  /*  Test ASTBuilder
  */
  TEST_METHOD(TestASTBuilderAPI) {
    Logger::WriteMessage("Running ASTBuilderAPI test");

    ASTBuilderAPI *builder = new ASTBuilder();
    AST *ast = builder->createAST();
    assertIsEqualType(ast->getRoot(), TNode::Type::Program);  // Check root initialized properly
    assertIsEqualLineNum(ast->getRoot(), TNode::NO_LINE_NUM);

    TNode *slNode = builder->createStmtList();
    // Check parent and child linked properly
    builder->linkParentToChild(ast->getRoot(), slNode);
    assertIsEqualNode(ast->getRoot()->getChildren()->at(0), slNode);
    assertIsEqualNode(ast->getRoot(), slNode->getParent());

    // Build assignment statement: x = 5;
    int lineNum1 = 1;
    std::string varName = "x";
    TNode *varNode = builder->createVariable(lineNum1, varName);

    int value = 5;
    TNode *constNode = builder->createConstant(lineNum1, value);

    TNode *assignNode = builder->buildAssignment(lineNum1, varNode, constNode);

    // Check parent and child linked properly
    builder->linkParentToChild(slNode, assignNode);
    assertIsEqualNode(slNode, assignNode->getParent());
    assertIsEqualNode(slNode->getChildren()->at(0), assignNode);

    // Test child node able to get data members
    AssignNode *assignNodeCopy = (AssignNode *)assignNode;
    assertIsEqualNode(assignNodeCopy->getLeftChild(), varNode);
    assertIsEqualNode(assignNodeCopy->getRightChild(), constNode);
  }

  TEST_METHOD(TestASTBuilderAPI_AssignAndPlus) {
    Logger::WriteMessage("Running ASTBuilderAPI test: x = a + b + 5;");

    ASTBuilderAPI *builder = new ASTBuilder();

    std::string varNameX = "x", varNameA = "a", varNameB = "b";
    int constValue = 5, lineNum = 100;

    // build x = a + b + 5 assignment subtree
    AssignNode *node = builder->buildAssignment(lineNum, builder->createVariable(lineNum, varNameX),
      builder->buildAddition(lineNum,
        builder->buildAddition(lineNum, 
          builder->createVariable(lineNum, varNameA),
          builder->createVariable(lineNum, varNameB)),
        builder->createConstant(lineNum, constValue)
      )
    );

    assertIsEqualType(node->getLeftChild(), TNode::Type::Variable);
    assertIsEqualNode(node->getLeftChild()->getParent(), node);

    assertIsEqualType(node->getRightChild(), TNode::Type::Plus);
    assertIsEqualNode(node->getRightChild()->getParent(), node);
    ConstantNode * constNode = (ConstantNode *)((TwoChildrenNode *)node->getRightChild())->getRightChild();
    assertIsEqualType(constNode, TNode::Type::Constant);
    Assert::IsTrue(constNode->getValue() == constValue);

    assertIsEqualType(((TwoChildrenNode *)node->getRightChild())->getLeftChild(), TNode::Type::Plus);
  }

private:

  /*  Given a TNode, checks if its type is equal to given type
  */
  void assertIsEqualType(TNode * t_tNode, TNode::Type t_type) {
    Assert::IsTrue(t_tNode->getType() == t_type);
  }

  /*  Given a TNode, checks if its line number is equal to given line number
  */
  void assertIsEqualLineNum(TNode * t_tNode, int t_lineNum) {
    Assert::IsTrue(t_tNode->getLineNum() == t_lineNum);
  }

  /*  Given 2 TNode pointers, checks if they are both pointing to the same TNode
  */
  void assertIsEqualNode(TNode * t_tNode1, TNode * t_tNode2) {
    Assert::IsTrue(t_tNode1 == t_tNode2);
  }

  };
}