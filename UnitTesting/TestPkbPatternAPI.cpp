#include "stdafx.h"
#include "CppUnitTest.h"

#include "PkbWriteOnly.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestPkbPatternAPI) {
public:

  TEST_METHOD(insertAndExtractStmts_byExactPattern) {
    PkbWriteOnly * pkbWrite = new PKB();
    PkbReadOnly * pkbRead = new PKB();
    std::list<STMT_NUM> result, expected;

    // simple expression
    pkbWrite->insertAssignStmt(1, { "x", "+", "y" });
    expected = { 1 };
    result = pkbRead->getAllAssignStmtByExactPattern("x+y");
    Assert::IsTrue(result == expected);

    pkbWrite->insertAssignStmt(2, { "x", "-", "y" });
    pkbWrite->insertAssignStmt(3, { "x", "+", "y" });

    // more assignment statements, pattern with extra whitespaces
    expected = { 1 , 3 };
    result = pkbRead->getAllAssignStmtByExactPattern("   x  +  y  ");
    Assert::IsTrue(result == expected);

    // multi-char strings for variables, extra whitespaces, larger integer for statement number
    pkbWrite->insertAssignStmt(500, { "xMan", "+", "chicken" });
    expected = { 500 };
    result = pkbRead->getAllAssignStmtByExactPattern("   xMan  +  chicken  ");
    Assert::IsTrue(result == expected);

    // should not match subtree
    expected = {};
    result = pkbRead->getAllAssignStmtByExactPattern("   xMan  ");
    Assert::IsTrue(result == expected);

    PatternMatch::resetInstance();
  }

  TEST_METHOD(insertAndExtractStmts_bySubtreePattern) {
    PkbWriteOnly * pkbWrite = new PKB();
    PkbReadOnly * pkbRead = new PKB();
    std::list<STMT_NUM> result, expected;

    pkbWrite->insertAssignStmt(1, { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmt(2, { "a", "*", "b", "+", "x", "*", "y" });
    pkbWrite->insertAssignStmt(3, { "x", "*", "y", "*", "b" });
    expected = { 1, 2, 3 };
    result = pkbRead->getAllAssignStmtBySubtreePattern(" x *  y   ");
    Assert::IsTrue(result == expected);

    PatternMatch::resetInstance();
  }

  //TODO will need testing after PKB singleton pattern done, and AST and nodes decoupled from PKB
  TEST_METHOD(insertAndExtractStmts_byVarAndExactPattern) {
    //PKB * pkb = new PKB();
    //PkbWriteOnly * pkbWrite = pkb;
    //PkbReadOnly * pkbRead = pkb;
    //std::list<STMT_NUM> result, expected;

    //pkbWrite->insertAssignStmt(1, { "x", "+", "y" });
    //pkbWrite->insertModifiedVariable("x", 1, {});
    //pkbWrite->populateAssignTableAbstractions();

    //result = pkbRead->getAllAssignStmtByVarAndExactPattern("x", "x+y");

    //printListOfIntegers(result);

    //expected = { 1 };

    //Assert::IsTrue(result == expected);

    //PatternMatch::resetInstance();
  }

private:
  void printListOfIntegers(std::list<int> list) {
    for (auto iterator = list.begin(); iterator != list.end(); iterator++) {
      Logger::WriteMessage(std::to_string((int)*iterator).c_str());
    }
  }

  };
}