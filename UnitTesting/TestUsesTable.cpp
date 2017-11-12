#include "stdafx.h"
#include "CppUnitTest.h"
#include "UsesTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestUsesTable) {
  private:
    UsesTable* m_usesTable;
    MAP_OF_STMT_NUM_TO_LIST_OF_VAR_NAMES m_testUsesStmtMap;
    MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS m_testUsesVarMap;
  public:
    TEST_METHOD_INITIALIZE(InitialiseUsesTable) {
      m_usesTable = new UsesTable();
      m_testUsesVarMap = {
        { "x",{ 1, 2 } },
        { "y",{ 2, 3 } }
      };
      m_testUsesStmtMap = {
        { 1,{ "x" } },
        { 2,{ "x", "y" } },
        { 3,{ "y" } }
      };
      m_usesTable->insertUsesForStmt("x", 1, 0);
      m_usesTable->insertUsesForStmt("x", 2, 0);
      m_usesTable->insertUsesForStmt("y", 2, 1);
      m_usesTable->insertUsesForStmt("y", 3, 1);
      m_usesTable->populateUsesAnythingRelationships();
    }
    TEST_METHOD(TestUsesTable_InsertUses) {

      Assert::IsTrue(m_usesTable->getUsesStmtMap() == m_testUsesStmtMap);
      Assert::IsTrue(m_usesTable->getUsesVarMap() == m_testUsesVarMap);
      //insert duplicate, expects no change made to the data.
      m_usesTable->insertUsesForStmt("y", 3, 1);
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
    TEST_METHOD(TestUsesTable_GetUses) {
      static const std::string arr[] = { "x", "y" };
      LIST_OF_VAR_NAMES expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(m_usesTable->getUses(2) == expected);
      LIST_OF_VAR_NAMES emptyVector;
      Assert::IsTrue(m_usesTable->getUses(4) == emptyVector);
    }
    TEST_METHOD(TestUsesTable_GetUsesByIdx) {
      static const int arr[] = { 0, 1 };
      LIST_OF_VAR_INDICES expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(m_usesTable->getUsesByIdx(2) == expected);
      LIST_OF_VAR_INDICES emptyVector;
      Assert::IsTrue(m_usesTable->getUsesByIdx(4) == emptyVector);
    }
    TEST_METHOD(TestUsesTable_GetStmtUses) {
      static const int arr[] = { 1, 2 };
      LIST_OF_STMT_NUMS expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(m_usesTable->getStmtUses("x") == expected);
      LIST_OF_STMT_NUMS emptyVector;
      Assert::IsTrue(m_usesTable->getStmtUses("z") == emptyVector);
    }
    TEST_METHOD(TestUsesTable_IsUsesAnything) {
      Assert::IsTrue(m_usesTable->isUsesAnything(1));
      Assert::IsFalse(m_usesTable->isUsesAnything(4));
    }
    TEST_METHOD(TestUsesTable_GetStmtUsesAnything) {
      static const int arr[] = { 1, 2, 3 };
      LIST_OF_STMT_NUMS expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(m_usesTable->getStmtUsesAnything() == expected);
    }
  };
}