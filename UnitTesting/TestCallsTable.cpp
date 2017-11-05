#include "stdafx.h"
#include "CppUnitTest.h"
#include "CallsTable.h"
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestCallsTable) {
  private:
    CallsTable* m_callsTable;
    std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_testCallsMap;
    MAP_OF_NAME_TO_SET_OF_NAMES m_testCallsSet;
    std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_testCalledByMap;
    std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_testCallsStarMap;
    MAP_OF_NAME_TO_SET_OF_NAMES m_testCallsStarSet;
    std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_testCalledByStarMap;
    std::unordered_map<STMT_NUM, PROC_NAME> m_testCallsStmtMap;
    std::unordered_map<PROC_NAME, LIST_OF_STMT_NUMS> m_testProcNameToCallsStmtsMap;

  public:
    TEST_METHOD_INITIALIZE(InitialiseCallsTable) {
      m_callsTable = new CallsTable();
      m_testCallsMap = {
        { "ATLANTA",{ "BOSTON", "CLEVELAND" } },  
        { "CLEVELAND",{ "BOSTON"} },
        { "DENVER",{ "CLEVELAND" } }
      };

      m_testCalledByMap = {
        { "BOSTON",{ "ATLANTA", "CLEVELAND" } },
        { "CLEVELAND",{ "DENVER", "ATLANTA" } }
      };

      m_testCallsStarMap = {
        { "ATLANTA",{ "BOSTON", "CLEVELAND" } },
        { "CLEVELAND",{ "BOSTON" } },
        { "DENVER",{ "CLEVELAND", "BOSTON" } }
      };

      m_testCalledByStarMap = {
        { "BOSTON",{ "ATLANTA", "CLEVELAND", "DENVER" } },
        { "CLEVELAND",{ "DENVER", "ATLANTA" } }
      };

      m_testCallsStmtMap = {
        {2, "BOSTON"},
        {3, "BOSTON"},
        {5, "CLEVELAND"},
        {7, "CLEVELAND"}
      };

      m_testProcNameToCallsStmtsMap = {
        {"BOSTON", {2, 3}},
        {"CLEVELAND", {5, 7}}
      };
      m_callsTable->insertCalls("ATLANTA", "BOSTON");
      m_callsTable->insertCallsStmt(2, "BOSTON");
      m_callsTable->insertCalls("CLEVELAND", "BOSTON");
      m_callsTable->insertCallsStmt(3, "BOSTON");
      m_callsTable->insertCalls("DENVER", "CLEVELAND");
      m_callsTable->insertCallsStmt(5, "CLEVELAND");
      m_callsTable->insertCalls("ATLANTA", "CLEVELAND");
      m_callsTable->insertCallsStmt(7, "CLEVELAND");
      m_callsTable->populateCallsStarMap();
      m_callsTable->populateCalledByStarMap();
    }
    TEST_METHOD(TestInsertCalls) {
      Assert::IsTrue(m_callsTable->getCallsMap() == m_testCallsMap);
      Assert::IsTrue(m_callsTable->getCalledByMap() == m_testCalledByMap);
    }
    TEST_METHOD(TestInsertCallsStmt) {
      Assert::IsTrue(m_callsTable->getCallsStmtMap() == m_testCallsStmtMap);
      Assert::IsTrue(m_callsTable->getProcNameToCallsStmtsMap() == m_testProcNameToCallsStmtsMap);
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

    TEST_METHOD(TestGetProcNameFromCallStmtNum) {
      PROC_NAME expected = "BOSTON";
      Assert::IsTrue(expected == m_callsTable->getProcNameFromCallStmtNum(2));
      Assert::IsTrue(expected == m_callsTable->getProcNameFromCallStmtNum(3));
      bool exceptionThrown = false;
      try {
        expected = m_callsTable->getProcNameFromCallStmtNum(9);
      } catch (std::invalid_argument) {
        Logger::WriteMessage("Exception thrown in getProcNameFromCallStmtNum");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown);
      
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

    TEST_METHOD(TestGetCallsByIdx) {
      LIST_OF_PROC_INDICES expected = { 1, 2 };
      LIST_OF_PROC_INDICES actual = m_callsTable->getCallsByIdx(0);
      Assert::IsTrue(expected == actual);

      LIST_OF_PROC_INDICES emptyVector;
      actual = m_callsTable->getCallsByIdx(1);
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

    TEST_METHOD(TestGetCalledByByIdx) {
      LIST_OF_PROC_INDICES expected = { 0, 2 };
      LIST_OF_PROC_INDICES actual = m_callsTable->getCalledByByIdx(1);
      Assert::IsTrue(expected == actual);

      LIST_OF_PROC_INDICES emptyVector;
      actual = m_callsTable->getCalledByByIdx(3);
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

    TEST_METHOD(TestIsCallsAnything) {
      Assert::IsTrue(m_callsTable->isCallsAnything("ATLANTA"));
      Assert::IsFalse(m_callsTable->isCallsAnything("BOSTON"));
    }

    TEST_METHOD(TestIsCalledByAnything) {
      Assert::IsFalse(m_callsTable->isCalledByAnything("ATLANTA"));
      Assert::IsTrue(m_callsTable->isCalledByAnything("BOSTON"));
    }

    TEST_METHOD(TestPopulateCallsStarTable) {
      m_callsTable->populateCallsStarMap();
      Assert::IsTrue(m_callsTable->getCallsStarMap() == m_testCallsStarMap);
    }

    TEST_METHOD(TestPopulateCalledByStarTable) {
      m_callsTable->populateCalledByStarMap();
      Assert::IsTrue(m_callsTable->getCalledByStarMap() == m_testCalledByStarMap);
    }

    TEST_METHOD(TestIsCallsStar) {
      bool actual = m_callsTable->isCallsStar("ATLANTA", "CLEVELAND");
      Assert::IsTrue(actual);
      actual = m_callsTable->isCallsStar("ATLANTA", "DENVER");
      Assert::IsFalse(actual);
    }

    TEST_METHOD(TestGetCallsStar) {
      LIST_OF_PROC_NAMES actual = m_callsTable->getCallsStar("DENVER");
      static const std::string arr[] = {  "CLEVELAND", "BOSTON" };
      LIST_OF_PROC_NAMES expected(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(expected == actual);
      actual = m_callsTable->getCallsStar("BOSTON");
      LIST_OF_PROC_NAMES emptyVector;
      Assert::IsTrue(emptyVector == actual);
    }
  };
}