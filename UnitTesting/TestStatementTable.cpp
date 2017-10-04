#include "stdafx.h"
#include "CppUnitTest.h"
#include "StatementTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestStatementTable) {\
  private:
    StatementTable* m_statementTable;
    std::unordered_map<STMT_NUM, queryType::GType> m_testTypeOfStatementTable;
    std::unordered_map<queryType::GType, LIST_OF_STMT_NUMS> m_testStatementTypeTable;
  public:
    TEST_METHOD_INITIALIZE(InitialiseStatementTable) {
      m_statementTable = new StatementTable();
      //insert insert
      m_statementTable->insertStatementTypeTable(queryType::GType::CALL, 2);
      m_statementTable->insertStatementTypeTable(queryType::GType::ASGN, 3);
      m_statementTable->insertStatementTypeTable(queryType::GType::CALL, 4);
    }
    TEST_METHOD(TestGetListOfStatements) {
      static const STMT_NUM arr[] = { 2, 4 };
      LIST_OF_STMT_NUMS expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(expected == m_statementTable->getListOfStatements(queryType::GType::CALL));
    }
  };
}