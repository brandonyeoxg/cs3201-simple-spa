#include "stdafx.h"
#include "CppUnitTest.h"
#include "AssignTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestAssignTable) {
  public:
    AssignTable* m_assignTable;
    TEST_METHOD_INITIALIZE(initialiseAssignTest) {
      m_assignTable = new AssignTable();

      m_assignTable->insertAssignStmt(1, 0, "a");
      m_assignTable->insertAssignStmt(2, 1, "b");
    }
    TEST_METHOD_CLEANUP(cleanupAssignTest) {
      delete m_assignTable;
    }
    TEST_METHOD(TestInsertAssignStmt) {
      int expected = 2;
      int actual = m_assignTable->getAllAssignStmt().size();
      Assert::IsTrue(expected == actual);

      expected = 3;
      m_assignTable->insertAssignStmt(3, 2, "c");
      actual = m_assignTable->getAllAssignStmt().size();
      Assert::IsTrue(expected == actual);

      auto actualList = m_assignTable->getAllAssignStmt();
      LIST_OF_STMT_NUMS expectedList = { 1, 2, 3 };
      Assert::IsTrue(expectedList == actualList);

      auto actualMap = m_assignTable->getAllVarIndexWithAssignStmtNum();
      MAP_OF_VAR_INDEX_TO_STMT_NUMS expectedMap = { std::make_pair(VAR_INDEX(0),LIST_OF_STMT_NUMS({1})),
                                                    std::make_pair(VAR_INDEX(1),LIST_OF_STMT_NUMS({2})),
                                                    std::make_pair(VAR_INDEX(2),LIST_OF_STMT_NUMS({3})) };
      Assert::IsTrue(expectedMap == actualMap);
    }

    TEST_METHOD(TestGetAllStmtList) {
      int expected = 2;
      int actual = m_assignTable->getAllAssignStmt().size();
      Assert::IsTrue(expected == actual);

      auto actualList = m_assignTable->getAllAssignStmt();
      LIST_OF_STMT_NUMS expectedList = { 1, 2 };
      Assert::IsTrue(expectedList == actualList);
    }

    TEST_METHOD(TestGetAllStmtListByVar)
    {
      LIST_OF_STMT_NUMS actualList = m_assignTable->getAllAssignStmtListByVar(0);
      LIST_OF_STMT_NUMS expectedList = { 1 };
      Assert::IsTrue(expectedList == actualList);

      actualList = m_assignTable->getAllAssignStmtListByVar(1);
      expectedList = { 2 };
      Assert::IsTrue(expectedList == actualList);

      actualList = m_assignTable->getAllAssignStmtListByVar(10);
      expectedList = {};
      Assert::IsTrue(expectedList == actualList);
    }

    TEST_METHOD(TestgetAllAssignStmtWithVar)
    {
      auto actual = m_assignTable->getAllAssignStmtWithVar();
      MAP_OF_STMT_NUM_TO_VAR_NAME expected = { {STMT_NUM(1), VAR_NAME("a")},
                                             {STMT_NUM(2), VAR_NAME("b")} };

      Assert::IsTrue(expected == actual);
    }
  };
}