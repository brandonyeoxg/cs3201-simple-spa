#include "stdafx.h"
#include "CppUnitTest.h"
#include "PatternMatch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestPatternMatch) {
public:
  TEST_METHOD(isExactPatternInStmt) {
    PatternMatch patternMatch = PatternMatch();
    std::vector<std::string> tokens, pattern;
    std::string result;
    int index = 1;

    tokens = { "x" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "x" };
    Assert::IsTrue(patternMatch.isExactPatternInStmt(index, pattern));

    // Expression with only plus operator
    index++;
    tokens = { "x", "+", "y" };
    pattern = {};
    patternMatch.addAssignStmt(index, tokens);
    Assert::IsFalse(patternMatch.isExactPatternInStmt(index, pattern));
    pattern = { "x", "+", "y" };
    Assert::IsTrue(patternMatch.isExactPatternInStmt(index, pattern));

    // Longer expression with only plus operator
    index++;
    tokens = { "x", "+", "y", "+", "z" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "x", "+", "y" };
    Assert::IsFalse(patternMatch.isExactPatternInStmt(index, pattern));
    pattern = { "x", "+", "y", "+", "z" };
    Assert::IsTrue(patternMatch.isExactPatternInStmt(index, pattern));

    // Long expression with plus and minus
    index++;
    tokens = { "x", "+", "y", "+", "z", "-", "a" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "x", "+", "y", "+", "z", "-", "a" };
    Assert::IsTrue(patternMatch.isExactPatternInStmt(index, pattern));

    // Test plus and multiply
    index++;
    tokens = { "x", "+", "y", "*", "z" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "x", "+", "y", "*", "z" };
    Assert::IsTrue(patternMatch.isExactPatternInStmt(index, pattern));
    pattern = { "x", "+", "y" };
    Assert::IsFalse(patternMatch.isExactPatternInStmt(index, pattern));

    // Test plus and multiply, with constants
    index++;
    tokens = { "x", "+", "y", "*", "5", "+", "10" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "x", "+", "y", "*", "5", "+", "10" };
    Assert::IsTrue(patternMatch.isExactPatternInStmt(index, pattern));
  }

  TEST_METHOD(isSubtreePatternInStmt) {
    PatternMatch patternMatch = PatternMatch();
    std::vector<std::string> tokens, pattern;
    std::string result;
    int index = 1;

    // Expression with only plus operator
    tokens = { "x", "+", "y" };
    pattern = {};
    patternMatch.addAssignStmt(index, tokens);
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "x", "+", "y" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "x" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "y" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));

    // Longer expression with only plus operator
    index++;
    tokens = { "x", "+", "y", "+", "z" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "x", "+", "y" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "x", "+", "y", "+", "z" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "y", "+", "z" };
    Assert::IsFalse(patternMatch.isSubtreePatternInStmt(index, pattern));

    // Long expression with plus, minus and multiply
    index++;
    tokens = { "x", "+", "y", "*", "z", "-", "a" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "x", "+", "y", "*", "z", "-", "a" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "y", "*", "z" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "x", "+", "y", "*", "z" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "z" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "y", "*", "z", "-", "a" };
    Assert::IsFalse(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "z", "-", "a" };
    Assert::IsFalse(patternMatch.isSubtreePatternInStmt(index, pattern));
  }

  TEST_METHOD(isExactPatternInStmt_isSubtreePatternInStmt_01) {
    PatternMatch patternMatch = PatternMatch();
    std::vector<std::string> tokens, pattern;
    std::string result;
    int index = 1;

    // multi-char variable names
    tokens = { "chicken", "+", "peanut", "-", "duck" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "chicken", "+", "peanut", "-", "duck" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    Assert::IsTrue(patternMatch.isExactPatternInStmt(index, pattern));
    pattern = { "chicken", "+", "peanut" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    Assert::IsFalse(patternMatch.isExactPatternInStmt(index, pattern));

    // variable names with different cases
    index++;
    tokens = { "chicken", "+", "CHICKEN", "-", "Chicken" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "chicken", "+", "CHICKEN", "-", "Chicken" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    Assert::IsTrue(patternMatch.isExactPatternInStmt(index, pattern));
    pattern = { "chicken" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    Assert::IsFalse(patternMatch.isExactPatternInStmt(index, pattern));
    pattern = { "CHICKEN" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    pattern = { "ChiCKEN" };
    Assert::IsFalse(patternMatch.isSubtreePatternInStmt(index, pattern));
  }

  TEST_METHOD(isExactPatternInStmt_isSubtreePatternInStmt_02) {
    PatternMatch patternMatch = PatternMatch();
    std::vector<std::string> tokens, pattern;
    std::string result;
    int index = 1;

    // trailing whitespaces
    tokens = { "   chicken  ", "  +  ", "  peanut  ", "-  ", "  duck" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "chicken", "+", "peanut   ", "-", "duck" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    Assert::IsTrue(patternMatch.isExactPatternInStmt(index, pattern));
    pattern = { "  chicken  ", "+", "peanut  " };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    Assert::IsFalse(patternMatch.isExactPatternInStmt(index, pattern));

    // tabs
    index++;
    tokens = { "chicken  \t  ", "  + ", "\t\tpeanut\t", "-", "duck" };
    patternMatch.addAssignStmt(index, tokens);
    pattern = { "chicken\t\t\t", "+", "peanut", "-", "duck" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    Assert::IsTrue(patternMatch.isExactPatternInStmt(index, pattern));
    pattern = { " \t  chicken   \t\t", "+\t\t", "peanut" };
    Assert::IsTrue(patternMatch.isSubtreePatternInStmt(index, pattern));
    Assert::IsFalse(patternMatch.isExactPatternInStmt(index, pattern));
  }

  TEST_METHOD(getAllStmtNumWithExactPattern_01) {
    PatternMatch patternMatch = PatternMatch();
    std::list<STMT_NUM> stmtNums, expected;
    std::vector<std::string> pattern;

    patternMatch.addAssignStmt(1, { "x", "+", "y" });
    patternMatch.addAssignStmt(2, { "x" });
    patternMatch.addAssignStmt(3, { "x", "+", "y" });
    pattern = { "x", "+", "y" };
    stmtNums = patternMatch.getAllStmtNumWithExactPattern(pattern);
    expected = { 1, 3 };
    Assert::IsTrue(stmtNums == expected);

    patternMatch.addAssignStmt(40, { "xMan", "-", "sugar" });
    patternMatch.addAssignStmt(50, { "xMan", "+", "sugar" });
    pattern = { "xMan", "-", "sugar" };
    stmtNums = patternMatch.getAllStmtNumWithExactPattern(pattern);
    expected = { 40 };
    Assert::IsTrue(stmtNums == expected);

    pattern = { "xMan" };
    expected = {};
    stmtNums = patternMatch.getAllStmtNumWithExactPattern(pattern);
    Assert::IsTrue(stmtNums == expected);
  }

  TEST_METHOD(getAllStmtNumWithExactPattern_02) {
    // test with trailing whitespaces, longer variable names
    PatternMatch patternMatch = PatternMatch();
    std::list<STMT_NUM> stmtNums, expected;
    std::vector<std::string> pattern;

    patternMatch.addAssignStmt(1, { " \t chicken  ", "+", "peanut", "-", "duck" });
    patternMatch.addAssignStmt(2, { "chicken", "  +   ", "peanut \t  ", "-", "duck" });
    pattern = { "chicken", "+", "peanut", "-", "duck" };
    stmtNums = patternMatch.getAllStmtNumWithExactPattern(pattern);
    expected = { 1, 2 };
    Assert::IsTrue(stmtNums == expected);
  }

  TEST_METHOD(getAllStmtNumWithExactPattern_03) {
    PatternMatch patternMatch = PatternMatch();
    std::list<STMT_NUM> stmtNums, expected;
    std::vector<std::string> pattern;

    patternMatch.addAssignStmt(1, { "(", "(", "(", "x", "+", "y", ")", ")", ")" });
    patternMatch.addAssignStmt(2, { "x" });
    patternMatch.addAssignStmt(3, { "x", "+", "y" });
    pattern = { "x", "+", "y" };
    stmtNums = patternMatch.getAllStmtNumWithExactPattern(pattern);
    expected = { 1, 3 };
    Assert::IsTrue(stmtNums == expected);

    pattern = { "(", "x", "+", "y", ")" };
    stmtNums = patternMatch.getAllStmtNumWithExactPattern(pattern);
    expected = { 1, 3 };
    Assert::IsTrue(stmtNums == expected);
  }

  TEST_METHOD(getAllStmtNumWithSubtreePattern_01) {
    PatternMatch patternMatch = PatternMatch();
    std::list<STMT_NUM> stmtNums, expected;
    std::vector<std::string> pattern;

    patternMatch.addAssignStmt(1, { "x", "+", "y", "*", "a" });
    patternMatch.addAssignStmt(2, { "(", "x", "+", "y", ")", "*", "a" });
    pattern = { "x", "+", "y" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 2 };
    Assert::IsTrue(stmtNums == expected);

    pattern = { "y", "*", "a" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 1 };
    Assert::IsTrue(stmtNums == expected);

    patternMatch.addAssignStmt(3, { "(", "(", "(", "x", "+", "y", ")", ")", ")", "*", "a" });
    pattern = { "x", "+", "y" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 2, 3 };
    Assert::IsTrue(stmtNums == expected);

    pattern = { "x" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 1, 2, 3 };
    Assert::IsTrue(stmtNums == expected);
  }

  TEST_METHOD(getAllStmtNumWithSubtreePattern_02) {
    PatternMatch patternMatch = PatternMatch();
    std::list<STMT_NUM> stmtNums, expected;
    std::vector<std::string> pattern;

    patternMatch.addAssignStmt(1, { "x", "-", "b", "*", "(", "x", "+", "y", ")", "*", "a" });
    patternMatch.addAssignStmt(2, { "x", "-", "(", "b", "*", "x", "+", "y", ")", "*", "a" });
    pattern = { "x", "+", "y" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 1 };
    Assert::IsTrue(stmtNums == expected);

    pattern = { "b", "*", "x", "+", "y" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 2 };
    Assert::IsTrue(stmtNums == expected);

    pattern = { "b", "*", "(", "x", "+", "y", ")" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 1 };
    Assert::IsTrue(stmtNums == expected);

    pattern = { "(", "b", "*", "x", "+", "y", ")", "*", "a" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 2 };
    Assert::IsTrue(stmtNums == expected);
  }

  TEST_METHOD(getAllStmtNumWithSubtreePattern_03) {
    PatternMatch patternMatch = PatternMatch();
    std::list<STMT_NUM> stmtNums, expected;
    std::vector<std::string> pattern;

    patternMatch.addAssignStmt(1, { "(", "x", "+", "(", "(", "x", "+", "(", "x", "+", "y", ")",
      ")", "+", "y", ")" });
    patternMatch.addAssignStmt(2, { "(", "x", ")" });
    patternMatch.addAssignStmt(3, { "x", "+", "y" });
    pattern = { "x", "+", "y" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 1, 3 };
    Assert::IsTrue(stmtNums == expected);

    pattern = { "x", "+", "x", "+", "y" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = {};
    Assert::IsTrue(stmtNums == expected);

    pattern = { "x", "+", "(", "x", "+", "y", ")" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 1 };
    Assert::IsTrue(stmtNums == expected);

    pattern = { "(", "(","x", "+", "(", "x", "+", "y", ")", ")", ")" };
    stmtNums = patternMatch.getAllStmtNumWithSubtreePattern(pattern);
    expected = { 1 };
    Assert::IsTrue(stmtNums == expected);
  }

private:

  void printVectorOfStrings(std::vector<std::string> t_vector) {
    for (int i = 0; i < (int)t_vector.size(); i++) {
      Logger::WriteMessage(t_vector.at(i).c_str());
    }
  }

  void printListOfIntegers(std::list<int> list) {
    for (auto iterator = list.begin(); iterator != list.end(); iterator++) {
      Logger::WriteMessage(std::to_string((int)*iterator).c_str());
    }
  }

  };
}