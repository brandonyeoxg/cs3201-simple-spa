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
      Logger::WriteMessage("testing insert uses");
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

      int index = 1;
      VarTable testVarTable;

      //test insertModifies (correct behaviour).
      int ans = testVarTable.insertModifiesForStmt(index, "y", 2);
      std::unordered_map<int, VarRelations> actual = testVarTable.getVarTable();
      VarRelations expected = actual[1];
      std::vector<int> expectedVector = expected.getModifies();
      Assert::IsTrue(2 == expectedVector[0]);
      
      //test insertModifies(add another line to same variable).
      ans = testVarTable.insertModifiesForStmt(index, "y", 3);
      actual = testVarTable.getVarTable();
      expected = actual[1];
      expectedVector = expected.getModifies();
      Assert::IsTrue(3 == expectedVector[1]);
      
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

    TEST_METHOD(TestIsModifies) {
      Logger::WriteMessage("testing isModifies");
      int index = 1;
      VarTable testVarTable;
      int ans = testVarTable.insertModifiesForStmt(index, "x", 1);
      index++;
      ans = testVarTable.insertModifiesForStmt(index, "y", 2);
      index++;
      ans = testVarTable.insertModifiesForStmt(index, "y", 3);

      Assert::IsTrue(testVarTable.isModifies(2, "y"));
      Assert::IsTrue(testVarTable.isModifies(3, "y"));
      Assert::IsFalse(testVarTable.isModifies(2, "x"));
    }

    TEST_METHOD(TestIsUses) {
      Logger::WriteMessage("testing isUses");
      int index = 1;
      VarTable testVarTable;
      int ans = testVarTable.insertUsesForStmt(index, "x", 1);
      index++;
      ans = testVarTable.insertUsesForStmt(index, "y", 2);
      index++;
      ans = testVarTable.insertUsesForStmt(index, "y", 3);

      Assert::IsTrue(testVarTable.isUses(2, "y"));
      Assert::IsTrue(testVarTable.isUses(3, "y"));
      Assert::IsFalse(testVarTable.isUses(2, "x"));
    }

    TEST_METHOD(TestGetModifies) {
      Logger::WriteMessage("testing getModifies");
      int index = 1;
      VarTable testVarTable;
      int ans = testVarTable.insertModifiesForStmt(index, "x", 1);
      index++;
      ans = testVarTable.insertModifiesForStmt(index, "y", 2);
      index++;
      ans = testVarTable.insertModifiesForStmt(index, "y", 3);
      std::vector<std::string> actual;
      actual.push_back("y");
      std::vector<std::string> expected = testVarTable.getModifies(2);
      Assert::IsTrue("y" == expected[0]);

      //test for non-existent relationship. expects empty vector.
      expected = testVarTable.getModifies(4);
      Assert::IsTrue(expected.size() == 0);
    }

  };
}