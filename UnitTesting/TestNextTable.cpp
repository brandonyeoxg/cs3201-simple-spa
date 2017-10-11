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