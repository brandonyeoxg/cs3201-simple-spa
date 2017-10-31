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

    LIST_OF_PROG_LINES expectedLines = {2, 3, 4, 5};
    cache.cacheAllLinesAfterAnyLine(pkb.getAllLinesAfterAnyLine());
    Assert::IsTrue(*cache.getAllLinesAfterAnyLine() == expectedLines);
  }
  };
}
