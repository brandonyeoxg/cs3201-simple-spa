#include "stdafx.h"
#include "CppUnitTest.h"
#include "PatternMatch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestPatternMatch) {
public:
  TEST_METHOD(getSubtreeStringsWithTokens_patternLogic) {
    std::vector<std::string> tokens, subtrees, expected;

    // Test simple expression with only plus operator
    tokens = { "x", "+", "y" };
    expected = { "x+y", "x", "y" };
    subtrees = PatternMatch::getInstance().getSubtreeStringsWithStmtTokens(tokens);
    Assert::IsTrue(subtrees == expected);

    // Longer expression with only plus operator
    tokens = { "x", "+", "y", "+", "z" };
    expected = { "x+y+z", "x+y", "x", "y", "z" };
    subtrees = PatternMatch::getInstance().getSubtreeStringsWithStmtTokens(tokens);
    Assert::IsTrue(subtrees == expected);

    // Long expression with plus and minus
    tokens = { "x", "+", "y", "+", "z", "-", "a" };
    expected = { "x+y+z-a", "x+y+z", "x+y", "x", "y", "z", "a" };
    subtrees = PatternMatch::getInstance().getSubtreeStringsWithStmtTokens(tokens);
    Assert::IsTrue(subtrees == expected);

    // Test plus and multiply
    tokens = { "x", "+", "y", "*", "z" };
    expected = { "x+y*z", "x", "y*z", "y", "z" };
    subtrees = PatternMatch::getInstance().getSubtreeStringsWithStmtTokens(tokens);
    Assert::IsTrue(subtrees == expected);

    // Test plus and multiply, with constants
    tokens = {"x", "+", "y", "*", "5", "+", "10"};
    expected = { "x+y*5+10", "x+y*5", "x", "y*5", "y", "5", "10" };
    subtrees = PatternMatch::getInstance().getSubtreeStringsWithStmtTokens(tokens);
    Assert::IsTrue(subtrees == expected);

  }

  TEST_METHOD(getSubtreeStringsWithTokens_testWithDuplicates) {
    std::vector<std::string> tokens, subtrees, expected;

    // Test with trailing duplicates, each variable should not appear more than once
    tokens = { "x", "+", "x", "+", "x" };
    expected = { "x+x+x", "x+x", "x" };
    subtrees = PatternMatch::getInstance().getSubtreeStringsWithStmtTokens(tokens);
    Assert::IsTrue(subtrees == expected);
  }

  TEST_METHOD(getSubtreeStringsWithTokens_testWithMultiCharVariables) {
    std::vector<std::string> tokens, subtrees, expected;

    // Test with variables longer than 1 char
    tokens = { "chicken", "+", "peanut", "-", "duck" };
    expected = { "chicken+peanut-duck", "chicken+peanut", "chicken", "peanut", "duck" };
    subtrees = PatternMatch::getInstance().getSubtreeStringsWithStmtTokens(tokens);
    Assert::IsTrue(subtrees == expected);

    // Ensure variables with same name but different case are included
    tokens = { "chicken", "+", "CHICKEN", "-", "Chicken" };
    expected = { "chicken+CHICKEN-Chicken", "chicken+CHICKEN", "chicken", "CHICKEN", "Chicken" };
    subtrees = PatternMatch::getInstance().getSubtreeStringsWithStmtTokens(tokens);
    Assert::IsTrue(subtrees == expected);
  }

private:

  void printVectorOfStrings(std::vector<std::string> t_vector) {
    for (int i = 0; i < (int)t_vector.size(); i++) {
      Logger::WriteMessage(t_vector.at(i).c_str());
    }
  }

  };
}