#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestParentTable) {
  public:
    std::unordered_map<int, std::vector<int>> test = {
      { 1,{ 2, 3 } },
      { 2,{ 3, 4 } },
      { 3,{ 4 } }
    };

    TEST_METHOD(TestInsertParent) {
      ParentTable* testParentTable = new ParentTable();
      testParentTable->setParentTable(test);
      testParentTable = testParentTable->insert(testParentTable, 4, 5);
      std::unordered_map<int, std::vector<int>> testParentTableResult;
      testParentTableResult = {
        { 1,{ 2, 3 } },
        { 2,{ 3, 4, 5 } },
        { 3,{ 4, 5 } },
        { 4,{ 5 } }
      };
      Assert::IsTrue(testParentTable->getParentTable() == testParentTableResult);
    }
  };
}