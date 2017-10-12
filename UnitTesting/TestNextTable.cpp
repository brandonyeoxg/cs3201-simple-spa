#include "stdafx.h"
#include "CppUnitTest.h"
#include "NextTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestNextTable) {
public:

  TEST_METHOD(isNext) {
    NextTable nextTable = NextTable();
    nextTable.insertNextRelationship(0, 1);
    nextTable.insertNextRelationship(0, 2);
    nextTable.insertNextRelationship(0, 3);
    nextTable.insertNextRelationship(0, 500); // Test a large number

    nextTable.executeAfterAllNextInserts();

    Assert::IsFalse(nextTable.isNext(1, 3));
    Assert::IsFalse(nextTable.isNext(500, 0));

    Assert::IsTrue(nextTable.isNext(0, 3));
    Assert::IsTrue(nextTable.isNext(0, 1));
    Assert::IsTrue(nextTable.isNext(0, 500));
  }

  TEST_METHOD(isNextStar) {
    NextTable nextTable = NextTable();
    nextTable.insertNextRelationship(0, 1);
    nextTable.insertNextRelationship(0, 2);
    nextTable.insertNextRelationship(0, 3);
    nextTable.insertNextRelationship(2, 1);
    nextTable.executeAfterAllNextInserts();

    Assert::IsTrue(nextTable.isNext(0, 1));
    Assert::IsTrue(nextTable.isNextStar(0, 1));

    Assert::IsFalse(nextTable.isNext(1, 3));
    Assert::IsFalse(nextTable.isNextStar(1, 3));

    Assert::IsFalse(nextTable.isNextStar(0, 5));

    nextTable.insertNextRelationship(2, 4);
    nextTable.insertNextRelationship(4, 5);
    nextTable.executeAfterAllNextInserts();

    Assert::IsTrue(nextTable.isNextStar(0, 5));

  }

  TEST_METHOD(isNext_isNextStar_01) {
    NextTable nextTable = NextTable();

    // basically an if statement, with a while statement nested in else section
    // if-statement: line 1, while-statement: line 3
    /*  1   if ...
        2     stmt
            else
        3     while ...
        4       stmt
        5   stmt
    */
    nextTable.insertNextRelationship(1, 2);
    nextTable.insertNextRelationship(1, 3);
    nextTable.insertNextRelationship(3, 4);
    nextTable.insertNextRelationship(4, 3);
    nextTable.insertNextRelationship(3, 5);
    nextTable.insertNextRelationship(2, 5);

    nextTable.executeAfterAllNextInserts();

    // test all possible control paths for Next()
    Assert::IsTrue(nextTable.isNext(1, 2));
    Assert::IsTrue(nextTable.isNext(1, 3));
    Assert::IsTrue(nextTable.isNext(3, 4));
    Assert::IsTrue(nextTable.isNext(4, 3));
    Assert::IsTrue(nextTable.isNext(3, 5));
    Assert::IsTrue(nextTable.isNext(2, 5));

    // test for false paths
    Assert::IsFalse(nextTable.isNext(1, 5));
    Assert::IsFalse(nextTable.isNext(2, 3));
    Assert::IsFalse(nextTable.isNext(2, 4));
    Assert::IsFalse(nextTable.isNext(2, 1));
    Assert::IsFalse(nextTable.isNext(4, 5));
    Assert::IsFalse(nextTable.isNext(3, 3));

    // test for possible control paths for Next*()
    Assert::IsTrue(nextTable.isNextStar(1, 2));
    Assert::IsTrue(nextTable.isNextStar(1, 5));
    Assert::IsTrue(nextTable.isNextStar(1, 4));
    Assert::IsTrue(nextTable.isNextStar(4, 5));
    Assert::IsTrue(nextTable.isNextStar(3, 3));
  }

  TEST_METHOD(isNext_isNextStar_02) {
    NextTable nextTable = NextTable();

    // if-statement nested within if-statement
    /*  1   stmt
        2   stmt
        3   if ...
        4     if ...
        5       stmt
              else
        6       stmt
            else
        7     stmt
    */
    nextTable.insertNextRelationship(1, 2);
    nextTable.insertNextRelationship(2, 3);
    nextTable.insertNextRelationship(3, 4);
    nextTable.insertNextRelationship(3, 7);
    nextTable.insertNextRelationship(4, 5);
    nextTable.insertNextRelationship(4, 6);

    nextTable.executeAfterAllNextInserts();

    // test possible control paths for Next()
    Assert::IsTrue(nextTable.isNext(1, 2));
    Assert::IsTrue(nextTable.isNext(2, 3));
    Assert::IsTrue(nextTable.isNext(4, 5));

    // test for false paths
    Assert::IsFalse(nextTable.isNext(1, 5));
    Assert::IsFalse(nextTable.isNext(6, 7));

    // test for possible control paths for Next*()
    Assert::IsTrue(nextTable.isNextStar(1, 2));
    Assert::IsTrue(nextTable.isNextStar(1, 3));
    Assert::IsTrue(nextTable.isNextStar(1, 4));
    Assert::IsTrue(nextTable.isNextStar(1, 5));
    Assert::IsTrue(nextTable.isNextStar(1, 6));
    Assert::IsTrue(nextTable.isNextStar(1, 7));

    Assert::IsTrue(nextTable.isNextStar(3, 5));
    Assert::IsTrue(nextTable.isNextStar(3, 6));
    Assert::IsTrue(nextTable.isNextStar(3, 7));

    Assert::IsTrue(nextTable.isNextStar(2, 3));
    Assert::IsTrue(nextTable.isNextStar(2, 4));
    Assert::IsTrue(nextTable.isNextStar(2, 5));
  }

  TEST_METHOD(getLinesAfter) {
    NextTable nextTable = NextTable();
    std::vector<PROG_LINE> expected, result;

    /*  1   if ...
        2     stmt
            else
        3     while ...
        4       stmt
        5       stmt
        6       stmt
    */
    nextTable.insertNextRelationship(1, 2);
    nextTable.insertNextRelationship(1, 3);
    nextTable.insertNextRelationship(3, 4);
    nextTable.insertNextRelationship(4, 5);
    nextTable.insertNextRelationship(5, 6);
    nextTable.insertNextRelationship(6, 3);

    nextTable.executeAfterAllNextInserts();

    expected = { 2, 3 };
    result = nextTable.getLinesAfter(1);
    Assert::IsTrue(expected == result);

    expected = {};
    result = nextTable.getLinesAfter(2);
    Assert::IsTrue(expected == result);

    expected = { 4 };
    result = nextTable.getLinesAfter(3);
    Assert::IsTrue(expected == result);
  }

  TEST_METHOD(getAllLinesAfter) {
    NextTable nextTable = NextTable();
    std::vector<PROG_LINE> expected, result;

    /*  1   if ...
        2     stmt
            else
        3     while ...
        4       stmt
        5       stmt
        6       stmt
    */
    nextTable.insertNextRelationship(1, 2);
    nextTable.insertNextRelationship(1, 3);
    nextTable.insertNextRelationship(3, 4);
    nextTable.insertNextRelationship(4, 5);
    nextTable.insertNextRelationship(5, 6);
    nextTable.insertNextRelationship(6, 3);

    nextTable.executeAfterAllNextInserts();

    expected = { 2, 3, 4, 5, 6 };
    result = nextTable.getAllLinesAfter(1);
    Assert::IsTrue(expected == result);

    expected = {};
    result = nextTable.getAllLinesAfter(2);
    Assert::IsTrue(expected == result);

    expected = { 3, 4, 5, 6 };
    result = nextTable.getAllLinesAfter(3);
    Assert::IsTrue(expected == result);

    expected = { 3, 4, 5, 6 };
    result = nextTable.getAllLinesAfter(4);
    Assert::IsTrue(expected == result);

    expected = { 3, 4, 5, 6 };
    result = nextTable.getAllLinesAfter(6);
    Assert::IsTrue(expected == result);
  }

  TEST_METHOD(getLinesBefore) {
    NextTable nextTable = NextTable();
    std::vector<PROG_LINE> expected, result;

    /*  1   while ...
        2     while ...
        3       while ...
        4         stmt
    */

    nextTable.insertNextRelationship(1, 2);
    nextTable.insertNextRelationship(2, 3);
    nextTable.insertNextRelationship(3, 4);
    nextTable.insertNextRelationship(4, 3);
    nextTable.insertNextRelationship(3, 2);
    nextTable.insertNextRelationship(2, 1);

    nextTable.executeAfterAllNextInserts();

    expected = { 2 };
    result = nextTable.getLinesBefore(1);
    Assert::IsTrue(expected == result);

    expected = { 1, 3 };
    result = nextTable.getLinesBefore(2);
    Assert::IsTrue(expected == result);

    expected = { 2, 4 };
    result = nextTable.getLinesBefore(3);
    Assert::IsTrue(expected == result);

    expected = { 3 };
    result = nextTable.getLinesBefore(4);
    Assert::IsTrue(expected == result);
  }

  TEST_METHOD(getAllLinesBefore) {
    NextTable nextTable = NextTable();
    std::vector<PROG_LINE> expected, result;

    /*  1   while ...
        2     while ...
        3       while ...
        4         stmt
    */

    nextTable.insertNextRelationship(1, 2);
    nextTable.insertNextRelationship(2, 3);
    nextTable.insertNextRelationship(3, 4);
    nextTable.insertNextRelationship(4, 3);
    nextTable.insertNextRelationship(3, 2);
    nextTable.insertNextRelationship(2, 1);

    nextTable.executeAfterAllNextInserts();

    expected = { 1, 2, 3, 4 };
    result = nextTable.getAllLinesBefore(1);
    Assert::IsTrue(expected == result);

    expected = { 1, 2, 3, 4 };
    result = nextTable.getAllLinesBefore(2);
    Assert::IsTrue(expected == result);

    expected = { 1, 2, 3, 4 };
    result = nextTable.getAllLinesBefore(3);
    Assert::IsTrue(expected == result);

    expected = { 1, 2, 3, 4 };
    result = nextTable.getAllLinesBefore(4);
    Assert::IsTrue(expected == result);
  }

  TEST_METHOD(getAllNextStar) {
    NextTable nextTable = NextTable();
    std::vector<PROG_LINE> expected;

    /*  1   while ...
        2     while ...
        3       while ...
        4         stmt
    */

    nextTable.insertNextRelationship(1, 2);
    nextTable.insertNextRelationship(2, 3);
    nextTable.insertNextRelationship(3, 4);
    nextTable.insertNextRelationship(4, 3);
    nextTable.insertNextRelationship(3, 2);
    nextTable.insertNextRelationship(2, 1);

    std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> map = nextTable.getAllNextStar();
    expected = { 1, 2, 3, 4 };

    Assert::IsTrue(map.size() == 4);  // 4 keys in total

    for (auto iter : expected) {
      Assert::IsTrue(map.count(iter) == 1); // Each key corresponds to a line number
    }

    for (auto iter : map) {
      Assert::IsTrue(iter.second == expected);  // Check lines reachable for each line
    }

  }

private:

  void printGraph(std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> graph) {
    for (auto iter : graph) {
      Logger::WriteMessage(("Line " + std::to_string(iter.first)).c_str());
      for (auto iter2 : iter.second) {
        Logger::WriteMessage(std::to_string(iter2).c_str());
      }
    }
  }

  void printVector(std::vector<int> vector) {
    for (auto iterator : vector) {
      Logger::WriteMessage(std::to_string(iterator).c_str());
    }
  }

  };
}