#include "stdafx.h"
#include "CppUnitTest.h"
#include "TokeniserUtil.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestAssignTable) {
  public:
    TEST_METHOD(TestIsKeyDelimiter) {
        Assert::IsTrue(TokeniserUtil::isKeyDelimiter(";"));
        Assert::IsFalse(TokeniserUtil::isKeyDelimiter(""));
        Assert::IsTrue(TokeniserUtil::isKeyDelimiter(" "));
    }

    TEST_METHOD(TestIsOperator) {
        Assert::IsTrue(TokeniserUtil::isOperator("+"));
        Assert::IsTrue(TokeniserUtil::isOperator("-"));
        Assert::IsTrue(TokeniserUtil::isOperator("*"));
        Assert::IsFalse(TokeniserUtil::isOperator("/"));
        Assert::IsFalse(TokeniserUtil::isOperator("\\"));
    }

    TEST_METHOD(TestIsBracket)
    {
        Assert::IsTrue(TokeniserUtil::isBracket("("));
        Assert::IsTrue(TokeniserUtil::isBracket(")"));
        Assert::IsFalse(TokeniserUtil::isBracket("-"));
    }

    TEST_METHOD(TestTokeniseLine)
    {
        std::string input = "Hello = X";
        std::vector<std::string> expected = { "Hello", "=", "X" };
        assertTokeniseLine(input, expected);

        input = "Hello =X";
        assertTokeniseLine(input, expected);

        input = "Hello= X";
        assertTokeniseLine(input, expected);
      
        input = "Hello=X";
        assertTokeniseLine(input, expected);

        input = "Hello";
        expected = { "Hello" };
        assertTokeniseLine(input, expected);

        input = "Hello X + =";
        expected = { "Hello", "X", "+", "=" };
        assertTokeniseLine(input, expected);
    }

    void assertTokeniseLine(std::string t_input, std::vector<std::string> t_expected) {
      std::vector<std::string> actual = TokeniserUtil::tokeniseLine(t_input);
      Assert::IsTrue(actual == t_expected);
    }
  };
}