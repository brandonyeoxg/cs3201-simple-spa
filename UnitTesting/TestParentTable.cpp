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

    TEST_METHOD(TestIsParent) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->setParentMap(testParentMap);
      //test isParent method (correct behaviour).
      Assert::IsTrue(testParentTable->isParent(1, 2));
      //test isParent method (non-existent parent relationship).
      Assert::IsFalse(testParentTable->isParent(5, 2));
      //test isParent method (non-existent s2).
      Assert::IsFalse(testParentTable->isParent(1, 5));
      //test isParent method (non-existent s1).
      Assert::IsFalse(testParentTable->isParent(5, 2));
    }

    TEST_METHOD(TestIsParentStar) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->setParentMap(testParentMap);
      //test isParentStar method (correct behaviour as isParent).
      Assert::IsTrue(testParentTable->isParentStar(1, 2));
      //test isParentStar method (correct behaviour).
      Assert::IsTrue(testParentTable->isParentStar(1, 4));
      //test isParentStar method (non-existent parentStar relationship).
      Assert::IsFalse(testParentTable->isParentStar(2, 3));
      //test isParentStar method (non-existent s1 and s2, check to avoid infinite loop).
      Assert::IsFalse(testParentTable->isParentStar(5, 10));
    }

    TEST_METHOD(TestGetParentOf) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->setParentMap(testParentMap);
      //test getParentOf method (correct behaviour).
      Assert::IsTrue(testParentTable->getParentOf(2) == 1);
      Assert::IsTrue(testParentTable->getParentOf(4) == 2);
      //test getParentOf method (non-existent parent relationship).
      Assert::IsFalse(testParentTable->getParentOf(4) == 1);
      //test getParentOf method (catch exception for non-existent s2).
      bool exceptionThrown = false;
      try {
        int expected = testParentTable->getParentOf(5);
      } catch (std::invalid_argument) {
        Logger::WriteMessage("Exception thrown in getParentOf");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown);
    }

    TEST_METHOD(TestGetChildrenOf) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->setChildMap(testChildMap);
      //test getChildrenOf method (correct behaviour).
      static const int arr[] = { 2, 3 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(testParentTable->getChildrenOf(1) == actual);
      //test getChildrenOf method (catch exception for non-existent s1).
      bool exceptionThrown = false;
      try {
        std::vector<int> expected = testParentTable->getChildrenOf(5);
      } catch (std::invalid_argument) {
        Logger::WriteMessage("Exception thrown in getParentOf");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown);
    }

    TEST_METHOD(TestGetParentStarOf) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->setParentMap(testParentMap);
      //test getParentStarOf method (correct behaviour).
      static const int arr[] = { 1, 2 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(testParentTable->getParentStarOf(4) == actual);
      //test getParentStarOf method (catch exception for non-existent s1).
      bool exceptionThrown = false;
      try {
        std::vector<int> expected = testParentTable->getParentStarOf(5);
      } catch (std::invalid_argument) {
        Logger::WriteMessage("Exception thrown in getParentStarOf");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown);
    }
    
  };
}