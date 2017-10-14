#include "stdafx.h"
#include "CppUnitTest.h"

#include "PkbWriteOnly.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestPkbPatternAPI) {
public:

  TEST_METHOD(getAllAssignStmtByExactPattern) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *) pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *) pkb;
    std::list<STMT_NUM> result, expected;
    std::string varName = "varName";

    // simple expression
    pkbWrite->insertAssignStmt(1, varName, { "x", "+", "y" });
    expected = { 1 };
    result = pkbRead->getAllAssignStmtByExactPattern({ "x", "+", "y" });
    Assert::IsTrue(result == expected);

    pkbWrite->insertAssignStmt(2, varName, { "x", "-", "y" });
    pkbWrite->insertAssignStmt(3, varName, { "x", "+", "y" });

    // more assignment statements, pattern with extra whitespaces
    expected = { 1 , 3 };
    result = pkbRead->getAllAssignStmtByExactPattern({ "  x  ", "   +  ", "  y" });
    Assert::IsTrue(result == expected);

    // multi-char strings for variables, extra whitespaces, larger integer for statement number
    pkbWrite->insertAssignStmt(500, varName, { "xMan", "+", "chicken" });
    expected = { 500 };
    result = pkbRead->getAllAssignStmtByExactPattern({ "  xMan  \t\t", "\t+", "chicken" });
    Assert::IsTrue(result == expected);

    // should not match subtree
    expected = {};
    std::vector<std::string> pattern = { "xMan" };
    result = pkbRead->getAllAssignStmtByExactPattern(pattern);
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllAssignStmtBySubtreePattern) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    std::list<STMT_NUM> result, expected;
    std::string varName = "varName";

    pkbWrite->insertAssignStmt(1, varName, { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmt(2, varName, { "a", "*", "b", "+", "x", "*", "y" });
    pkbWrite->insertAssignStmt(3, varName, { "x", "*", "y", "*", "b" });
    expected = { 1, 2, 3 };
    result = pkbRead->getAllAssignStmtBySubtreePattern({ "x", "*", "y" });
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllAssignStmtByVar) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    LIST_OF_STMT_NUMS result, expected;

    pkb->insertVar("x");
    pkb->insertVar("y");
    pkbWrite->insertAssignStmt(1, "x", { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmt(2, "y", { "a", "*", "b", "+", "x", "*", "y" });
    pkbWrite->insertAssignStmt(3, "x", { "x", "*", "y", "*", "b" });
    expected = { 1, 3 };
    result = pkbRead->getAllAssignStmtByVar("x");
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllAssignStmtByVarAndExactPattern) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    std::list<STMT_NUM> result, expected;

    pkb->insertVar("xMan");
    pkb->insertVar("y");
    pkbWrite->insertAssignStmt(1, "xMan", { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmt(2, "y", { "a", "*", "b", "+", "x", "*", "y" });
    pkbWrite->insertAssignStmt(3, "xMan", { "x", "*", "y", "*", "b" });

    expected = { 1 };
    result = pkbRead->getAllAssignStmtByVarAndExactPattern("xMan", { "x", "*", "y", "+", "a", "+", "b" });
    Assert::IsTrue(result == expected);

    expected = {};
    result = pkbRead->getAllAssignStmtByVarAndExactPattern("xMan", { "x", "*", "y" });
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllAssignStmtByVarAndSubtreePattern) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    std::list<STMT_NUM> result, expected;

    pkb->insertVar("x");
    pkb->insertVar("y");
    pkbWrite->insertAssignStmt(1, "x", { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmt(2, "y", { "a", "*", "b", "+", "x", "*", "y" });
    pkbWrite->insertAssignStmt(3, "x", { "x", "*", "y", "*", "b" });
    expected = { 1, 3 };
    result = pkbRead->getAllAssignStmtByVarAndSubtreePattern("x", { "x", "*", "y" });
    Assert::IsTrue(result == expected);
  }

private:
  void printListOfIntegers(std::list<int> list) {
    for (auto iterator : list) {
      Logger::WriteMessage(std::to_string((int)iterator).c_str());
    }
  }

  };
}