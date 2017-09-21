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
      PROC_INDEX index = procTable.insertProcByProcNode(dummyProc);

      Assert::AreEqual(index, (PROC_INDEX)0);

      dummyProc = builder.createProcedure(dummyProcName + "1");
      index = procTable.insertProcByProcNode(dummyProc);
      Assert::AreEqual(index, (PROC_INDEX)1);
    }

    TEST_METHOD(getProcIdxNumWithName)
    {
      ASTBuilder builder;
      std::string dummyProcName = "dummy";
      ProcedureNode* dummyProc = builder.createProcedure(dummyProcName);
      ProcTable procTable;
      PROC_INDEX index = procTable.insertProcByProcNode(dummyProc);

      PROC_INDEX actual = procTable.getProcIdxNumWithName(std::string("dummy"));

      Assert::AreEqual(actual, (PROC_INDEX)0);

      dummyProc = builder.createProcedure(dummyProcName + "1");
      index = procTable.insertProcByProcNode(dummyProc);

      actual = procTable.getProcIdxNumWithName(std::string("dummy1"));

      Assert::AreEqual(actual, (PROC_INDEX)1);
    }

    TEST_METHOD(isModifies) 
    {
      ASTBuilder builder;
      std::string dummyProcName = "dummy";
      ProcedureNode* dummyProc = builder.createProcedure(dummyProcName);
      ProcTable procTable;
      PROC_INDEX index = procTable.insertProcByProcNode(dummyProc);
      
      procTable.insertModifies(index, std::string("HELLO"));
      procTable.convertProcTableSetToList();

      Assert::IsTrue(procTable.isModifies(dummyProcName, std::string("HELLO")));
      Assert::IsFalse(procTable.isModifies(dummyProcName, std::string("BYEBYE")));
    }

    TEST_METHOD(getVarFromProcModifies)
    {
      ASTBuilder builder;
      std::string dummyProcName = "dummy";
      ProcedureNode* dummyProc = builder.createProcedure(dummyProcName);
      ProcTable procTable;
      PROC_INDEX index = procTable.insertProcByProcNode(dummyProc);
      std::string varName1 = "HELLO";
      std::string varName2 = "BYE";
      procTable.insertModifies(index, varName1);
      procTable.insertModifies(index, varName2);

      procTable.convertProcTableSetToList();

      std::list<std::string> actual = procTable.getVarFromProcModifies(index);
      std::list<std::string> expected = {varName2, varName1};

      Assert::IsTrue(actual == expected);
    }

    TEST_METHOD(getProcNameThatModifiesVar) 
    {
      ASTBuilder builder;
      std::string dummyProcName = "dummy";
      ProcedureNode* dummyProc = builder.createProcedure(dummyProcName);
      ProcTable procTable;
      PROC_INDEX index = procTable.insertProcByProcNode(dummyProc);
      std::string varName1 = "HELLO";
      std::string varName2 = "BYE";

      procTable.insertModifies(index, varName1);
      procTable.insertModifies(index, varName2);

      dummyProcName = dummyProcName + "1";
      dummyProc = builder.createProcedure(dummyProcName);
      index = procTable.insertProcByProcNode(dummyProc);
      procTable.insertModifies(index, varName2);
      procTable.convertProcTableSetToList();

      std::list<std::string> expected = { "dummy" };
      std::list<std::string> actual = procTable.getProcNameThatModifiesVar(varName1);
      Assert::IsTrue(expected == actual);
      actual = procTable.getProcNameThatModifiesVar(varName2);
      expected = { "dummy", "dummy1" };
      Assert::IsTrue(expected == actual);
    }


    TEST_METHOD(isUses)
    {
      ASTBuilder builder;
      std::string dummyProcName = "dummy";
      ProcedureNode* dummyProc = builder.createProcedure(dummyProcName);
      ProcTable procTable;
      PROC_INDEX index = procTable.insertProcByProcNode(dummyProc);

      procTable.insertUses(index, std::string("HELLO"));
      procTable.convertProcTableSetToList();

      Assert::IsTrue(procTable.isUses(dummyProcName, std::string("HELLO")));
      Assert::IsFalse(procTable.isUses(dummyProcName, std::string("BYEBYE")));
    }

    TEST_METHOD(getVarFromProcUses)
    {
      ASTBuilder builder;
      std::string dummyProcName = "dummy";
      ProcedureNode* dummyProc = builder.createProcedure(dummyProcName);
      ProcTable procTable;
      PROC_INDEX index = procTable.insertProcByProcNode(dummyProc);
      std::string varName1 = "HELLO";
      std::string varName2 = "BYE";
      procTable.insertUses(index, varName1);
      procTable.insertUses(index, varName2);

      procTable.convertProcTableSetToList();

      std::list<std::string> actual = procTable.getVarFromProcUses(index);
      std::list<std::string> expected = { varName2, varName1 };

      Assert::IsTrue(actual == expected);
    }
  };
}