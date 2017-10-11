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
    nextTable.insertNextRelationship(0, 500);

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

private:

  void printGraph(std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> graph) {
    for (auto iter : graph) {
      Logger::WriteMessage(("Line " + std::to_string(iter.first)).c_str());
      for (auto iter2 : iter.second) {
        Logger::WriteMessage(std::to_string(iter2).c_str());
      }
    }
  }

  };
}