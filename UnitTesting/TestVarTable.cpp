#include "stdafx.h"
#include "CppUnitTest.h"
#include "VarTable.h"

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
  };
  
  
}