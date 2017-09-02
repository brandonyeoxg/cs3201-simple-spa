#include <cstdio>
#include <iostream>
#include <fstream>
#include <functional>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "SyntaxErrorException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
  TEST_CLASS(TestParser)
  {
  public:

    TEST_METHOD(parseValidProgram)
    {
      PKB pkb;
      Parser parser(&pkb);
      int actual = parser.parse("../AutoTester/simple-prog.txt");
      int expected = 0;
      Assert::AreEqual(expected, actual);

      actual = parser.parse("randomTXT.txt");
      Assert::AreNotEqual(expected, actual);
    }

    TEST_METHOD(parseProgramWithNoOpenBraceThrowException) 
    {
      assertParseOnProgramExpectException("procedure test } \n");
    }

    TEST_METHOD(parseProgramWithNoSemiColonThrowException)
    {
      assertParseOnProgramExpectException("procedure test {x=y} \n");
    }

    TEST_METHOD(parseProgramWithNoOperatorThrowException)
    {
      assertParseOnProgramExpectException("procedure test {xy;} \n");
    }

    TEST_METHOD(parseProgramWithIncorrectCommandThrowException)
    {
      assertParseOnProgramExpectException("procedure test{x=y;whi i {z=y;}} \n");
    }

    TEST_METHOD(parseProgramWithNoCloseBraceThrowException)
    {
      assertParseOnProgramExpectException("procedure test{x=y; \n");
      assertParseOnProgramExpectException("procedure test{x=y; \n\n");
    }

    void assertParseOnProgramExpectException(const std::string &t_programString) {
      PKB pkb;
      Parser *parser = new Parser(&pkb);
      ofstream tmpFile;
      const std::string tmpFileName = "testSimple.txt";
      tmpFile.open(tmpFileName);
      tmpFile << t_programString;
      tmpFile.close();
      auto funcPtr = [parser, tmpFileName] { parser->parse(tmpFileName); };
    }
  };
}