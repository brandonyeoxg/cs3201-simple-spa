#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestParentTable) {
  public:
    MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_testChildMap;
    MAP_OF_STMT_NUMS m_testParentMap;
    TEST_METHOD_INITIALIZE(InitialiseParentTable) {
      m_testChildMap = {
        { 1,{ 2, 3 } },
        { 2,{ 4 } }
      };
      m_testParentMap = {
        { 2, 1 },
        { 3, 1 },
        { 4, 2 }
      };
    }
    TEST_METHOD(TestInsertParent) {
      //test insertFollows method.
      ParentTable *testParentTable = new ParentTable();
      testParentTable->insertParent(1, 2);
      testParentTable->insertParent(1, 3);
      testParentTable->insertParent(2, 4);
      //std::unordered_map<int, int> expected = testParentTable->getParentMap();
      //Assert::IsTrue(expected == testParentMap);
      Assert::IsTrue(testParentTable->getChildMap() == m_testChildMap);
    }

    TEST_METHOD(TestIsParent) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->setParentMap(m_testParentMap);
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
      testParentTable->setParentMap(m_testParentMap);
      testParentTable->setChildMap(m_testChildMap);
      testParentTable->populateParentStarMap();
      testParentTable->populateParentMatrix(4);
      //test isParentStar method (correct behaviour as isParent).
      Assert::IsTrue(testParentTable->isParentStar(1, 2));
      //test isParentStar method (correct behaviour).
      Assert::IsTrue(testParentTable->isParentStar(1, 4));
      //test isParentStar method (non-existent parentStar relationship).
      Assert::IsFalse(testParentTable->isParentStar(2, 3));
      //test isParentStar method (non-existent s1 and s2, check to avoid infinite loop).
      Assert::IsFalse(testParentTable->isParentStar(3, 5));
    }

    TEST_METHOD(TestGetParentOf) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->setParentMap(m_testParentMap);
      //test getParentOf method (correct behaviour).
      Assert::IsTrue(testParentTable->getParentOf(2) == 1);
      Assert::IsTrue(testParentTable->getParentOf(4) == 2);
      //test getParentOf method (non-existent parent relationship).
      Assert::IsFalse(testParentTable->getParentOf(4) == 1);
      //test getParentOf method (catch exception for non-existent s2).
      BOOLEAN exceptionThrown = false;
      try {
        int expected = testParentTable->getParentOf(5);
      } catch (InvalidArgumentException) {
        Logger::WriteMessage("Exception thrown in getParentOf");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown);
    }

    TEST_METHOD(TestGetChildrenOf) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->setChildMap(m_testChildMap);
      //test getChildrenOf method (correct behaviour).
      static const int arr[] = { 2, 3 };
      LIST_OF_STMT_NUMS actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(testParentTable->getChildrenOf(1) == actual);
      //test getChildrenOf method (expect empty vector for non-existent s1).
      LIST_OF_STMT_NUMS emptyVector = testParentTable->getChildrenOf(0);
      Assert::IsTrue(emptyVector.size() == 0);
    }

    TEST_METHOD(TestGetChildrenOfAnything) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->insertParent(1, 2);
      testParentTable->insertParent(1, 3);
      testParentTable->insertParent(2, 4);
      testParentTable->populateParentAnythingRelationships();
      static const int arr[] = { 2, 3, 4 };
      LIST_OF_STMT_NUMS actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      LIST_OF_STMT_NUMS expected = testParentTable->getChildrenOfAnything();
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestGetParentOfAnything) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->insertParent(1, 2);
      testParentTable->insertParent(1, 3);
      testParentTable->insertParent(2, 4);
      testParentTable->populateParentAnythingRelationships();
      static const int arr[] = { 1, 2 };
      LIST_OF_STMT_NUMS actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      LIST_OF_STMT_NUMS expected = testParentTable->getParentOfAnything();
      Assert::IsTrue(expected == actual);
    }
  };
}