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
	public:
    TEST_METHOD_INITIALIZE(InitialisePkbandParser) 
    {
      m_pkb = new PKB();
      m_parser = new ParserDriver(m_pkb);
      std::ofstream tmpFile;
      m_tmpFileName = "testSimple.txt";
    }

    TEST_METHOD_CLEANUP(CleanupPkbAndParser) 
    {
      m_tmpFile.close();
      std::remove(m_tmpFileName.c_str());
      delete m_pkb;
      delete m_parser;
    }

		TEST_METHOD(TestParserAndPKBProcedure)
		{
      // Test for the procedure recorded properly
      editSimpleProgramFile("procedure main {\n");
      m_parser->parseProcedure();

      ProcTable* procTable = m_pkb->getProcTable();
      LIST_OF_PROC_NAMES actual = procTable->getAllProcsName();

      Assert::AreEqual(actual.size(), size_t(1));
      Assert::AreEqual(*actual.begin(), std::string("main"));
    }

    TEST_METHOD(TestParserAndPKBFollows) // This only tests follows relation
    {
      LIST_OF_STMT_NUMS dummyStmtList;
      editSimpleProgramFile("procedure main {\n x=y;}\n");
      m_parser->parseProcedure();
      m_parser->parseStmt(dummyStmtList);

      // Follow table should be empty
      FollowTable* followTable = m_pkb->getFollowTable();
      LIST_OF_STMT_NUMS actual = followTable->getFollowsAnything();
      Assert::AreEqual(actual.size(), size_t(0));
      delete m_parser;

      m_parser = new ParserDriver(m_pkb);
      editSimpleProgramFile("x=y;\ny=x;\n");
      m_parser->parseStmt(dummyStmtList);
      dummyStmtList.push_back(1);
      m_parser->parseStmt(dummyStmtList);

      // Follow table should be populated with 1 follows relation
      actual = followTable->getFollowsAnything();
      Assert::AreEqual(actual.size(), size_t(1));
      Assert::IsTrue(m_pkb->isFollows(1, 2));
      Assert::IsFalse(m_pkb->isFollows(0, 1));
    }

    TEST_METHOD(TestParserAndPKBParent)
    {
      editSimpleProgramFile("procedure main {\n x=y;}\n");
      LIST_OF_STMT_NUMS dummyStmtList;
      m_parser->parseProcedure();
      m_parser->parseStmt(dummyStmtList);

      ParentTable* parentTable = m_pkb->getParentTable();
      LIST_OF_STMT_NUMS actual = parentTable->getParentOfAnything();
      Assert::AreEqual(actual.size(), size_t(0));

      editSimpleProgramFile("while i { \nx=y;}");
      m_parser->parseStmt(dummyStmtList);
      actual = parentTable->getParentOfAnything();
      Assert::AreEqual(actual.size(), size_t(1));

      editSimpleProgramFile("while i {\n while j { \n x=y;}}");
      m_parser->parseStmt(dummyStmtList);
      actual = parentTable->getParentOfAnything();
      Assert::AreEqual(actual.size(), size_t(3));
      actual = parentTable->getChildrenOfAnything();
      Assert::AreEqual(actual.size(), size_t(3));
    }

    TEST_METHOD(TestParserAndPKBModifiesP)
    {
      editSimpleProgramFile("procedure main {\n x=y;}");
      m_parser->parseProcedure();
      
    }

    void editSimpleProgramFile(std::string t_editText) {
      m_tmpFile.open(m_tmpFileName);
      m_tmpFile << t_editText;
      m_tmpFile.close();
      m_parser->openFileStream(m_tmpFileName);
    }
	};
}