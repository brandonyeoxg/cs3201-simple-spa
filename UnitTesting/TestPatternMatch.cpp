#include "stdafx.h"
#include "CppUnitTest.h"
#include "PatternMatch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestPatternMatch) {
public:
  TEST_METHOD(getSubtreeStringsWithTokens) {
    std::vector<std::string> tokens = { "x", "+", "y" };

    //TODO add asserts for all these

    tokens = { "x", "+", "y", "+", "z" };

    tokens = { "x", "+", "y", "+", "z", "-", "a" };

    tokens = {"x", "+", "y", "*", "5", "+", "10"};

    tokens = { "x", "+", "y", "*", "z" };

    std::vector<std::string> subtrees = PatternMatch::getInstance().getSubtreeStringsWithStmtTokens(tokens);

    printVectorOfStrings(subtrees);
  }

private:

  void printVectorOfStrings(std::vector<std::string> t_vector) {
    for (int i = 0; i < (int)t_vector.size(); i++) {
      Logger::WriteMessage(t_vector.at(i).c_str());
    }
  }

  };
}