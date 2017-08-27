#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestAST) {
  public:

    TEST_METHOD(TestInsert) {
      unordered_map<int, vector<int>> testFollowTable;
      testFollowTable = {
        { '1', { 2, 3 } },
        { '2', { 3, 4 } },
        { '3', { 4 } }
      };

      unordered_map<int, vector<int>> testFollowTableResult;
      testFollowTable = {
        { '1', { 2, 3 } },
        { '2', { 3, 4 } },
        { '3', { 4 } },
        { '4', { 5 } }
      };
      testFollowTable.insert(4, 5);
      Assert::AreEqual(testFollowTable, testFollowTableResult);

    }
  };
}