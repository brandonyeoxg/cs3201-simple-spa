#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryPreProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

  namespace UnitTesting {
    TEST_CLASS(TestQueryProcessor) {
  public:
    std::string testInput = "assign a, b; stmt s; Select s such that Follows(s, a)";
    TEST_METHOD(TestSplittingDeclaration) {
      QueryPreProcessor testPreProcessor;
      std::vector<std::string> tokenizedVector;
      Logger::WriteMessage("Testing declaration splitting");

      std::vector<std::string> testVector;
      testVector.push_back("assign");
      tokenizedVector = testPreProcessor.tokenizeDeclaration(testInput);
      Assert::IsTrue(tokenizedVector.front() == testVector.front());
    }

    TEST_METHOD(TestSplit) {
      QueryPreProcessor testPreProcessor;
      Logger::WriteMessage("Testing declaration statement");

      std::string testResult = "assign a, b; stmt s";
      Assert::IsTrue(testPreProcessor.splitString(testInput) == testResult);
    }
  };
}