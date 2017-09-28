#include "stdafx.h"
#include "CppUnitTest.h"
#include "CallsTable.h"
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestCallsTable) {
  private:
    CallsTable* m_callsTable;
    std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_testCallsMapResult;
    std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_testCalledByMapResult;
  public:
    TEST_METHOD_INITIALIZE(InitialisePkbAndEvaluator) {
      m_callsTable = new CallsTable();
      m_testCallsMapResult = {
        { "ATLANTA",{ "BOSTON", "CLEVELAND" } },  
        { "CLEVELAND",{ "BOSTON"} },
        { "DENVER",{ "CLEVELAND" } }
      };

      m_testCalledByMapResult = {
        { "BOSTON",{ "ATLANTA", "CLEVELAND" } },
        { "CLEVELAND",{ "DENVER", "ATLANTA" } }
      };
      m_callsTable->insertCalls("ATLANTA", "BOSTON");
      m_callsTable->insertCalls("CLEVELAND", "BOSTON");
      m_callsTable->insertCalls("DENVER", "CLEVELAND");
      m_callsTable->insertCalls("ATLANTA", "CLEVELAND");
    }
    TEST_METHOD(TestInsertCalls) {
      Assert::IsTrue(m_callsTable->getCallsMap() == m_testCallsMapResult);
      Assert::IsTrue(m_callsTable->getCalledByMap() == m_testCalledByMapResult);
    }
    TEST_METHOD(TestIsCalls) {
      bool expected = m_callsTable->isCalls("ATLANTA", "BOSTON");
      Assert::IsTrue(expected);
      expected = m_callsTable->isCalls("DENVER", "CLEVELAND");
      Assert::IsTrue(expected);
      //non-existent calls(proc1, proc2) relationship.
      expected = m_callsTable->isCalls("ATLANTA", "DENVER");
      Assert::IsFalse(expected);
      //non-existent proc1
      expected = m_callsTable->isCalls("atlanta", "BOSTON");
      Assert::IsFalse(expected);
      //non-existent proc2
      expected = m_callsTable->isCalls("ATLANTA", "WASHINGTON");
      Assert::IsFalse(expected);
    }

    TEST_METHOD(TestGetCalls) {
      static const std::string arr[] = { "BOSTON", "CLEVELAND" };
      LIST_OF_PROC_NAMES expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      LIST_OF_PROC_NAMES actual = m_callsTable->getCalls("ATLANTA");
      Assert::IsTrue(expected == actual);

      LIST_OF_PROC_NAMES emptyVector;
      actual = m_callsTable->getCalls("BOSTON");
      Assert::IsTrue(emptyVector == actual);
    }

    TEST_METHOD(TestGetCalledBy) {
      static const std::string arr[] = { "ATLANTA", "CLEVELAND" };
      LIST_OF_PROC_NAMES expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      LIST_OF_PROC_NAMES actual = m_callsTable->getCalledBy("BOSTON");
      Assert::IsTrue(expected == actual);

      LIST_OF_PROC_NAMES emptyVector;
      actual = m_callsTable->getCalledBy("DENVER");
      Assert::IsTrue(emptyVector == actual);
    }

    TEST_METHOD(TestAllCalls) {
      std::unordered_map<PROC_NAME, PROC_NAME> expected;
      expected = {
        { "ATLANTA", "BOSTON" },
        { "CLEVELAND", "BOSTON" },
        { "ATLANTA", "CLEVELAND" },
        { "DENVER", "CLEVELAND"}
      };

      std::unordered_map<PROC_NAME, PROC_NAME> actual = m_callsTable->getAllCalls();
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestGetCallsAnything) {
      static const std::string arr[] = { "ATLANTA", "CLEVELAND", "DENVER" };
      LIST_OF_PROC_NAMES expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      LIST_OF_PROC_NAMES actual = m_callsTable->getCallsAnything();
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestGetCalledByAnything) {
      static const std::string arr[] = { "BOSTON", "CLEVELAND" };
      LIST_OF_PROC_NAMES expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      LIST_OF_PROC_NAMES actual = m_callsTable->getCalledByAnything();
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestHasCallsRelationship) {
      bool actual = m_callsTable->hasCallsRelationship();
      Assert::IsTrue(actual);
    }
  };
}