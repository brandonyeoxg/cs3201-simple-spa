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

    Assert::IsTrue(nextTable.isNextStar(0, 1));
    Assert::IsTrue(nextTable.isNextStar(0, 1));

    Assert::IsFalse(nextTable.isNext(1, 3));
    Assert::IsFalse(nextTable.isNextStar(1, 3));

    Assert::IsFalse(nextTable.isNextStar(0, 5));
    nextTable.insertNextRelationship(2, 5);
    //nextTable.insertNextRelationship(4, 5);
    nextTable.executeAfterAllNextInserts();
    Assert::IsTrue(nextTable.isNextStar(0, 5));
  }

  };
}