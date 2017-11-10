#include "stdafx.h"
#include "CppUnitTest.h"
#include "test-drivers\IntermediateTableDriver.h"
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestIntermediateTable) {
  public:
    PkbReadOnly* m_pkb;
    IntermediateTableDriver* m_driver;
    IntermediateTableDriver* tableDriver1;
    IntermediateTableDriver* tableDriver2;
    IntermediateTableDriver* tableDriver3;

    TEST_METHOD_INITIALIZE(intialiseIntermediateTable)
    {
      m_driver = new IntermediateTableDriver();
      tableDriver1 = new IntermediateTableDriver();
      tableDriver2 = new IntermediateTableDriver();
      tableDriver3 = new IntermediateTableDriver();

      /*
      a   v   s1  s2 
      1  'a'  2   3
      2  'b'  3   4
      3  'c'  4   5
      */
      INTERMEDIATE_TABLE result = { {"1", "a", "2", "3"}, 
      {"2", "b", "3", "4"}, {"3","c","4", "5"}};
      m_driver->insertTable(result);
      m_driver->insertSynonymRow({"a", "v", "s1", "s2"});
    }

    TEST_METHOD_CLEANUP(cleanupIntermediateTable)
    {
      delete m_driver;
      delete tableDriver1;
      delete tableDriver2;
      delete tableDriver3;
    }

    TEST_METHOD(TestHasSynonym)
    {
      bool actual = m_driver->TestHasSynonym("a");
      Assert::IsTrue(actual);

      actual = m_driver->TestHasSynonym("s1");
      Assert::IsTrue(actual);

      actual = m_driver->TestHasSynonym("kokoko");
      Assert::IsFalse(actual);

      actual = m_driver->TestHasSynonym("");
      Assert::IsFalse(actual);
    }

    TEST_METHOD(TestGetResults)
    {
      Grammar g1 = Grammar(3, "a");
      Grammar g2 = Grammar(7, "v");
      Grammar g3 = Grammar(2, "s1");
      Grammar g4 = Grammar(2, "s2");
      Grammar g5 = Grammar(2, "KOKOK");
      Grammar g6 = Grammar(2, "face");
      Grammar g7 = Grammar(2, "book");

      LIST_OF_RESULTS actual = m_driver->getResults({ g1 }, m_pkb);
      LIST_OF_RESULTS expected = { "1", "2", "3" };
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({ g1, g2 }, m_pkb);
      expected = { "1 a", "2 b", "3 c" };
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({ g1, g3, g4 }, m_pkb);
      expected = { "1 2 3", "2 3 4", "3 4 5" };
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({ g5 }, m_pkb);
      expected = {};
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({ g6, g7 }, m_pkb);
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({}, m_pkb);
      Assert::IsTrue(actual == expected);
    }

    TEST_METHOD(TestInsertOneSynonym)
    {
      m_driver->clearTable();

      //Test the inserting of one synonym when the table is empty
      LIST_OF_RESULTS result = { "1", "3", "7" };
      INTERMEDIATE_TABLE actual = m_driver->TestInsertOneSynonym("s1", result);
      INTERMEDIATE_TABLE expected = { { "1" },{ "3" },{ "7" } };
      Assert::IsTrue(actual == expected);

      //Test the inserting of one synonym when the synonym is not inside the non-empty table
      result = { "2", "4" };
      actual = m_driver->TestInsertOneSynonym("s2", result);
      expected = { { "1", "2" },{ "3", "2" },{ "7", "2" },{ "1", "4" },{ "3", "4" },{ "7", "4" } };
      Assert::IsTrue(actual == expected);

      //Test the inserting of one synonym when the synonym is inside the non-empty table
      result = { "1", "3", "4" };
      actual = m_driver->TestInsertOneSynonym("s2", result);
      expected = { { "1", "4" },{ "3", "4" },{ "7", "4" } };
      Assert::IsTrue(actual == expected);
    }

    TEST_METHOD(TestInsertTwoSynonym)
    {
      m_driver->clearTable();

      //Test the inserting of two synonyms when the table is empty
      SET_OF_RESULTS result = { { "1",{ "4" } },{ "3",{ "4" } },{ "7",{ "4" } } };
      INTERMEDIATE_TABLE actual = m_driver->TestInsertTwoSynonym("s1", "s2", result);
      INTERMEDIATE_TABLE expected = { { "1", "4" },{ "3", "4" },{ "7", "4" } };
      Assert::IsTrue(actual == expected);

      //Test the inserting of two synonyms when both synonyms are not inside the non-empty table
      result = { { "2", { "3", "4", "5" } },{ "3", { "4", "5" } },{ "4", { "5" } } };
      actual = m_driver->TestInsertTwoSynonym("s3", "s4", result);
      expected = { 
      { "1", "4", "2", "3" },{ "3", "4", "2", "3" },{ "7", "4", "2", "3" },
      { "1", "4", "2", "4" },{ "3", "4", "2", "4" },{ "7", "4", "2", "4" },
      { "1", "4", "2", "5" },{ "3", "4", "2", "5" },{ "7", "4", "2", "5" },
      { "1", "4", "3", "4" },{ "3", "4", "3", "4" },{ "7", "4", "3", "4" },
      { "1", "4", "3", "5" },{ "3", "4", "3", "5" },{ "7", "4", "3", "5" },
      { "1", "4", "4", "5" },{ "3", "4", "4", "5" },{ "7", "4", "4", "5" } };
      Assert::IsTrue(actual == expected);

      //Test the inserting of two synonyms when one of the synonym is inside the non-empty table
      result = { { "1", { "2", "3", "4" } },{ "2", { "3", "4", "5" } },{ "4", { "5", "6" } },{ "6", { "7" } } };
      actual = m_driver->TestInsertTwoSynonym("s4", "s5", result);
      expected = {
        { "1", "4", "2", "4", "5" },{ "1", "4", "2", "4", "6" },{ "3", "4", "2", "4", "5" },{ "3", "4", "2", "4", "6" },
        { "7", "4", "2", "4", "5" },{ "7", "4", "2", "4", "6" },{ "1", "4", "3", "4", "5" },{ "1", "4", "3", "4", "6" },
        { "3", "4", "3", "4", "5" },{ "3", "4", "3", "4", "6" },{ "7", "4", "3", "4", "5" },{ "7", "4", "3", "4", "6" } };
      Assert::IsTrue(actual == expected);

      //Test the inserting of two synonyms when both synonyms are inside the non-empty table
      result = { { "1", { "4", "6", "7" } },{ "2", { "3", "4", "5" } }, { "3",{ "7", "8" } } };
      actual = m_driver->TestInsertTwoSynonym("s1", "s5", result);
      expected = { { "1", "4", "2", "4", "6" },{ "1", "4", "3", "4", "6" } };
      Assert::IsTrue(actual == expected);
    }

    TEST_METHOD(TestMergeTables)
    {
      m_driver->clearTable();

      LIST_OF_RESULTS result = { "1", "3", "7" };
      INTERMEDIATE_TABLE table1 = tableDriver1->TestInsertOneSynonym("s1", result);

      result = { "2", "4" };
      INTERMEDIATE_TABLE table2 = tableDriver2->TestInsertOneSynonym("s2", result);

      std::pair<MAP_OF_SYNONYM_TO_TABLE_POSITION, INTERMEDIATE_TABLE> actual = m_driver->TestMergeTables({ tableDriver1, tableDriver2 });
      MAP_OF_SYNONYM_TO_TABLE_POSITION expectedSynRow = { { "s1", 0 },{ "s2", 1 } };
      INTERMEDIATE_TABLE expectedTable = { { "1", "2" },{ "3", "2" },{ "7", "2" },{ "1", "4" },
      { "3", "4" },{ "7", "4" } };
      Assert::IsTrue(actual.first == expectedSynRow);
      Assert::IsTrue(actual.second == expectedTable);
      m_driver->clearTable();
      tableDriver1->clearTable();
      tableDriver2->clearTable();

      LIST_OF_SYNONYMS synRow1 = { "s1", "s2" };
      table1 = { { "1", "2" },{ "3", "2" },{ "7", "2" } };
      tableDriver1->insertSynonymRow(synRow1);
      tableDriver1->insertTable(table1);

      LIST_OF_SYNONYMS synRow2 = { "s3", "s4", "s5" };
      table2 = { { "3", "2", "4" },{ "6", "2", "2" } };
      tableDriver2->insertSynonymRow(synRow2);
      tableDriver2->insertTable(table2);

      actual = m_driver->TestMergeTables({ tableDriver1, tableDriver2 });
      expectedSynRow = { { "s1", 0 },{ "s2", 1 }, { "s3", 2 }, { "s4", 3 }, { "s5", 4 } };
      expectedTable = { { "1", "2", "3", "2", "4" },{ "3", "2", "3", "2", "4" },{ "7", "2", "3", "2", "4" },
      { "1", "2", "6", "2", "2" },{ "3", "2", "6", "2", "2" },{ "7", "2", "6", "2", "2" } };
      Assert::IsTrue(actual.first == expectedSynRow);
      Assert::IsTrue(actual.second == expectedTable);
      m_driver->clearTable();
      tableDriver1->clearTable();
      tableDriver2->clearTable();

      synRow1 = { "s1", "s2" };
      table1 = { { "1", "2" },{ "3", "2" },{ "7", "2" } };
      tableDriver1->insertSynonymRow(synRow1);
      tableDriver1->insertTable(table1);

      synRow2 = { "s3", "s4", "s5" };
      table2 = { { "3", "2", "4" },{ "6", "2", "2" } };
      tableDriver2->insertSynonymRow(synRow2);
      tableDriver2->insertTable(table2);

      LIST_OF_SYNONYMS synRow3 = { "s6", "s7" };
      INTERMEDIATE_TABLE table3 = { { "1", "8" },{ "8", "9" },{ "2", "5" },{ "3", "4" } };
      tableDriver3->insertSynonymRow(synRow3);
      tableDriver3->insertTable(table3);

      actual = m_driver->TestMergeTables({ tableDriver1, tableDriver2, tableDriver3 });
      expectedSynRow = { { "s1", 0 },{ "s2", 1 },{ "s3", 2 },{ "s4", 3 },{ "s5", 4 },{ "s6", 5 },{ "s7", 6 } };
      expectedTable = { { "1", "2", "3", "2", "4", "1", "8" },{ "3", "2", "3", "2", "4", "1", "8" },{ "7", "2", "3", "2", "4", "1", "8" },{ "1", "2", "6", "2", "2", "1", "8" },{ "3", "2", "6", "2", "2", "1", "8" },{ "7", "2", "6", "2", "2", "1", "8" },
      { "1", "2", "3", "2", "4", "8", "9" },{ "3", "2", "3", "2", "4", "8", "9" },{ "7", "2", "3", "2", "4", "8", "9" },{ "1", "2", "6", "2", "2", "8", "9" },{ "3", "2", "6", "2", "2", "8", "9" },{ "7", "2", "6", "2", "2", "8", "9" },
      { "1", "2", "3", "2", "4", "2", "5" },{ "3", "2", "3", "2", "4", "2", "5" },{ "7", "2", "3", "2", "4", "2", "5" },{ "1", "2", "6", "2", "2", "2", "5" },{ "3", "2", "6", "2", "2", "2", "5" },{ "7", "2", "6", "2", "2", "2", "5" },
      { "1", "2", "3", "2", "4", "3", "4" },{ "3", "2", "3", "2", "4", "3", "4" },{ "7", "2", "3", "2", "4", "3", "4" },{ "1", "2", "6", "2", "2", "3", "4" },{ "3", "2", "6", "2", "2", "3", "4" },{ "7", "2", "6", "2", "2", "3", "4" } };
      Assert::IsTrue(actual.first == expectedSynRow);
      Assert::IsTrue(actual.second == expectedTable);
    }
  };
}