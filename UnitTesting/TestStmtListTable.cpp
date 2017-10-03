#include "stdafx.h"
#include "CppUnitTest.h"
#include "StmtListTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestStmtListTable) {
  public:
    StmtListTable* m_stmtList;

    TEST_METHOD_INITIALIZE(instantiateStmtList) 
    {
      m_stmtList = new StmtListTable();
      m_stmtList->insertStmtLst(1);
      m_stmtList->insertStmtLst(3);
      m_stmtList->insertStmtLst(4);
      m_stmtList->insertStmtLst(9);
    }

    TEST_METHOD_CLEANUP(cleanupStmtList) 
    {
      delete m_stmtList;
      m_stmtList = nullptr;
    }
    
    TEST_METHOD(TestInsertStmtList)
    {
      m_stmtList->insertStmtLst(10);
      LIST_OF_STMT_NUMS actual = m_stmtList->getStmtList();
      LIST_OF_STMT_NUMS expected = { 1, 3, 4, 9, 10 };
      Assert::IsTrue(actual == expected);
    }
  };
}