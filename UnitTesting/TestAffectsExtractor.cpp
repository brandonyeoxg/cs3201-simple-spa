#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include <sstream>

#include "Parser.h"
#include "AffectsExtractor.h"
#include "pkb/PKB.h"
#include "pkb/PkbTablesOnly.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestAffectsExtractor) {
  public:
    AffectsExtractor* m_affectsExtractor;
    PKB* m_pkb;
    std::string m_tmpFileName;
    std::ofstream m_tmpFile;

    TEST_METHOD_INITIALIZE(initTestAffectsExtractor) {
      m_pkb = new PKB();
      Parser m_testParser(m_pkb);
      DesignExtractor de;
      m_affectsExtractor = new AffectsExtractor(m_pkb);
      std::stringstream testProgramStream;
      testProgramStream << "procedure Llama{ \n"
        << "while i{ \n"
        << "if j then{ \n"
        << "a = a + 1; \n"
        << "call Alpaca; \n"
        << "b = c + 2;\n"
        << "c = 78; } \n"
        << "else {\n"
        << "d = f - e;\n"
        << "f = d*e;\n"
        << "} } \n"
        << "call Pajama;\n"
        << "}\n"
        << "procedure Alpaca{\n"
        << "call Obama;\n"
        << "emperor = emperor - new*groove;\n"
        << "}\n"
        << "procedure Pajama{ \n"
        << "President = orange;\n"
        << "}\n"
        << "procedure Obama{\n"
        << "Bush = 9 * 11;\n"
        << "}\n";

      std::string testProgram = testProgramStream.str();
      m_tmpFileName = "testSimple.txt";
      m_tmpFile.open(m_tmpFileName);
      m_tmpFile << testProgram;
      m_tmpFile.close();

      m_testParser.parse(m_tmpFileName);
      m_pkb->getNextTable()->executeAfterAllNextInserts();

      std::remove(m_tmpFileName.c_str());
      de.extractRestOfDesignAbstractions(m_pkb);
    }

    TEST_METHOD_CLEANUP(cleanupTestAffectsExtractor) {
      delete m_pkb;
      delete m_affectsExtractor;
    }
    TEST_METHOD(TestExtractAffects) {
      LIST_OF_AFFECTS_STMTS expected = { 6 };
      LIST_OF_AFFECTS_STMTS actual = m_affectsExtractor->extractAffects(5);
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestExtractAffectedBy) {
      LIST_OF_AFFECTS_STMTS expected = { 5 };
      LIST_OF_AFFECTS_STMTS actual = m_affectsExtractor->extractAffectedBy(6);
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestExtractIsAffects) {
      Assert::IsTrue(m_affectsExtractor->extractIsAffects(3, 3));
      //line number inserted exceeds the number of statements.
      Assert::IsFalse(m_affectsExtractor->extractIsAffects(3, 15));
      //there exist at least one statement where it's not an assignment stmt.
      Assert::IsFalse(m_affectsExtractor->extractIsAffects(3, 1));
      //2 statements are not in same proc
      Assert::IsFalse(m_affectsExtractor->extractIsAffects(3, 1));
    }

    TEST_METHOD(TestExtractAffectsAnything) {
      LIST_OF_AFFECTS_STMTS expected = { 3, 6, 7, 8 };
      LIST_OF_AFFECTS_STMTS actual = m_affectsExtractor->extractAffectsAnything();
      std::sort(actual.begin(), actual.end());
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestExtractAffectedByAnything) {
      LIST_OF_AFFECTS_STMTS expected = { 3, 5, 7, 8 };
      LIST_OF_AFFECTS_STMTS actual = m_affectsExtractor->extractAffectedByAnything();
      std::sort(actual.begin(), actual.end());
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestExtractIsAffectsAnything) {
      Assert::IsTrue(m_affectsExtractor->extractIsAffectsAnything(3));
      //line number inserted exceeds the number of statements.
      
    }
    TEST_METHOD(TestExtractIsAffectedByAnything) {
      Assert::IsFalse(m_affectsExtractor->extractIsAffectedByAnything(10));
      //line number inserted exceeds the number of statements.

    }

    TEST_METHOD(TestExtractAllAffects) {
      MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS expected = {
        { 3,{ 3 } },
        { 6,{ 5 } },
        { 7,{ 8 } },
        { 8,{ 7 } }
      };
      MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS actual = m_affectsExtractor->extractAllAffects();  
      Assert::IsTrue(expected == actual);
    }
  };
}