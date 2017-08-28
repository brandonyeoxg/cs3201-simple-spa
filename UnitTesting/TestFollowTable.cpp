#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  public:
    unordered_map<int, std::vector<int>> test = {
      { 1,{ 2, 3 } },
      { 2,{ 3, 4 } },
      { 3,{ 4 } }
    };
    TEST_METHOD(TestInsertFollow) {
      Logger::WriteMessage("Running follow table test insert");
      FollowTable testFollowTable;
      testFollowTable.setFollowTable(test);
      testFollowTable = testFollowTable.insert(testFollowTable, 4, 5);
      unordered_map<int, std::vector<int>> testFollowTableResult;
      testFollowTableResult = {
        { 1, { 2, 3 } },
        { 2, { 3, 4, 5 } },
        { 3, { 4, 5 } },
        { 4, { 5 } }
      };
      Assert::IsTrue(testFollowTable.getFollowTable() == testFollowTableResult);

    }

    TEST_METHOD(TestGetS1Follow) {
      Logger::WriteMessage("Running follow table test getS1");
      static const int arr[] = { 1, 2 };
      std::vector<int> expected (arr, arr + sizeof(arr) / sizeof(arr[0]));
      FollowTable testFollowTable;
      testFollowTable.setFollowTable(test);
      vector<int> actual = testFollowTable.getS1(3);
      Assert::IsTrue(actual == expected);
    }

    TEST_METHOD(TestGetS2Follow) {
      Logger::WriteMessage("Running follow table test getS2");
      static const int arr[] = { 3, 4 };
      std::vector<int> expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      FollowTable testFollowTable;
      testFollowTable.setFollowTable(test);
      vector<int> actual = testFollowTable.getS2(2);
      Assert::IsTrue(actual == expected);
    }
  };
}