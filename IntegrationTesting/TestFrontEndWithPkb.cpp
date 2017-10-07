#include "stdafx.h"
#include "CppUnitTest.h"
#include "test-drivers/ParserDriver.h"
#include "PkbWriteOnly.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
	TEST_CLASS(TestFrontEndWithPkb)
	{
  private:
    PKB* m_pkb;
    ParserDriver* m_parser;
    std::string m_tmpFileName;
    std::ofstream m_tmpFile;
    ASTBuilder m_builder;
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
      //// Test for the procedure recorded properly
      //m_tmpFile << "procedure main {\n";
      //m_tmpFile.close();
      //m_parser->openFileStream(m_tmpFileName);
      //m_parser->parseProcedure();

      //ProcTable* procTable = m_pkb->getProcTable();
      //std::list<std::string> actual = procTable->getAllProcNameInProgram();

      //Assert::AreEqual(actual.size(), size_t(1));
      //Assert::AreEqual(*actual.begin(), std::string("main"));
    }

    TEST_METHOD(TestParserAndPKBStmt) // This only tests follows relation
    {
      m_tmpFile << "procedure main {x=y;}\n";
      m_tmpFile.close();
      m_parser->openFileStream(m_tmpFileName);
      //StmtListNode* stmtListNode = m_builder.createStmtList(0);
      LIST_OF_STMT_NUMS stmtList;
      m_parser->parseStmt(stmtList);

      // Follow table should be empty
      FollowTable* followTable = m_pkb->getFollowTable();
      std::vector<int> actual = followTable->getFollowsAnything();
      Assert::AreEqual(actual.size(), size_t(0));

      // Parent table should be empty
      ParentTable* parentTable = m_pkb->getParentTable();
      actual = parentTable->getParentOfAnything();
      Assert::AreEqual(actual.size(), size_t(0));

      delete m_parser;
      m_parser = new ParserDriver(m_pkb);
      m_tmpFile.open(m_tmpFileName);
      m_tmpFile << "x=y;\ny=x;\n";
      m_tmpFile.close();
      m_parser->openFileStream(m_tmpFileName);
      m_parser->parseStmt(stmtList);
      stmtList.push_back(1);
      m_parser->parseStmt(stmtList);

      // Follow table should be populated with 1 follows relation
      actual = followTable->getFollowsAnything();
      Assert::AreEqual(actual.size(), size_t(1));
      Assert::IsTrue(m_pkb->isFollows(1, 2));
      Assert::IsFalse(m_pkb->isFollows(0, 1));
    }
	};
}