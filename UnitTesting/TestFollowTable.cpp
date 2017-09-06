#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  public:
    
    TEST_METHOD(TestInsertFollow) {
      //test insertFollows method.
      FollowTable *testFollowTable = new FollowTable();
      testFollowTable->insertFollows(1, 2);
      testFollowTable->insertFollows(2, 3);
      testFollowTable->insertFollows(3, 4);
      testFollowTable->insertFollows(4, 5);
      std::unordered_map<int, std::vector<int>> testFollowTableResult;
      testFollowTableResult = {
        { 1, { 2, 3, 4, 5 } },
        { 2, { 3, 4, 5 } },
        { 3, { 4, 5 } },
        { 4, { 5 } }
      };
      Assert::IsTrue(testFollowTable->getFollowTable() == testFollowTableResult);
      //test insertFollows method (duplicate key/value).
      //testFollowTable->setFollowTable(test);
      bool expected = testFollowTable->insertFollows(2, 3);
      Assert::IsFalse(expected);
    }
    TEST_METHOD(TestIsFollows) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test isFollows");
      FollowTable *testFollowTable = new FollowTable();
      testFollowTable->setFollowTable(test);
      //test isFollows method (correct behaviour).
      bool expected = testFollowTable->isFollows(1, 2);
      Assert::IsTrue(expected);
      //test isFollows method (existing in vector but not first element).
      expected = testFollowTable->isFollows(1, 3);
      Assert::IsFalse(expected);
      //test isFollows method (non-existing key/value pair).
      expected = testFollowTable->isFollows(1, 6);
      Assert::IsFalse(expected);
      //test isFollows method (non-existing key).
      expected = testFollowTable->isFollows(5, 6);
      Assert::IsFalse(expected);
    }

    TEST_METHOD(TestIsFollowsStar) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test isFollowsStar");
      FollowTable *testFollowTable = new FollowTable();
      testFollowTable->setFollowTable(test);
      //test isFollowsStar method (correct behaviour).
      bool expected = testFollowTable->isFollowsStar(1, 4);
      Assert::IsTrue(expected);
      //test isFollowsStar method (non-existing key).
      expected = testFollowTable->isFollows(4, 5);
      Assert::IsFalse(expected);
      //test isFollowsStar method (non-existing value in existing key).
      expected = testFollowTable->isFollows(2, 5);
      Assert::IsFalse(expected);
    }
    TEST_METHOD(TestGetS1Follow) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test getS1");
      static const int arr[] = { 1, 2 };
      std::vector<int> expected (arr, arr + sizeof(arr) / sizeof(arr[0]));
      FollowTable *testFollowTable = new FollowTable();
      testFollowTable->setFollowTable(test);
      std::vector<int> actual = testFollowTable->getS1(3);
      Assert::IsTrue(actual == expected);
    }

    TEST_METHOD(TestGetS2Follow) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test getS2");
      static const int arr[] = { 3, 4 };
      std::vector<int> expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      FollowTable *testFollowTable = new FollowTable();
      testFollowTable->setFollowTable(test);
      std::vector<int> actual = testFollowTable->getS2(2);
      Assert::IsTrue(actual == expected);
    }
  };
}