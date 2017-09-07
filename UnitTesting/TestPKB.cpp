#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"
#include "ParentTable.h"
#include "VarTable.h"
#include "PKB.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  public:
    std::unordered_map<int, std::vector<int>> test = {
      { 1,{ 2, 3 } },
      { 2,{ 3, 4 } },
      { 3,{ 4 } }
    };
    

    TEST_METHOD(TestReturnVarTable) {
      Logger::WriteMessage("Running return parent table test...");
      std::unordered_map<std::string, std::vector<int>> testVar = {
        { "x",{ 1, 2 } },
        { "y",{ 2, 3 } },
        { "z",{ 4 } }
      };
      PKB *pkb = new PKB();
      VarTable *varTable = pkb->getVarTable();
      varTable->setVarTable(testVar);
      std::unordered_map<std::string, std::vector<int>> actual = {
        { "x",{ 1, 2 } }
      };
      std::unordered_map<std::string, std::vector<int>> expected = pkb->returnVarTable("x");
      Assert::IsTrue(expected == actual);
    }

  };

}