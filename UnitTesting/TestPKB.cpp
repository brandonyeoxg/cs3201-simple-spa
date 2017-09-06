#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  public:
    TEST_METHOD(TestReturnFollowTable) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running return follow table test...");
      PKB *pkb = new PKB();
      FollowTable* table = pkb->getFollowTable();
      table->setFollowTable(test);

      std::unordered_map<int, std::vector<int>> expected = pkb->returnFollowTable(-1, 3);
      std::unordered_map<int, std::vector<int>> actual;
      actual = {
        { 3,{ 1, 2 } }
      };
      //test with s1 as querying variable
      Assert::IsTrue(expected == actual);
      actual = {
        { 2,{ 3, 4 } }
      };
      expected = pkb->returnFollowTable(2, -2);
      //test with s2 as querying variable
      Assert::IsTrue(expected == actual);

      expected = pkb->returnFollowTable(-1, -2);
      //test with both s1 and s2 as querying variable
      Assert::IsTrue(expected == test);

      //test with both s1 and s2 not being querying variable
      expected = pkb->returnFollowTable(2, 3);
      actual = {
        { 2,{ 3, 4 } }
      };
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestReturnVarTable) {
      Logger::WriteMessage("Running return parent table test...");
      std::unordered_map<std::string, std::vector<int>> testVar = {
        { "x",{ 1, 2 } },
        { "y",{ 2, 3 } },
        { "z",{ 4 } }
      };
      PKB *pkb = new PKB();
      VarTable *varTable = pkb->getVarTable();
      varTable->setVarTable(testVar);
      std::unordered_map<std::string, std::vector<int>> actual = {
        { "x",{ 1, 2 } }
      };
      std::unordered_map<std::string, std::vector<int>> expected = pkb->returnVarTable("x");
      Assert::IsTrue(expected == actual);
    }

  };

}