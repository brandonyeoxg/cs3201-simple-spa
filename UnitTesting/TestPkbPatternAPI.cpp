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
    pkbWrite->insertAssignStmt(1, { "x", "+", "y" });

    PkbReadOnly * pkbRead = new PKB();
    std::list<STMT_NUM> result, expected;

    // simple expression
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

private:
  void printListOfIntegers(std::list<int> list) {
    for (auto iterator = list.begin(); iterator != list.end(); iterator++) {
      Logger::WriteMessage(std::to_string((int)*iterator).c_str());
    }
  }

  };
}