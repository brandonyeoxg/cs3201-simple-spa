#include "stdafx.h"
#include "CppUnitTest.h"
#include "UsesTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestUsesTable) {
  private:
    UsesTable* m_usesTable;
    std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> m_testUsesStmtMap;
    std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> m_testUsesVarMap;
  public:
    TEST_METHOD_INITIALIZE(InitialiseUsesTable) {
      m_usesTable = new UsesTable();
      m_testUsesVarMap = {
        {"x", {1, 2}},
        {"y", {2, 3}}
      };
      m_testUsesStmtMap = {
        {1, {"x"}},
        {2, {"x", "y"}},
        {3, {"y"}}
      };
      
      m_usesTable->insertUsesForStmt("x", 1);
      m_usesTable->insertUsesForStmt("x", 2);
      m_usesTable->insertUsesForStmt("y", 2);
      m_usesTable->insertUsesForStmt("y", 3); 
    }
    TEST_METHOD(TestUsesTable_InsertUses) {
      Assert::IsTrue(m_usesTable->getUsesStmtMap() == m_testUsesStmtMap);
      Assert::IsTrue(m_usesTable->getUsesVarMap() == m_testUsesVarMap);
      //insert duplicate, expects no change made to the data.
      m_usesTable->insertUsesForStmt("y", 3);
      Assert::IsTrue(m_usesTable->getUsesStmtMap() == m_testUsesStmtMap);
      Assert::IsTrue(m_usesTable->getUsesVarMap() == m_testUsesVarMap);
    }
    TEST_METHOD(TestUsesTable_IsUses) {
      //true relationships
      Assert::IsTrue(m_usesTable->isUses(1, "x"));
      Assert::IsTrue(m_usesTable->isUses(2, "y"));
      //non-existent key/value pair
      Assert::IsFalse(m_usesTable->isUses(3, "x"));
      //non-existent statement number
      Assert::IsFalse(m_usesTable->isUses(4, "x"));
    }
  };
}