#include <cstdio>
#include <iostream>
#include <fstream>
#include <functional>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "test-drivers\ParserDriver.h"
#include "pkb/PKB.h"
#include "SyntaxErrorException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
  TEST_CLASS(TestParser)
  {
  public:
    PKB* m_pkb;
    ParserDriver* m_parser;
    
    TEST_METHOD_INITIALIZE(instantiateParser) {
      m_pkb = new PKB();
      m_parser = new ParserDriver(m_pkb);
    }

    TEST_METHOD_CLEANUP(cleanupParser) {
      delete m_pkb;
      delete m_parser;
    }

    TEST_METHOD(parseValidProgram)
    {
      int actual = m_parser->parse("../AutoTester/simple-prog.txt");
      int expected = 0;
      Assert::AreEqual(expected, actual);

      actual = m_parser->parse("randomTXT.txt");
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

    TEST_METHOD(testIsValidName) 
    {
      Assert::IsTrue(m_parser->testIsValidName(std::string("x")));
      Assert::IsTrue(m_parser->testIsValidName(std::string("x1")));
      Assert::IsTrue(m_parser->testIsValidName(std::string("variable")));
      Assert::IsTrue(m_parser->testIsValidName(std::string("variable1is2good3v")));
      Assert::IsFalse(m_parser->testIsValidName(std::string("1")));
      Assert::IsFalse(m_parser->testIsValidName(std::string("1x")));
    }

    TEST_METHOD(testIsConstant) 
    {
      Assert::IsTrue(m_parser->testIsConstant(std::string("4")));
      Assert::IsTrue(m_parser->testIsConstant(std::string("40709")));
      Assert::IsFalse(m_parser->testIsConstant(std::string("c")));
      Assert::IsFalse(m_parser->testIsConstant(std::string("89c")));
    }

    void assertParseOnProgramExpectException(const std::string &t_programString) {
      // Lamda capture list only captures this pointer and non static local variable....
      // Cannot use m_pkb and m_parser.
      PKB pkb;
      Parser* parser = new Parser(&pkb);
      std::ofstream tmpFile;
      const std::string tmpFileName = "testSimple.txt";
      tmpFile.open(tmpFileName);
      tmpFile << t_programString;
      tmpFile.close();
      auto funcPtr = [parser, tmpFileName] { parser->parse(tmpFileName); };
      Assert::ExpectException<SyntaxErrorException>(funcPtr);
    }
  };
}