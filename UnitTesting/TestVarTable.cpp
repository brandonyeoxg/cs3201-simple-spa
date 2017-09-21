#include "stdafx.h"
#include "CppUnitTest.h"
#include "VarTable.h"
#include "VarRelations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  public:
    
    TEST_METHOD(TestInsertUses) {
      Logger::WriteMessage("testing insert uses");
      VarTable testVarTable;

      //test insertUses (correct behaviour).
      int ans = testVarTable.insertUsesForStmt("x", 2);
      ans = testVarTable.insertUsesForStmt("x", 3);
      std::unordered_map<int, VarRelations> actual = testVarTable.getVarTable();
      VarRelations expected = actual[0];
      std::vector<int> expectedVector = expected.getUses();
      Assert::IsTrue(2 == expectedVector[0]);
      Assert::IsTrue(3 == expectedVector[1]);
    }

    TEST_METHOD(TestInsertModifies) {
      Logger::WriteMessage("testing insertModifies");
      VarTable testVarTable;

      //test insertModifies (correct behaviour).
      int ans = testVarTable.insertModifiesForStmt("y", 2);
      std::unordered_map<int, VarRelations> actual = testVarTable.getVarTable();
      VarRelations expected = actual[0];
      std::vector<int> expectedVector = expected.getModifies();
      Assert::IsTrue(2 == expectedVector[0]);
      
      //test insertModifies(add another line to same variable).
      ans = testVarTable.insertModifiesForStmt("y", 3);
      actual = testVarTable.getVarTable();
      expected = actual[0];
      expectedVector = expected.getModifies();
      Assert::IsTrue(3 == expectedVector[1]);

    }

    TEST_METHOD(TestIsModifies) {
      Logger::WriteMessage("testing isModifies");
      VarTable testVarTable;
      int ans = testVarTable.insertModifiesForStmt("x", 1);
      ans = testVarTable.insertModifiesForStmt("y", 2);
      ans = testVarTable.insertModifiesForStmt("y", 3);

      Assert::IsTrue(testVarTable.isModifies(2, "y"));
      Assert::IsTrue(testVarTable.isModifies(3, "y"));
      Assert::IsFalse(testVarTable.isModifies(2, "x"));
    }

    TEST_METHOD(TestIsUses) {
      Logger::WriteMessage("testing isUses");
      VarTable testVarTable;
      int ans = testVarTable.insertUsesForStmt("x", 1);
      ans = testVarTable.insertUsesForStmt("y", 2);
      ans = testVarTable.insertUsesForStmt("y", 3);

      Assert::IsTrue(testVarTable.isUses(2, "y"));
      Assert::IsTrue(testVarTable.isUses(3, "y"));
      Assert::IsFalse(testVarTable.isUses(2, "x"));
    }

    TEST_METHOD(TestGetModifies) {
      Logger::WriteMessage("testing getModifies");
      VarTable testVarTable;
      int ans = testVarTable.insertModifiesForStmt("x", 1);
      ans = testVarTable.insertModifiesForStmt("y", 2);
      ans = testVarTable.insertModifiesForStmt("y", 3);
      std::vector<std::string> actual;
      actual.push_back("y");
      std::vector<std::string> expected = testVarTable.getModifies(2);
      Assert::IsTrue("y" == expected[0]);

      //test for non-existent relationship. expects empty vector.
      expected = testVarTable.getModifies(4);
      Assert::IsTrue(expected.size() == 0);
    }

    TEST_METHOD(TestGetUses) {
      Logger::WriteMessage("testing getUses");
      VarTable testVarTable;
      int ans = testVarTable.insertUsesForStmt("x", 1);
      ans = testVarTable.insertUsesForStmt("y", 2);
      ans = testVarTable.insertUsesForStmt("y", 3);
      std::vector<std::string> actual;
      actual.push_back("y");
      std::vector<std::string> expected = testVarTable.getUses(2);
      Assert::IsTrue("y" == expected[0]);

      //test for non-existent relationship. expects empty vector.
      expected = testVarTable.getUses(4);
      Assert::IsTrue(expected.size() == 0);
    }

    TEST_METHOD(TestGetStmtModifies) {
      Logger::WriteMessage("testing getStmtModifies");
      VarTable testVarTable;
      int ans = testVarTable.insertModifiesForStmt("x", 1);
      ans = testVarTable.insertModifiesForStmt("y", 2);
      ans = testVarTable.insertModifiesForStmt("y", 3);
      std::vector<int> actual;
      actual.push_back(2);
      actual.push_back(3);
      std::vector<int> expected = testVarTable.getStmtModifies("y");
      Assert::IsTrue(actual == expected);

      //test for non-existent relationship. expects empty vector.
      expected = testVarTable.getStmtModifies("z");
      Assert::IsTrue(expected.size() == 0);
    }

    TEST_METHOD(TestGetStmtUses) {
      Logger::WriteMessage("testing getStmtUses");
      VarTable testVarTable;
      int ans = testVarTable.insertUsesForStmt("x", 1);
      ans = testVarTable.insertUsesForStmt("y", 2);
      ans = testVarTable.insertUsesForStmt("y", 3);
      std::vector<int> actual;
      actual.push_back(2);
      actual.push_back(3);
      std::vector<int> expected = testVarTable.getStmtUses("y");
      Assert::IsTrue(actual == expected);

      //test for non-existent relationship. expects empty vector.
      expected = testVarTable.getStmtUses("z");
      Assert::IsTrue(expected.size() == 0);
    }

    TEST_METHOD(TestGetAllStmtModifies) {
      std::unordered_map<std::string, std::vector<int>> expected = {
        {"x", {1}},
        {"y", {2, 3}}
      };
      Logger::WriteMessage("testing getAllStmtModifies");
      VarTable testVarTable;
      int ans = testVarTable.insertModifiesForStmt("x", 1);
      ans = testVarTable.insertModifiesForStmt("y", 2);
      ans = testVarTable.insertModifiesForStmt("y", 3);

      std::unordered_map<std::string, std::vector<int>> actual = testVarTable.getAllStmtModifies();
      Assert::IsTrue(actual == expected);
    }

    TEST_METHOD(TestGetAllStmtUses) {
      std::unordered_map<std::string, std::vector<int>> expected = {
        { "x",{ 1 } },
        { "y",{ 2, 3 } }
      };
      Logger::WriteMessage("testing getAllStmtUses");
      VarTable testVarTable;
      int ans = testVarTable.insertUsesForStmt("x", 1);
      ans = testVarTable.insertUsesForStmt("y", 2);
      ans = testVarTable.insertUsesForStmt("y", 3);

      std::unordered_map<std::string, std::vector<int>> actual = testVarTable.getAllStmtUses();
      Assert::IsTrue(actual == expected);
    }

    TEST_METHOD(TestGetIndexOfVar) {
      Logger::WriteMessage("testing getIndexOfVar");
      VarTable testVarTable;
      int ans = testVarTable.insertUsesForStmt("x", 1);
      ans = testVarTable.insertUsesForStmt("y", 2);
      ans = testVarTable.insertUsesForStmt("y", 3);

      int expected = testVarTable.getIndexOfVar("y");
      Assert::IsTrue(expected == 1);

      // PC this is not needed since we dont want to throw exception.
      //test getIndexOfVar (non-existent varName).
      //bool exceptionThrown = false;
      //try {
      //  expected = testVarTable.getIndexOfVar("z");
      //} catch (std::invalid_argument) {
      //  Logger::WriteMessage("Exception thrown in getIndexOfVar");
      //  exceptionThrown = true;
      //}
      //Assert::IsTrue(exceptionThrown);
    }

    TEST_METHOD(TestGetVarNameFromIndex) {
      Logger::WriteMessage("testing getIndexOfVar");
      VarTable testVarTable;
      int ans = testVarTable.insertUsesForStmt("x", 1);
      ans = testVarTable.insertUsesForStmt("y", 2);
      ans = testVarTable.insertUsesForStmt("y", 3);

      std::string expected = testVarTable.getVarNameFromIndex(1);
      Assert::IsTrue("y" == expected);
    }

    TEST_METHOD(TestIsModifiesAnything) {
      Logger::WriteMessage("testing getIndexOfVar");
      VarTable testVarTable;
      int ans = testVarTable.insertModifiesForStmt("x", 1);
      ans = testVarTable.insertModifiesForStmt("y", 2);
      ans = testVarTable.insertModifiesForStmt("y", 3);

      Assert::IsTrue(testVarTable.isModifiesAnything(2));
      Assert::IsTrue(testVarTable.isModifiesAnything(3));
      Assert::IsFalse(testVarTable.isModifiesAnything(4));
    }

    TEST_METHOD(TestGetStmtModifiesAnything) {
      Logger::WriteMessage("testing getStmtModifiesAnything");
      VarTable testVarTable;
      int ans = testVarTable.insertModifiesForStmt("x", 1);
      ans = testVarTable.insertModifiesForStmt("y", 2);
      ans = testVarTable.insertModifiesForStmt("y", 3);
      ans = testVarTable.insertModifiesForStmt("x", 3);

      std::vector<int> expected = testVarTable.getStmtModifiesAnything();
      static const int arr[] = { 1, 2, 3 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));

      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestGetStmtUsesAnything) {
      Logger::WriteMessage("testing getStmtUsesAnything");
      VarTable testVarTable;
      int ans = testVarTable.insertUsesForStmt("x", 1);
      ans = testVarTable.insertUsesForStmt("y", 2);
      ans = testVarTable.insertUsesForStmt("x", 3);
      ans = testVarTable.insertUsesForStmt("y", 3);
      ans = testVarTable.insertUsesForStmt("z", 3);
      std::vector<int> expected = testVarTable.getStmtUsesAnything();
      static const int arr[] = { 1, 2, 3 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));

      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestGetAllVariables) {
      Logger::WriteMessage("testing getAllVariables");
      VarTable testVarTable;
      int ans = testVarTable.insertUsesForStmt("x", 1);
      ans = testVarTable.insertUsesForStmt("y", 2);
      ans = testVarTable.insertUsesForStmt("x", 3);
      ans = testVarTable.insertUsesForStmt("y", 3);
      ans = testVarTable.insertUsesForStmt("z", 3);

      std::vector<std::string> expected = testVarTable.getAllVariables();
      static const std::string arr[] = { "x", "y", "z" };
      std::vector<std::string> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));

      Assert::IsTrue(expected == actual);
    }
  };
}