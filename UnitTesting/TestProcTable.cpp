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

    TEST_METHOD(getProcIdxNumWithName)
    {
      ASTBuilder builder;
      std::string dummyProcName = "dummy";
      ProcedureNode* dummyProc = builder.createProcedure(dummyProcName);
      ProcTable procTable;
      PROC_INDEX_NO index = procTable.insertProcByProcNode(dummyProc);

      PROC_INDEX_NO actual = procTable.getProcIdxNumWithName(std::string("dummy"));

      Assert::AreEqual(actual, (PROC_INDEX_NO)0);

      dummyProc = builder.createProcedure(dummyProcName + "1");
      index = procTable.insertProcByProcNode(dummyProc);

      actual = procTable.getProcIdxNumWithName(std::string("dummy1"));

      Assert::AreEqual(actual, (PROC_INDEX_NO)1);
    }
  };
}