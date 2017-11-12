#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql/QueryPreProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestQueryPreProcessor) {
  public:
    std::string sampleString = "stmt s; variable v; Select s such that Uses(2, v)";
    TEST_METHOD(TestSplitStringDeclaration) {
      Logger::WriteMessage("Running QueryPreProcessor Declaration splitting");
      std::string expectedDeclaration = "stmt s; variable v;";
      QueryPreProcessor q1;
      
      std::string result = q1.splitStringDeclaration(sampleString);

      Assert::IsTrue(result == expectedDeclaration);
    }
    TEST_METHOD(TestSplitStringQuery) {
      Logger::WriteMessage("Running QueryPreProcessor Query splitting");
      std::string expectedQuery = "Select s such that Uses(2, v)";
      QueryPreProcessor q1;

      std::string result = q1.splitStringQuery(sampleString);

      Assert::IsTrue(result == expectedQuery);
    }
    TEST_METHOD(TestTokenizedDeclaration) {
      Logger::WriteMessage("Running QueryPreProcessor tokenizeDeclaration");
      
      QueryPreProcessor q1;

      std::string resultString = q1.splitStringDeclaration(sampleString);

      bool result = q1.tokenizeDeclaration(resultString);
            
      Assert::IsTrue(q1.getGrammarVector().size() == 2);
      Assert::IsTrue(result == true);
    }
    TEST_METHOD(TestTokenizedQuery) {
      /*Logger::WriteMessage("Running QueryPreProcessor tokenizeQuery");

      QueryPreProcessor q1;

      std::string declarationResultString = q1.splitStringDeclaration(sampleString);
      std::string queryResultString = q1.splitStringQuery(sampleString);

      bool resultDeclaration = q1.tokenizeDeclaration(declarationResultString);
      bool result = q1.tokenizeQuery(queryResultString);

      std::queue<Grammar> testQueue = q1.getSelect();
      std::queue<Relation> testDAOQueue = q1.getSuchThat();
      int i = testQueue.size();
      int j = testDAOQueue.size();
      
      Assert::IsTrue(i == 1);
      Assert::IsTrue(j == 1);
      Assert::IsTrue(result == true);*/
    }
  };
}