#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestStatementTable) {\
  private:
    StatementTable* m_statementTable;
    MAP_OF_STMT_NUM_TO_GTYPE m_testTypeOfStatementTable;
    MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS m_testStatementTypeTable;
  public:
    TEST_METHOD_INITIALIZE(InitialiseStatementTable) {
      m_statementTable = new StatementTable();
      //insert insert
      m_statementTable->insertStatementTypeTable(queryType::GType::ASGN, 1);
      m_statementTable->insertStatementTypeTable(queryType::GType::CALL, 2);
      m_statementTable->insertStatementTypeTable(queryType::GType::ASGN, 3);
      m_statementTable->insertStatementTypeTable(queryType::GType::CALL, 4);

      m_statementTable->insertTypeOfStatementTable(1, queryType::GType::ASGN);
      m_statementTable->insertTypeOfStatementTable(2, queryType::GType::CALL);
      m_statementTable->insertTypeOfStatementTable(3, queryType::GType::ASGN);
      m_statementTable->insertTypeOfStatementTable(4, queryType::GType::CALL);
      
    }
    TEST_METHOD(TestGetListOfStatements) {
      Assert::IsTrue(m_statementTable->getNumberOfStatements() == 4);
      static const STMT_NUM arr[] = { 2, 4 };
      LIST_OF_STMT_NUMS expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(expected == m_statementTable->getListOfStatements(queryType::GType::CALL));
    }
  };
}