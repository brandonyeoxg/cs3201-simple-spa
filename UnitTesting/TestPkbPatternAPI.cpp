#include "stdafx.h"
#include "CppUnitTest.h"

#include "PkbWriteOnly.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestPkbPatternAPI) {
public:

  TEST_METHOD(insertAndExtractStmts_byExactPattern) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *) pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *) pkb;
    std::list<STMT_NUM> result, expected;

    // simple expression
    pkbWrite->insertAssignStmtPattern(1, { "x", "+", "y" });
    expected = { 1 };
    result = pkbRead->getAllAssignStmtByExactPattern("x+y");
    Assert::IsTrue(result == expected);

    pkbWrite->insertAssignStmtPattern(2, { "x", "-", "y" });
    pkbWrite->insertAssignStmtPattern(3, { "x", "+", "y" });

    // more assignment statements, pattern with extra whitespaces
    expected = { 1 , 3 };
    result = pkbRead->getAllAssignStmtByExactPattern("   x  +  y  ");
    Assert::IsTrue(result == expected);

    // multi-char strings for variables, extra whitespaces, larger integer for statement number
    pkbWrite->insertAssignStmtPattern(500, { "xMan", "+", "chicken" });
    expected = { 500 };
    result = pkbRead->getAllAssignStmtByExactPattern("   xMan  +  chicken  ");
    Assert::IsTrue(result == expected);

    // should not match subtree
    expected = {};
    result = pkbRead->getAllAssignStmtByExactPattern("   xMan  ");
    Assert::IsTrue(result == expected);

  }

  TEST_METHOD(insertAndExtractStmts_bySubtreePattern) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    std::list<STMT_NUM> result, expected;

    pkbWrite->insertAssignStmtPattern(1, { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmtPattern(2, { "a", "*", "b", "+", "x", "*", "y" });
    pkbWrite->insertAssignStmtPattern(3, { "x", "*", "y", "*", "b" });
    expected = { 1, 2, 3 };
    result = pkbRead->getAllAssignStmtBySubtreePattern(" x *  y   ");
    Assert::IsTrue(result == expected);

  }

  //TODO will need testing after insertAssignStmt done, with AST and nodes decoupled from PKB
  TEST_METHOD(insertAndExtractStmts_byVarAndExactPattern) {
    //PKB * pkb = new PKB();
    //PkbWriteOnly * pkbWrite = pkb;
    //PkbReadOnly * pkbRead = pkb;
    //std::list<STMT_NUM> result, expected;

    //pkbWrite->insertAssignStmtPattern(1, { "x", "+", "y" });
    //pkbWrite->insertModifiedVariable("x", 1, {});
    //pkbWrite->populateAssignTableAbstractions();

    //result = pkbRead->getAllAssignStmtByVarAndExactPattern("x", "x+y");

    //printListOfIntegers(result);

    //expected = { 1 };

    //Assert::IsTrue(result == expected);

  }

private:
  void printListOfIntegers(std::list<int> list) {
    for (auto iterator = list.begin(); iterator != list.end(); iterator++) {
      Logger::WriteMessage(std::to_string((int)*iterator).c_str());
    }
  }

  };
}