#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  public:

    TEST_METHOD(TestInsert) {
      Logger::WriteMessage("Running follow table test");
      FollowTable testFollowTable;
      unordered_map<int, std::vector<int>> test = {
        { 1, { 2, 3 } },
        { 2, { 3, 4 } },
        { 3, { 4 } }
      }; 
      
      testFollowTable.setFollowTable(test);
      testFollowTable = testFollowTable.insert(testFollowTable, 4, 5);
      unordered_map<int, std::vector<int>> testFollowTableResult;
      testFollowTableResult = {
        { 1, { 2, 3 } },
        { 2, { 3, 4, 5 } },
        { 3, { 4, 5 } },
        { 4, { 5 } }
      };
      //testFollowTable.setFollowTable(testFollowTableResult);
      Assert::IsTrue(testFollowTable.getFollowTable() == testFollowTableResult);
      //Assert::IsTrue(2 == 2);
      //Assert:AreEquals
      //return;
    }
  };
}