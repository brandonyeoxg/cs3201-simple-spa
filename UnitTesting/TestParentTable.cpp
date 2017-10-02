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
      testParentTable->setChildMap(testChildMap);
      //testParentTable->populateParentedByStarMap();
      //testParentTable->populateParentStarMap();
      ////test isParentStar method (correct behaviour as isParent).
      //Assert::IsTrue(testParentTable->isParentStar(1, 2));
      ////test isParentStar method (correct behaviour).
      //Assert::IsTrue(testParentTable->isParentStar(1, 4));
      ////test isParentStar method (non-existent parentStar relationship).
      //Assert::IsFalse(testParentTable->isParentStar(2, 3));
      ////test isParentStar method (non-existent s1 and s2, check to avoid infinite loop).
      //Assert::IsFalse(testParentTable->isParentStar(5, 10));
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
      //test getChildrenOf method (expect empty vector for non-existent s1).
      std::vector<int> emptyVector = testParentTable->getChildrenOf(0);
      Assert::IsTrue(emptyVector.size() == 0);
    }

    TEST_METHOD(TestGetParentStarOf) {
      //std::unordered_map<int, int> testParentMap = {
      //  { 2, 1 },
      //  { 3, 1 },
      //  { 4, 2 }
      //};
      //ParentTable *testParentTable = new ParentTable();
      //testParentTable->setParentMap(testParentMap);
      //for (auto it = testParentMap.begin(); it != testParentMap.end(); it++) {
      //  testParentTable->populateParentedByStarMap(it);
      //}
      //
      ////test getParentStarOf method (correct behaviour).
      //static const int arr[] = { 2, 1 };
      //std::vector<int> expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      //std::vector<int> actual = testParentTable->getParentStarOf(4);
      //Assert::IsTrue( expected == actual);
      ////test getParentStarOf method (expect empty vector for non-existent s2).
      //std::vector<int> emptyVector = testParentTable->getParentStarOf(0);
      //
      //Assert::IsTrue(emptyVector.size() == 0);  
    }
    
    TEST_METHOD(TestGetChildrenStarOf) {
      //ParentTable *testParentTable = new ParentTable();
      //std::unordered_map<int, int> testGetChildrenStarParentMap = {
      //  { 3, 2 },
      //  { 4, 2 },
      //  { 5, 4 }
      //};
      //std::unordered_map<int, std::vector<int>> testGetChildrenStarMapChildMap = {
      //  { 2,{ 3, 4 } },
      //  { 4,{ 5 } }
      //};
      //testParentTable->setParentMap(testGetChildrenStarParentMap);
      //testParentTable->setChildMap(testGetChildrenStarMapChildMap);
      //testParentTable->populateParentStarMap();
      ////test getChildrenStarOf method (correct behaviour).
      //static const int arr[] = { 3, 4, 5 };
      //std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      //std::vector<int> expected = testParentTable->getChildrenStarOf(2);
      //Assert::IsTrue(expected == actual);
      ////test getChildrenStarOf method (expect empty vector for non-existent s1).
      //std::vector<int> emptyVector = testParentTable->getChildrenStarOf(0);
      //Assert::IsTrue(emptyVector.size() == 0);

    }

    TEST_METHOD(TestPopulateParentStarMap) {
      //ParentTable *testParentTable = new ParentTable();
      //std::unordered_map<int, std::vector<int>> testChildMap = {
      //  { 1,{ 2, 3, 4 } },
      //  { 4,{ 5, 6, 7 } },
      //  { 7, {8} }
      //};
      //testParentTable->setChildMap(testChildMap);
      //testParentTable->populateParentStarMap();
      //std::unordered_map<int, std::vector<int>> expected = {
      //  { 1,{ 2, 3, 4, 5, 6, 7, 8 } },
      //  { 4,{ 5, 6, 7, 8} },
      //  { 7, {8}}
      //};

      //Assert::IsTrue(expected == testParentTable->getParentStarMap());
    }

    TEST_METHOD(TestGetChildrenOfAnything) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->insertParent(1, 2);
      testParentTable->insertParent(1, 3);
      testParentTable->insertParent(2, 4);
      static const int arr[] = { 2, 3, 4 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      std::vector<int> expected = testParentTable->getChildrenOfAnything();
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestGetParentOfAnything) {
      ParentTable *testParentTable = new ParentTable();
      testParentTable->insertParent(1, 2);
      testParentTable->insertParent(1, 3);
      testParentTable->insertParent(2, 4);
      static const int arr[] = { 1, 2 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      std::vector<int> expected = testParentTable->getParentOfAnything();
      Assert::IsTrue(expected == actual);
    }
  };
}