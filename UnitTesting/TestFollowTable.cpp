#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  private:
    FollowTable* m_testFollowTable;
    MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS testFollowTableResult;
  public:
    TEST_METHOD_INITIALIZE(InitialiseFollowsTable) {
      m_testFollowTable = new FollowTable();
      testFollowTableResult = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      m_testFollowTable->insertFollows(1, 2);
      m_testFollowTable->insertFollows(2, 3);
      m_testFollowTable->insertFollows(3, 4);
      m_testFollowTable->populateFollowsMatrix(4);
      m_testFollowTable->populateAllFollowsMap();
    }
    TEST_METHOD(TestInsertFollow) {
      //test insertFollows method.
      Assert::IsTrue(m_testFollowTable->getFollowTable() == testFollowTableResult);
      //test insertFollows method (duplicate key/value).
      //m_testFollowTable->setFollowTable(test);
      BOOLEAN expected = m_testFollowTable->insertFollows(2, 3);
      Assert::IsFalse(expected);
    }
    TEST_METHOD(TestIsFollows) {
      Logger::WriteMessage("Running follow table test isFollows");
      //test isFollows method (correct behaviour).
      BOOLEAN expected = m_testFollowTable->isFollows(1, 2);
      Assert::IsTrue(expected);
      //test isFollows method (existing in vector but not first element).
      expected = m_testFollowTable->isFollows(1, 3);
      Assert::IsFalse(expected);
      //test isFollows method (non-existing key/value pair).
      expected = m_testFollowTable->isFollows(1, 6);
      Assert::IsFalse(expected);
      //test isFollows method (non-existing key).
      expected = m_testFollowTable->isFollows(5, 6);
      Assert::IsFalse(expected);
    }

    TEST_METHOD(TestIsFollowsStar) {
      Logger::WriteMessage("Running follow table test isFollowsStar");
      //test isFollowsStar method (correct behaviour).
      BOOLEAN expected = m_testFollowTable->isFollowsStar(1, 4);
      Assert::IsTrue(expected);
      //test isFollowsStar method (non-existing key).
      expected = m_testFollowTable->isFollows(4, 5);
      Assert::IsFalse(expected);
      //test isFollowsStar method (non-existing value in existing key).
      expected = m_testFollowTable->isFollows(2, 10);
      Assert::IsFalse(expected);
    }

    TEST_METHOD(TestGetFollows) {
      Logger::WriteMessage("Running follow table test getFollows");
      //test getFollows method (correct behaviour)
      int expected = m_testFollowTable->getFollows(1);
      Assert::IsTrue(expected == 2);
      //test getFollows method (non-existing s1, expects exception)
      BOOLEAN exceptionThrown = false;
      try {
        int expected = m_testFollowTable->getFollows(5);
      } catch(InvalidArgumentException) {
        Logger::WriteMessage("Exception thrown in getFollows");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown);
    }

    TEST_METHOD(TestGetFollowedBy) {
      Logger::WriteMessage("Running follow table test getFollowedBy");
      //test getFollowedBy method (correct behaviour)
      int expected = m_testFollowTable->getFollowedBy(3);
      Assert::IsTrue(expected == 2);
      //test getFollowedBy method (s2 being the first element in vector)
      expected = m_testFollowTable->getFollowedBy(2);
      Assert::IsTrue(expected == 1);
      //test getFollowed method (non-existing s2, expects exception)
      BOOLEAN exceptionThrown = false;
      try {
        expected = m_testFollowTable->getFollowedBy(5);
      } catch (InvalidArgumentException) {
        Logger::WriteMessage("Exception thrown in getFollowedBy");
        exceptionThrown = true;
      }
      Assert::IsTrue(exceptionThrown);
    }

    TEST_METHOD(TestGetFollowsStar) {
      Logger::WriteMessage("Running follow table test getFollowsStar");
      //test getFollowsStar method (correct behaviour)
      std::vector<int> expected = m_testFollowTable->getFollowsStar(1);
      static const int arr[] = { 2, 3, 4 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(expected == actual);

      //test getFollowsStar method (non-existent s1, return empty vector)
      std::vector<int> emptyResult;
      expected = m_testFollowTable->getFollowsStar(5);
      Assert::IsTrue(expected == emptyResult);
    }

    TEST_METHOD(TestGetFollowedByStar) {
      Logger::WriteMessage("Running follow table test getFollowedByStar");
      //test getFollowedByStar method (correct behaviour)
      std::vector<int> expected = m_testFollowTable->getFollowedByStar(4);
      std::sort(expected.begin(), expected.end());
      static const int arr[] = { 1, 2, 3 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(expected == actual);
      //test getFollowedByStar method (non-existent s2, return empty vector)
      std::vector<int> emptyResult;
      expected = m_testFollowTable->getFollowedByStar(5);
      Assert::IsTrue(expected == emptyResult);
    }

    TEST_METHOD(TestGetAllFollows) {
      Logger::WriteMessage("Running follow table test getAllFollows");

      //test getAllFollows method (correct behaviour)
      std::unordered_map<int, int> expected = {
        { 1, 2 },
        { 2, 3 },
        { 3, 4 }
      };
      Assert::IsTrue(expected == m_testFollowTable->getAllFollows());
    }

    TEST_METHOD(TestGetFollowedByAnything) {
      Logger::WriteMessage("Running follow table test getFollowedByAnything");
      //test getFollowedByStar method (correct behaviour)
      std::vector<int> expected = m_testFollowTable->getFollowedByAnything();
      static const int arr[] = { 1, 2, 3 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestGetFollowsAnything) {
      Logger::WriteMessage("Running follow table test getFollowsAnything");
      static const int arr[] = { 2, 3, 4 };
      std::vector<int> actual(arr, arr + sizeof(arr) / sizeof(arr[0]));

      std::vector<int> expected = m_testFollowTable->getFollowsAnything();
      Assert::IsTrue(expected == actual);
    }

    TEST_METHOD(TestIsFollowsAnything) {
      Logger::WriteMessage("Running follow table test isFollowsAnything");


      Assert::IsFalse(m_testFollowTable->isFollowsAnything(1));
      Assert::IsTrue(m_testFollowTable->isFollowsAnything(2));
      Assert::IsTrue(m_testFollowTable->isFollowsAnything(3));
      Assert::IsTrue(m_testFollowTable->isFollowsAnything(4));
    }

    TEST_METHOD(TestIsFollowedByAnything) {
      Logger::WriteMessage("Running follow table test isFollowedByAnything");

      Assert::IsTrue(m_testFollowTable->isFollowedByAnything(2));
      Assert::IsTrue(m_testFollowTable->isFollowedByAnything(3));
      Assert::IsFalse(m_testFollowTable->isFollowedByAnything(4));

    }
  };
}