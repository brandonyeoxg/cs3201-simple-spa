#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include <cstdio>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
  TEST_CLASS(TestParser)
  {
  public:

    TEST_METHOD(parse)
    {
      PKB pkb;
      Parser parser(&pkb);
      int actual = parser.parse("../AutoTester/simple-prog.txt");
      int expected = 0;
      Assert::AreEqual(expected, actual);

      actual = parser.parse("randomTXT.txt");
      Assert::AreNotEqual(expected, actual);
    }
  };
}