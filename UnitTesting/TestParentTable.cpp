#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestParentTable) {
  public:
    std::unordered_map<int, std::vector<int>> testChildMap = {
      { 1,{ 2, 3 } },
      { 2,{ 4 } }
    };
    std::unordered_map<int, int> testParentMap = {
      {2, 1},
      {3, 1},
      {4, 2}
    };
    TEST_METHOD(TestInsertParent) {
      //test insertFollows method.
      ParentTable *testParentTable = new ParentTable();
      testParentTable->insertParent(1, 2);
      testParentTable->insertParent(1, 3);
      testParentTable->insertParent(2, 4);
      //std::unordered_map<int, int> expected = testParentTable->getParentMap();
      //Assert::IsTrue(expected == testParentMap);
      Assert::IsTrue(testParentTable->getChildMap() == testChildMap);
    }
  };
}