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
      INTERMEDIATE_TABLE result = { {"1", "2", "3"}, 
      {"a", "b", "c"}, {"2","3","4"}, {"3", "4", "5"} };
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
      bool expected = true;
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
      LIST_OF_RESULTS expected = {"1", "2", "3"};
      Assert::IsTrue(actual == expected);

      actual = m_driver->getResults({"a", "v"});
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
  };
}