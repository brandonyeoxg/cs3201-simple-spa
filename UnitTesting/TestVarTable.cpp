#include "stdafx.h"
#include "CppUnitTest.h"
#include "VarTable.h"
#include "VarRelations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  public:
    /*
    TEST_METHOD(TestInsertVar) {
      std::unordered_map<std::string, std::vector<int>> testVar = {
        { "x",{ 1, 2 } },
        { "y",{ 2, 3 } },
        { "z",{ 4 } }
      };
      VarTable* testVarTable = new VarTable();
      testVarTable->setVarTable(testVar);
      std::unordered_map<std::string, std::vector<int>> testInsertResult = {
        { "x",{ 1, 2 } },
        { "y",{ 2, 3, 4 } },
        { "z",{ 4 } }
      };

      std::unordered_map<std::string, std::vector<int>> testInsertResultNew = {
        { "x",{ 1, 2 } },
        { "y",{ 2, 3, 4 } },
        { "z",{ 4 } },
        { "w",{ 5 } }
      };
      
      testVarTable = testVarTable->insert(testVarTable, "y", 4);
      Assert::IsTrue(testVarTable->getVarTable() == testInsertResult);
      testVarTable = testVarTable->insert(testVarTable, "w", 5);
      Assert::IsTrue(testVarTable->getVarTable() == testInsertResultNew);
    }

    TEST_METHOD(TestGetVar) {
      std::unordered_map<std::string, std::vector<int>> testVar = {
        { "x",{ 1, 2 } },
        { "y",{ 2, 3 } },
        { "z",{ 4 } }
      };
      VarTable* testVarTable = new VarTable();
      testVarTable->setVarTable(testVar);
      static const int arr[] = { 1, 2 };
      std::vector<int> expected(arr, arr + sizeof(arr) / sizeof(arr[0]));

      std::vector<int> actual = testVarTable->get("x");
      Assert::IsTrue(actual == expected);
    }
    */
    TEST_METHOD(TestInsertUses) {
      VarRelations var;
      var.insertUses(2);
      var.setVarName("x");
      int index = 1;
      VarTable testVarTable;

      //test insertUses (correct behaviour).
      int ans = testVarTable.insertUsesForStmt(index, "x", 2);
      std::unordered_map<int, VarRelations> actual = testVarTable.getVarTable();
      VarRelations expected = actual[1];
      std::vector<int> expectedVector = expected.getUses();
      Assert::IsTrue(2 == expectedVector[0]);
      index++;
      
      //test insertUses (duplicate entry, expect exception).
      bool exceptionThrown = false;
      try {
        int expected = testVarTable.insertUsesForStmt(index, "x", 2);
      } catch (std::invalid_argument) {
        Logger::WriteMessage("Exception thrown in insertUses");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown); 
    }

    TEST_METHOD(TestInsertModifies) {
      Logger::WriteMessage("testing insertModifies");
      VarRelations var;
      var.insertModifies(2);
      var.setVarName("y");

      int index = 1;
      VarTable testVarTable;

      //test insertModifies (correct behaviour).
      int ans = testVarTable.insertModifiesForStmt(index, "y", 2);
      std::unordered_map<int, VarRelations> actual = testVarTable.getVarTable();
      VarRelations expected = actual[1];
      std::vector<int> expectedVector = expected.getModifies();
      Assert::IsTrue(2 == expectedVector[0]);
      
      //test insertModifies (duplicate entry, expect exception).
      bool exceptionThrown = false;
      try {
        int expected = testVarTable.insertModifiesForStmt(index, "y", 2);
      } catch (std::invalid_argument) {
        Logger::WriteMessage("Exception thrown in insertModifies");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown);
    }

  };
  
  
}