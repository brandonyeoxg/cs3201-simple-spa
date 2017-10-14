#include "stdafx.h"
#include "CppUnitTest.h"
#include "test-drivers\IntermediateTableDriver.h"
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestIntermediateTable) {
  public:
    IntermediateTableDriver* m_driver;

    TEST_METHOD_INITIALIZE(intialiseIntermediateTable)
    {
      m_driver = new IntermediateTableDriver();

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
      LIST_OF_RESULTS actual = m_driver->getResults({ "a" });
      LIST_OF_RESULTS expected = { "1", "2", "3" };
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({ "a", "v" });
      expected = { "1 a", "2 b", "3 c" };
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({ "a", "s1", "s2" });
      expected = { "1 2 3", "2 3 4", "3 4 5" };
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({ "KOKOK" });
      expected = {};
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({ "face", "book" });
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({});
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
  };
}