#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParserDriver.h"
#include "PkbWriteOnly.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
	TEST_CLASS(UnitTest1)
	{
  private:
    PKB* m_pkb;
    ParserDriver* m_parser;
    std::string m_tmpFileName;
    std::ofstream m_tmpFile;
	public:
    TEST_METHOD_INITIALIZE(InitialisePkbandParser) 
    {
      m_pkb = new PKB();
      m_parser = new ParserDriver(m_pkb);
      std::ofstream tmpFile;
      m_tmpFileName = "testSimple.txt";
      m_tmpFile.open(m_tmpFileName);
    }

    TEST_METHOD_CLEANUP(CleanupPkbAndParser) 
    {
      std::remove(m_tmpFileName.c_str());
    }

		TEST_METHOD(TestParserAndPKBProcedure)
		{
      // Test for the procedure recorded properly
      m_tmpFile << "procedure main {\n";
      m_tmpFile.close();
      m_parser->openFileStream(m_tmpFileName);
      m_parser->parseProcedure();

      ProcTable* procTable = m_pkb->getProcTable();
      std::list<std::string> actual = procTable->getAllProcNameInProgram();

      Assert::AreEqual(actual.size(), size_t(1));
      Assert::AreEqual(*actual.begin(), std::string("main"));
    }

	};
}