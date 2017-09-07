#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"
#include <stdexcept>

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

    TEST_METHOD(TestGetFollows) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test getFollows");
      FollowTable *testFollowTable = new FollowTable();
      testFollowTable->setFollowTable(test);
      //test getFollows method (correct behaviour)
      int expected = testFollowTable->getFollows(1);
      Assert::IsTrue(expected == 2);
      //test getFollows method (non-existing s1, expects exception)
      bool exceptionThrown = false;
      try {
        int expected = testFollowTable->getFollows(5);
      } catch(std::invalid_argument) {
        Logger::WriteMessage("Exception thrown in getFollows");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown);
    }

    TEST_METHOD(TestGetFollowedBy) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test getFollowedBy");
      FollowTable *testFollowTable = new FollowTable();
      testFollowTable->setFollowTable(test);
      //test getFollowedBy method (correct behaviour)
      int expected = testFollowTable->getFollowedBy(3);
      Assert::IsTrue(expected == 2);
      //test getFollowedBy method (s2 being the first element in vector)
      expected = testFollowTable->getFollowedBy(2);
      Assert::IsTrue(expected == 1);
      //test getFollowed method (non-existing s2, expects exception)
      bool exceptionThrown = false;
      try {
        expected = testFollowTable->getFollowedBy(5);
      } catch (std::invalid_argument) {
        Logger::WriteMessage("Exception thrown in getFollowedBy");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown);
    }

    TEST_METHOD(TestGetFollowsStar) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test getFollowsStar");
      FollowTable *testFollowTable = new FollowTable();
      testFollowTable->setFollowTable(test);
      //test getFollowsStar method (correct behaviour)
      std::vector<int> expected = testFollowTable->getFollowsStar(1);
      static const int arr[] = { 2, 3, 4 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(expected == actual);

      //test getFollowsStar method (non-existent s1, return empty vector)
      std::vector<int> emptyResult;
      expected = testFollowTable->getFollowsStar(5);
      Assert::IsTrue(expected == emptyResult);
    }

    TEST_METHOD(TestGetFollowedByStar) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test getFollowedByStar");
      FollowTable *testFollowTable = new FollowTable();
      testFollowTable->setFollowTable(test);
      //test getFollowedByStar method (correct behaviour)
      std::vector<int> expected = testFollowTable->getFollowedByStar(4);
      static const int arr[] = { 1, 2, 3 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(expected == actual);
      //test getFollowedByStar method (non-existent s2, return empty vector)
      std::vector<int> emptyResult;
      expected = testFollowTable->getFollowedByStar(5);
      Assert::IsTrue(expected == emptyResult);
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