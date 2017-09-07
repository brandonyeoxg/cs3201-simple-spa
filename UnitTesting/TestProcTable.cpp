#include "stdafx.h"
#include "CppUnitTest.h"
#include "ProcTable.h"
#include "..\source\ASTBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestProcTable) {
  public:
    
    TEST_METHOD(insertIntoProcTable)
    {
      ASTBuilder builder;
      std::string dummyProcName = "dummy";
      ProcedureNode* dummyProc = builder.createProcedure(dummyProcName);
      ProcTable procTable;
      PROC_INDEX_NO index = procTable.insertProcByProcNode(dummyProc);

      Assert::AreEqual(index, (PROC_INDEX_NO)0);

      dummyProc = builder.createProcedure(dummyProcName + "1");
      index = procTable.insertProcByProcNode(dummyProc);
      Assert::AreEqual(index, (PROC_INDEX_NO)1);

    }

    TEST_METHOD(getProcWithIndex)
    {
      ASTBuilder builder;
      std::string dummyProcName = "dummy";
      ProcTable procTable;

      ProcedureNode* actualProcNode = procTable.getProcWithIndex(0);
      Assert::IsNull(actualProcNode);

      ProcedureNode* dummyProc = builder.createProcedure(dummyProcName);
      procTable.insertProcByProcNode(dummyProc);
      actualProcNode = procTable.getProcWithIndex(0);
      Assert::IsTrue(dummyProc == actualProcNode);

      dummyProc = builder.createProcedure(dummyProcName + "1");
      procTable.insertProcByProcNode(dummyProc);
      actualProcNode = procTable.getProcWithIndex(1);
      Assert::IsTrue(dummyProc == actualProcNode);
    }
  };
}