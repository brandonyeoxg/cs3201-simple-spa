#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb/relationshipTables/ProcTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestProcTable) {
  public:
    ProcTable* m_procTable;

    TEST_METHOD_INITIALIZE(initialiseProcTable) 
    {
      m_procTable = new ProcTable();
      m_procTable->insertProc("first");
      m_procTable->insertProc("second");
    }

    TEST_METHOD_CLEANUP(cleanupProcTable)
    {
      delete m_procTable;
    }

    TEST_METHOD(TestInsertIntoProcTable)
    {
      int expected = 2;
      int actual = m_procTable->getAllProcsName().size();
      Assert::IsTrue(actual == expected);

      LIST_OF_PROC_NAMES actualList = m_procTable->getAllProcsName();
      LIST_OF_PROC_NAMES expectedList = { "first", "second" };
      Assert::IsTrue(actualList == expectedList);

      int actualIdx = m_procTable->insertProc("third");
      Assert::IsTrue(actualIdx == 2);

      actual = m_procTable->getAllProcsName().size();
      expected = 3;
      Assert::IsTrue(actual == expected);

      actualList = m_procTable->getAllProcsName();
      expectedList.push_back("third");
      Assert::IsTrue(actualList == expectedList);
    }

    TEST_METHOD(TestGetProcIdxWithName)
    {
      int actualIdx = m_procTable->getProcIdxFromName("first");
      Assert::AreEqual(actualIdx, 0);
      actualIdx = m_procTable->getProcIdxFromName("totallyAProcInTheProgram");
      Assert::AreEqual(actualIdx, INVALID_INDEX);
    }

    TEST_METHOD(TestGetProcNameWithIdx) 
    {
      std::string actualName = m_procTable->getProcNameFromIdx(0);
      std::string expectedName = "first";
      Assert::IsTrue(actualName == expectedName);

      actualName = m_procTable->getProcNameFromIdx(1000);
      Assert::IsTrue(actualName == "");
    }

  };
}