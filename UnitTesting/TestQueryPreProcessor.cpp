#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryPreProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestQueryPreProcessor) {
  public:
    std::string sampleString = "   stmt s, s1, s2; assign a, b; while w;    Select s such that Follows(a, s);    ";
    TEST_METHOD(TestSplitStringDeclaration) {
      Logger::WriteMessage("Running QueryPreProcessor Declaration splitting");
      std::string expectedDeclaration = "stmt s, s1, s2; assign a, b; while w;";
      QueryPreProcessor q1;
      
      std::string result = q1.splitStringDeclaration(sampleString);

      Assert::IsTrue(result == expectedDeclaration);
    }
    TEST_METHOD(TestSplitStringQuery) {
      Logger::WriteMessage("Running QueryPreProcessor Query splitting");
      std::string expectedQuery = "Select s such that Follows(a, s);";
      QueryPreProcessor q1;

      std::string result = q1.splitStringQuery(sampleString);

      Assert::IsTrue(result == expectedQuery);
    }
    TEST_METHOD(TestTokenizedDeclaration) {
      Logger::WriteMessage("Running QueryPreProcessor tokenizeDeclaration");
      
      QueryPreProcessor q1;

      std::string resultString = q1.splitStringDeclaration(sampleString);

      bool result = q1.tokenizeDeclaration(resultString);

      Assert::IsTrue(result == true);
    }
    TEST_METHOD(TestTokenizedQuery) {
      Logger::WriteMessage("Running QueryPreProcessor tokenizeQuery");

      QueryPreProcessor q1;

      std::string resultString = q1.splitStringQuery(sampleString);

      bool result = q1.tokenizeQuery(resultString);

      Assert::IsTrue(result == true);
    }
  };
}