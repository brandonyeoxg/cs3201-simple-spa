#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb/PKB.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  public:

    TEST_METHOD(TestPKBInsertFollow) {
      MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      PKB *pkb = new PKB();
      LIST_OF_STMT_NUMS stmtInLst;
      pkb->insertFollowsRelation(stmtInLst, 1);
      stmtInLst.push_back(1);

      pkb->insertFollowsRelation(stmtInLst, 2);
      stmtInLst.push_back(2);

      pkb->insertFollowsRelation(stmtInLst, 3);
      stmtInLst.push_back(3);

      pkb->insertFollowsRelation(stmtInLst, 4);
      stmtInLst.push_back(4);

      Assert::IsTrue(pkb->getFollowTable()->getFollowTable() == test);
    }

    TEST_METHOD(TestPKBGetFollows) {
      MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test getFollows");
      PKB *pkb = new PKB();
      pkb->getFollowTable()->setFollowTable(test);
      //test getFollows method (correct behaviour)
      int expected = pkb->getFollows(1);
      Assert::IsTrue(expected == 2);
    }

    TEST_METHOD(TestPKBTypeOfStatementTable) {
      MAP_OF_STMT_NUM_TO_GTYPE testTypeOfStatementTable = {
        {1, queryType::GType::ASGN},
        {2, queryType::GType::WHILE },
        {3, queryType::GType::IF }
      };
      PKB *pkb = new PKB();
      pkb->insertTypeOfStatementTable(1, queryType::GType::ASGN);
      pkb->insertTypeOfStatementTable(2, queryType::GType::WHILE);
      pkb->insertTypeOfStatementTable(3, queryType::GType::IF);
      Assert::IsTrue(testTypeOfStatementTable == pkb->getTypeOfStatementTable());
      //test duplicate line number.
      bool actual = pkb->insertTypeOfStatementTable(1, queryType::GType::WHILE);
      //Assert::IsFalse(actual);

    }

    TEST_METHOD(TestPKBInsertStatementTypeTable) {
      MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS testStatementTypeTable = {
        { queryType::GType::ASGN, {1, 2, 3}},
        { queryType::GType::WHILE, {4, 5}},
        { queryType::GType::IF, {6}}
      };
      PKB *pkb = new PKB();
      pkb->insertStatementTypeTable(queryType::GType::ASGN, 1);
      pkb->insertStatementTypeTable(queryType::GType::ASGN, 2);
      pkb->insertStatementTypeTable(queryType::GType::ASGN, 3);
      pkb->insertStatementTypeTable(queryType::GType::WHILE, 4);
      pkb->insertStatementTypeTable(queryType::GType::WHILE, 5);
      pkb->insertStatementTypeTable(queryType::GType::IF, 6);
      Assert::IsTrue(testStatementTypeTable == pkb->getStatementTypeTable());
      //test for duplicate entry.
      bool actual = pkb->insertStatementTypeTable(queryType::GType::IF, 6);
      Assert::IsFalse(actual);

    }
  };
}
