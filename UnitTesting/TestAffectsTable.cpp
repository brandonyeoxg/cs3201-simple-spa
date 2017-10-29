#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include <sstream>

#include "Parser.h"
#include "PKB.h"
#include "AffectsTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestAffectsTable) {
  public:
    PKB *m_pkb;
    AffectsTable *m_affectsTable;
    std::string m_tmpFileName;
    std::ofstream m_tmpFile;

    TEST_METHOD_INITIALIZE(initTestAffectsTable) {
      m_pkb = new PKB();
      Parser m_testParser(m_pkb);
      m_affectsTable = new AffectsTable(m_pkb);
      std::stringstream testProgramStream;
      /*
      procedure Llama{
        while i{
          if j then{
            a=a+1;
            call Alpaca;
            b=c+2;
            c=78;}
          else{
            d=f-e;
            f=d*e;}}
        call Pajama;
      }
      procedure Alpaca{
        call Obama;
        emperor=emperor-new*groove;
      }
      procedure Pajama{
        President=orange;
        call Alapaca;
      }
      procedure Obama{
        Bush=9*11;
      }
      */
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
      << "call Alapaca;\n"
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
    }

    TEST_METHOD_CLEANUP(cleanupTestAffectsTable) {
      delete m_pkb;
      delete m_affectsTable;
    }

    TEST_METHOD(TestgetAffectsListsFromBounds) {
      m_affectsTable->getAffectsListsFromBounds(1, 6);
    }
  private:
  };
}