#include "stdafx.h"
#include "CppUnitTest.h"
#include "TNode.h"
#include "VariableNode.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
  TEST_CLASS(TestAST)
  {
  public:

    TEST_METHOD(TestMethod1)
    {
      TNode T;
      // TODO: Your test code here
    }

    TEST_METHOD(TestTNodeConstructor) {
      Logger::WriteMessage("Running TNode constructor test");

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

  };
}