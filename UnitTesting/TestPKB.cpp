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

    TEST_METHOD(TestPKBGetFollows) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test getFollows");
      PKB *pkb = new PKB();
      pkb->setFollowTable(test);
      //test getFollows method (correct behaviour)
      int expected = pkb->getFollows(1);
      Assert::IsTrue(expected == 2);
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

    TEST_METHOD(TestPKBInsertStatementTypeTable) {
      std::unordered_map<int, std::string> testStatementTypeTable = {
        {1, "ASGN"},
        {2, "WHILE"},
        {3, "IF"}
      };     
      PKB *pkb = new PKB();
      pkb->insertStatementTypeTable(1, "ASGN");
      pkb->insertStatementTypeTable(2, "WHILE");
      pkb->insertStatementTypeTable(3, "IF");
      Assert::IsTrue(testStatementTypeTable == pkb->getStatementTypeTable());
      //test duplicate line number.
      bool actual = pkb->insertStatementTypeTable(1, "WHILE");
      //Assert::IsFalse(actual);

    }

  };

}