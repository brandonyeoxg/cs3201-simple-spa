#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql/QueryCache.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestQueryCache) {
public:
  TEST_METHOD(cacheQuery) {
    QueryCache cache = QueryCache();
    PKB pkb = PKB();
    LIST_OF_PROG_LINES expectedLines, list;

    Assert::IsTrue(cache.getAllNext() == nullptr);
    Assert::IsTrue(cache.getAllNextStar() == nullptr);
    Assert::IsTrue(cache.getAllLinesAfterAnyLine() == nullptr);
    Assert::IsTrue(cache.getChildrenOfAnything() == nullptr);
    Assert::IsTrue(cache.getParentStarOfAnything() == nullptr);
    Assert::IsTrue(cache.getAllIfStmtsWithVar() == nullptr);

    pkb.insertNextRelation(1, 2);
    pkb.insertNextRelation(2, 3);
    pkb.insertNextRelation(3, 4);
    pkb.insertNextRelation(4, 2);
    pkb.insertNextRelation(2, 5);

    pkb.executeAfterAllNextInserts();

    expectedLines = {2, 3, 4, 5};
    list = pkb.getAllLinesAfterAnyLine();
    cache.cacheAllLinesAfterAnyLine(list);
    Assert::IsTrue(*cache.getAllLinesAfterAnyLine() == expectedLines);

    expectedLines = { 1, 2, 3, 4 };
    list = pkb.getAllLinesBeforeAnyLine();
    cache.cacheAllLinesBeforeAnyLine(list);
    Assert::IsTrue(*cache.getAllLinesBeforeAnyLine() == expectedLines);
  }

  TEST_METHOD(cacheNextStar_getAllLinesAfter) {
    QueryCache cache = QueryCache();
    PKB pkb = PKB();
    LIST_OF_PROG_LINES expected;

    pkb.insertNextRelation(1, 2);
    pkb.insertNextRelation(2, 3);
    pkb.insertNextRelation(3, 4);
    pkb.insertNextRelation(4, 2);
    pkb.insertNextRelation(2, 5);

    pkb.executeAfterAllNextInserts();

    MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES map = pkb.getAllNextStar();
    cache.cacheAllNextStar(map);

    expected = { 2, 3, 4, 5 };
    Assert::IsTrue(*cache.getAllLinesAfter(1) == expected);

    expected = {};
    Assert::IsTrue(*cache.getAllLinesAfter(10) == expected);
  }


private:
  void printVector(std::vector<int> vector) {
    Logger::WriteMessage("Printing vector");
    for (auto iterator : vector) {
      Logger::WriteMessage(std::to_string(iterator).c_str());
    }
  }
  };
}
