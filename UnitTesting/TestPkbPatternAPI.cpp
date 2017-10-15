#include "stdafx.h"
#include "CppUnitTest.h"

#include "PkbWriteOnly.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/** This file tests pattern matching API in PKB.h.
    Ensures PKB integrated with PatternMatch works correctly.
    @jazlyn
*/

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
    std::string varNameA = "varNameA";
    std::string varNameB = "varNameB";

    pkb->insertVar(varNameA);
    pkb->insertVar(varNameB);
    pkbWrite->insertAssignStmt(1, varNameA, { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmt(2, varNameB, { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmt(3, varNameA, { "x", "*", "y", "*", "b" });

    expected = { 1 };
    result = pkbRead->getAllAssignStmtByVarAndExactPattern(varNameA, { "x", "*", "y", "+", "a", "+", "b" });
    Assert::IsTrue(result == expected);

    expected = { 2 };
    result = pkbRead->getAllAssignStmtByVarAndExactPattern(varNameB, { "x", "*", "y", "+", "a", "+", "b" });
    Assert::IsTrue(result == expected);

    expected = {};
    result = pkbRead->getAllAssignStmtByVarAndExactPattern(varNameA, { "x", "*", "y" });
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllAssignStmtByVarAndSubtreePattern) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    std::list<STMT_NUM> result, expected;
    std::string varNameA = "varNameA";
    std::string varNameB = "varNameB";

    pkb->insertVar(varNameA);
    pkb->insertVar(varNameB);
    pkbWrite->insertAssignStmt(1, varNameA, { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmt(2, varNameB, { "a", "*", "b", "+", "x", "*", "y" });
    pkbWrite->insertAssignStmt(3, varNameA, { "x", "*", "y", "*", "b" });

    expected = { 1, 3 };
    result = pkbRead->getAllAssignStmtByVarAndSubtreePattern(varNameA, { "x", "*", "y" });
    Assert::IsTrue(result == expected);

    expected = { 2 };
    result = pkbRead->getAllAssignStmtByVarAndSubtreePattern(varNameB, { "x", "*", "y" });
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllAssignStmtWithVarByExactPattern) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    std::unordered_map<STMT_NUM, VAR_NAME> result, expected;
    std::string varNameA = "varNameA";
    std::string varNameB = "varNameB";

    pkbWrite->insertModifies(0, varNameA, {}, 1);
    pkbWrite->insertModifies(0, varNameA, {}, 2);
    pkbWrite->insertModifies(0, varNameA, {}, 3);
    pkbWrite->insertModifies(0, varNameB, {}, 4);
    pkbWrite->insertAssignStmt(1, varNameA, { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmt(2, varNameA, { "a", "*", "b", "+", "x", "*", "y" });
    pkbWrite->insertAssignStmt(3, varNameA, { "x", "*", "y", "*", "b" });
    pkbWrite->insertAssignStmt(4, varNameB, { "x", "*", "y", "+", "a", "+", "b" });
    
    expected = std::unordered_map<STMT_NUM, VAR_NAME>();
    result = pkbRead->getAllAssignStmtWithVarByExactPattern({ "x", "*", "y" });
    Assert::IsTrue(result == expected);

    expected.insert({ 1, varNameA });
    expected.insert({ 4, varNameB });
    result = pkbRead->getAllAssignStmtWithVarByExactPattern({ "x", "*", "y", "+", "a", "+", "b" });
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllAssignStmtWithVarBySubtreePattern) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    std::unordered_map<STMT_NUM, VAR_NAME> result, expected;
    std::string varNameA = "varNameA";
    std::string varNameB = "varNameB";

    pkbWrite->insertModifies(0, varNameA, {}, 1);
    pkbWrite->insertModifies(0, varNameA, {}, 2);
    pkbWrite->insertModifies(0, varNameA, {}, 3);
    pkbWrite->insertModifies(0, varNameB, {}, 4);
    pkbWrite->insertAssignStmt(1, varNameA, { "x", "*", "y", "+", "a", "+", "b" });
    pkbWrite->insertAssignStmt(2, varNameA, { "a", "*", "b", "+", "x", "*", "y" });
    pkbWrite->insertAssignStmt(3, varNameA, { "x", "*", "y", "*", "b" });
    pkbWrite->insertAssignStmt(4, varNameB, { "x", "*", "y", "+", "a", "+", "b" });

    expected = std::unordered_map<STMT_NUM, VAR_NAME>();
    expected.insert({ 1, varNameA });
    expected.insert({ 2, varNameA });
    expected.insert({ 3, varNameA });
    expected.insert({ 4, varNameB });
    result = pkbRead->getAllAssignStmtWithVarBySubtreePattern({ "x", "*", "y" });
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getWhileStmtByVar) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    LIST_OF_STMT_NUMS result, expected;
    std::string varNameA = "varNameA";
    std::string varNameB = "varNameB";

    expected = {};
    result = pkbRead->getWhileStmtByVar(varNameA);
    Assert::IsTrue(result == expected);

    pkbWrite->insertWhileStmt(0, varNameA, {}, 1);
    pkbWrite->insertWhileStmt(0, varNameA, {}, 2);
    pkbWrite->insertWhileStmt(0, varNameB, {}, 3);
    pkbWrite->insertWhileStmt(0, varNameA, {}, 4);
    pkbWrite->insertIfStmt(0, varNameB, {}, 5);
    pkbWrite->insertIfStmt(0, varNameA, {}, 6);

    expected = {1, 2, 4};
    result = pkbRead->getWhileStmtByVar(varNameA);
    Assert::IsTrue(result == expected);

    expected = { 3 };
    result = pkbRead->getWhileStmtByVar(varNameB);
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllWhileStmtsWithVar) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    std::unordered_map<STMT_NUM, VAR_NAME> result, expected;
    std::string varNameA = "varNameA";
    std::string varNameB = "varNameB";

    expected = std::unordered_map<STMT_NUM, VAR_NAME>();
    result = pkbRead->getAllWhileStmtsWithVar();
    Assert::IsTrue(result == expected);

    pkbWrite->insertWhileStmt(0, varNameA, {}, 1);
    pkbWrite->insertWhileStmt(0, varNameA, {}, 2);
    pkbWrite->insertWhileStmt(0, varNameB, {}, 3);
    pkbWrite->insertWhileStmt(0, varNameA, {}, 4);
    pkbWrite->insertIfStmt(0, varNameB, {}, 5);
    pkbWrite->insertIfStmt(0, varNameA, {}, 6);

    expected.insert({ 1, varNameA });
    expected.insert({ 2, varNameA });
    expected.insert({ 3, varNameB });
    expected.insert({ 4, varNameA });

    result = pkbRead->getAllWhileStmtsWithVar();
    Assert::IsTrue(result == expected);

    pkbWrite->insertWhileStmt(0, varNameA, {}, 500);
    expected.insert({ 500, varNameA });

    result = pkbRead->getAllWhileStmtsWithVar();
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllWhileStmts) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    LIST_OF_STMT_NUMS result, expected;
    std::string varNameA = "varNameA";
    std::string varNameB = "varNameB";

    expected = {};
    result = pkbRead->getAllWhileStmts();
    Assert::IsTrue(result == expected);

    pkbWrite->insertWhileStmt(0, varNameA, {}, 1);
    pkbWrite->insertWhileStmt(0, varNameA, {}, 2);
    pkbWrite->insertWhileStmt(0, varNameB, {}, 3);
    pkbWrite->insertWhileStmt(0, varNameA, {}, 4);
    pkbWrite->insertIfStmt(0, varNameB, {}, 5);
    pkbWrite->insertIfStmt(0, varNameA, {}, 6);

    expected = { 1, 2, 3, 4 };
    result = pkbRead->getAllWhileStmts();
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getIfStmtByVar) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    LIST_OF_STMT_NUMS result, expected;
    std::string varNameA = "varNameA";
    std::string varNameB = "varNameB";

    expected = {};
    result = pkbRead->getIfStmtByVar(varNameA);
    Assert::IsTrue(result == expected);

    pkbWrite->insertIfStmt(0, varNameA, {}, 1);
    pkbWrite->insertIfStmt(0, varNameA, {}, 2);
    pkbWrite->insertIfStmt(0, varNameB, {}, 3);
    pkbWrite->insertIfStmt(0, varNameA, {}, 4);
    pkbWrite->insertWhileStmt(0, varNameB, {}, 5);
    pkbWrite->insertWhileStmt(0, varNameA, {}, 6);
    pkbWrite->insertIfStmt(0, varNameB, {}, 7);

    expected = { 1, 2, 4 };
    result = pkbRead->getIfStmtByVar(varNameA);
    Assert::IsTrue(result == expected);

    expected = { 3, 7 };
    result = pkbRead->getIfStmtByVar(varNameB);
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllIfStmtsWithVar) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    std::unordered_map<STMT_NUM, VAR_NAME> result, expected;
    std::string varNameA = "varNameA";
    std::string varNameB = "varNameB";

    expected = std::unordered_map<STMT_NUM, VAR_NAME>();
    result = pkbRead->getAllIfStmtsWithVar();
    Assert::IsTrue(result == expected);

    pkbWrite->insertIfStmt(0, varNameA, {}, 1);
    pkbWrite->insertIfStmt(0, varNameA, {}, 2);
    pkbWrite->insertIfStmt(0, varNameB, {}, 3);
    pkbWrite->insertIfStmt(0, varNameA, {}, 4);
    pkbWrite->insertWhileStmt(0, varNameB, {}, 5);
    pkbWrite->insertWhileStmt(0, varNameA, {}, 6);

    expected.insert({ 1, varNameA });
    expected.insert({ 2, varNameA });
    expected.insert({ 3, varNameB });
    expected.insert({ 4, varNameA });

    result = pkbRead->getAllIfStmtsWithVar();
    Assert::IsTrue(result == expected);

    pkbWrite->insertIfStmt(0, varNameA, {}, 500);
    expected.insert({ 500, varNameA });

    result = pkbRead->getAllIfStmtsWithVar();
    Assert::IsTrue(result == expected);
  }

  TEST_METHOD(getAllIfStmts) {
    PKB * pkb = new PKB();
    PkbWriteOnly * pkbWrite = (PkbWriteOnly *)pkb;
    PkbReadOnly * pkbRead = (PkbReadOnly *)pkb;
    LIST_OF_STMT_NUMS result, expected;
    std::string varNameA = "varNameA";
    std::string varNameB = "varNameB";

    expected = {};
    result = pkbRead->getAllIfStmts();
    Assert::IsTrue(result == expected);

    pkbWrite->insertIfStmt(0, varNameA, {}, 1);
    pkbWrite->insertIfStmt(0, varNameA, {}, 2);
    pkbWrite->insertIfStmt(0, varNameB, {}, 3);
    pkbWrite->insertIfStmt(0, varNameA, {}, 5);

    pkbWrite->insertWhileStmt(0, varNameB, {}, 4);
    pkbWrite->insertWhileStmt(0, varNameA, {}, 6);

    expected = { 1, 2, 3, 5 };
    result = pkbRead->getAllIfStmts();
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