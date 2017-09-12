#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"
#include "ParentTable.h"
#include "VarTable.h"
#include "PKB.h"
#include "Grammar.h"

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



    TEST_METHOD(TestPKBTypeOfStatementTable) {
      std::unordered_map<int, Grammar::GType> testTypeOfStatementTable = {
        {1, Grammar::GType::ASGN},
        {2, Grammar::GType::WHILE },
        {3, Grammar::GType::IF }
      };     
      PKB *pkb = new PKB();
      pkb->insertTypeOfStatementTable(1, Grammar::GType::ASGN);
      pkb->insertTypeOfStatementTable(2, Grammar::GType::WHILE);
      pkb->insertTypeOfStatementTable(3, Grammar::GType::IF);
      Assert::IsTrue(testTypeOfStatementTable == pkb->getTypeOfStatementTable());
      //test duplicate line number.
      bool actual = pkb->insertTypeOfStatementTable(1, Grammar::GType::WHILE);
      //Assert::IsFalse(actual);

    }

    TEST_METHOD(TestPKBInsertStatementTypeTable) {
      std::unordered_map<Grammar::GType, std::vector<int>> testStatementTypeTable = {
        { Grammar::GType::ASGN, {1, 2, 3}},
        { Grammar::GType::WHILE, {4, 5}},
        { Grammar::GType::IF, {6}}
      };
      PKB *pkb = new PKB();
      pkb->insertStatementTypeTable(Grammar::GType::ASGN, 1);
      pkb->insertStatementTypeTable(Grammar::GType::ASGN, 2);
      pkb->insertStatementTypeTable(Grammar::GType::ASGN, 3);
      pkb->insertStatementTypeTable(Grammar::GType::WHILE, 4);
      pkb->insertStatementTypeTable(Grammar::GType::WHILE, 5);
      pkb->insertStatementTypeTable(Grammar::GType::IF, 6);
      Assert::IsTrue(testStatementTypeTable == pkb->getStatementTypeTable());
      //test for duplicate entry. 
      bool actual = pkb->insertStatementTypeTable(Grammar::GType::IF, 6);
      Assert::IsFalse(actual);

    }
    //TODO: combine the two stmt tables for insertion, test for duplicate line number entries.
  };

}