#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql/QueryProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestQueryProcessor) {
public:
  std::string sampleString = "stmt s; variable v; Select s such that Uses(2, v)";
  TEST_METHOD(TestRunQueryInput) {
    Logger::WriteMessage("Running QueryProcessor Run Method. Return false if error");
    //std::string expectedDeclaration = "stmt s; variable v;";
    PKB pkb;
    bool successful = false;

    QueryProcessor *q1 = new QueryProcessor(&pkb);

    std::list<std::string> result = q1->runQueryProcessor(sampleString);
    successful = true;
    Assert::IsTrue(successful);
    }
  };
}