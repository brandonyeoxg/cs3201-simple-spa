#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"
#include "ParentTable.h"
#include "VarTable.h"
#include "PKB.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  public:
    
    TEST_METHOD(TestPKBInsertFollow) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      PKB *pkb = new PKB();
      pkb->insertFollows(1, 2);
      pkb->insertFollows(2, 3);
      pkb->insertFollows(3, 4);
      Assert::IsTrue(pkb->getFollowTable()->getFollowTable() == test);
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