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
      LIST_OF_STMT_NUMS dummyStmtList, dummyProgLines;
      editSimpleProgramFile("procedure main {\n x=y;}\n");
      m_parser->parseProcedure();
      m_parser->parseStmt(dummyStmtList, dummyProgLines);

      // Follow table should be empty
      FollowTable* followTable = m_pkb->getFollowTable();
      LIST_OF_STMT_NUMS actual = followTable->getFollowsAnything();
      Assert::AreEqual(actual.size(), size_t(0));

      editSimpleProgramFile("x=y;\ny=x;\n");
      m_parser->parseStmt(dummyStmtList, dummyProgLines);
      m_parser->parseStmt(dummyStmtList, dummyProgLines);

      // Follow table should be populated with 1 follows relation
      Assert::IsTrue(m_pkb->isFollows(1, 2));
      Assert::IsTrue(m_pkb->isFollows(2, 3));
      Assert::IsFalse(m_pkb->isFollows(0, 1));
      Assert::IsFalse(m_pkb->isFollows(3, 4));
    }

    TEST_METHOD(TestParserAndPKBParent)
    {
      editSimpleProgramFile("procedure main {\n x=y;}\n");
      LIST_OF_STMT_NUMS dummyStmtList, dummyProgLines;
      m_parser->parseProcedure();
      m_parser->parseStmt(dummyStmtList, dummyProgLines);

      ParentTable* parentTable = m_pkb->getParentTable();
      LIST_OF_STMT_NUMS actual = parentTable->getParentOfAnything();
      Assert::AreEqual(actual.size(), size_t(0));

      editSimpleProgramFile("while i { \nx=y;}");
      m_parser->parseStmt(dummyStmtList, dummyProgLines);
      actual = parentTable->getParentOfAnything();
      Assert::AreEqual(actual.size(), size_t(1));

      editSimpleProgramFile("while i {\n while j { \n x=y;}}");
      m_parser->parseStmt(dummyStmtList, dummyProgLines);
      actual = parentTable->getParentOfAnything();
      Assert::AreEqual(actual.size(), size_t(3));
      actual = parentTable->getChildrenOfAnything();
      Assert::AreEqual(actual.size(), size_t(3));
    }

    TEST_METHOD(TestParserAndPKBModifiesP) 
    {
      editSimpleProgramFile("procedure main {\n x=y;\n}");
      LIST_OF_STMT_NUMS dummyStmtList, dummyProgLines;
      m_parser->parseProcedure();
      m_parser->parseStmt(dummyStmtList, dummyProgLines);

      ModifiesP* modifiesP = m_pkb->getModifiesP();
      LIST_OF_RESULTS actual = modifiesP->getAllProcNames();
      LIST_OF_RESULTS expected = { "main" };
      Assert::AreEqual(actual.size(), size_t(1));
      Assert::IsTrue(actual == expected);
      actual = modifiesP->getVarNamesWithProcIdx(0);
      expected = { "x" };
      Assert::IsTrue(actual == expected);

      editSimpleProgramFile("k =n;\n j =p;");
      m_parser->parseStmt(dummyStmtList, dummyProgLines);
      m_parser->parseStmt(dummyStmtList, dummyProgLines);
      actual = modifiesP->getAllProcNames();
      expected = { "main" };
      Assert::AreEqual(actual.size(), size_t(1));
      Assert::IsTrue(actual == expected);
      actual = modifiesP->getVarNamesWithProcIdx(0);
      expected = { "x", "k", "j" };
      Assert::AreEqual(actual.size(), size_t(3));
      Assert::IsTrue(actual == expected);
    }

    TEST_METHOD(TestParserAndPKBUsesP) 
    {
      editSimpleProgramFile("procedure main {\n x=y;\n}");
      LIST_OF_STMT_NUMS dummyStmtList, dummyProgList;
      m_parser->parseProcedure();
      m_parser->parseStmt(dummyStmtList, dummyProgList);

      UsesP* usesP = m_pkb->getUsesP();
      LIST_OF_RESULTS actual = usesP->getAllProcNames();
      LIST_OF_RESULTS expected = { "main" };
      Assert::AreEqual(actual.size(), size_t(1));

      actual = usesP->getVarNamesWithProcIdx(0);
      expected = { "y" };
      Assert::IsTrue(actual == expected);

      editSimpleProgramFile("k =n;\n j =p;");
      m_parser->parseStmt(dummyStmtList, dummyProgList);
      m_parser->parseStmt(dummyStmtList, dummyProgList);
      actual = usesP->getAllProcNames();
      expected = { "main" };
      Assert::AreEqual(actual.size(), size_t(1));
      Assert::IsTrue(actual == expected);
      actual = usesP->getVarNamesWithProcIdx(0);
      expected = { "y", "n", "p" };
      Assert::AreEqual(actual.size(), size_t(3));
      Assert::IsTrue(actual == expected);
    }

    void editSimpleProgramFile(std::string t_editText) {
      m_tmpFile.open(m_tmpFileName);
      m_tmpFile << t_editText;
      m_tmpFile.close();
      m_parser->openFileStream(m_tmpFileName);
    }
	};
}