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
    }
    TEST_METHOD(TestInsertCalls) {
      m_callsTable->insertCalls("ATLANTA", "BOSTON");
      m_callsTable->insertCalls("CLEVELAND", "BOSTON");
      m_callsTable->insertCalls("DENVER", "CLEVELAND");
      m_callsTable->insertCalls("ATLANTA", "CLEVELAND");
      Assert::IsTrue(m_callsTable->getCallsMap() == m_testCallsMapResult);
      Assert::IsTrue(m_callsTable->getCalledByMap() == m_testCalledByMapResult);
    }
  };
}