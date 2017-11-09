#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql/QueryCache.h"
#include "Pattern.h"
#include "With.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestQueryCache) {
public:
  TEST_METHOD(testKey) {
    Logger::WriteMessage("Print out keys for Clauses");
    Pattern pattern;
    Relation relation;

    pattern = Pattern(Grammar(4, "smth"), Grammar(), Grammar(), false);
    QueryCache cache = QueryCache();
    std::string key;

    key = cache.getKey(pattern);

    Logger::WriteMessage(key.c_str());

    relation = Relation("Next*", Grammar(11, "_"), Grammar(9, "p1"));
    key = cache.getKey(relation);
    Assert::IsTrue(key == "Next*_s");
    Logger::WriteMessage(key.c_str());

    relation = Relation("Affects*", Grammar(3, "s4"), Grammar(3, "s5"));
    key = cache.getKey(relation);
    Assert::IsTrue(key == "Affects*s1s2");
    Logger::WriteMessage(key.c_str());

    relation = Relation("Parent*", Grammar(3, "s5"), Grammar(3, "s5"));
    key = cache.getKey(relation);
    Assert::IsTrue(key == "Parent*ss");
    Logger::WriteMessage(key.c_str());
  }

  TEST_METHOD(isCacheable) {
    QueryCache cache = QueryCache();
    Clause *clause;

    clause = new Pattern(Grammar(4, "smth"), Grammar(), Grammar(), false);
    Assert::IsFalse(cache.isCacheable(clause));

    clause = new Pattern(Grammar(4, "smth"), Grammar(7, "smth"), Grammar(11, "_"), false);
    Assert::IsTrue(cache.isCacheable(clause));

    clause = new Relation("Next*", Grammar(11, "_"), Grammar());
    Assert::IsTrue(cache.isCacheable(clause));

    clause = new Relation("Follows*", Grammar(8, "3"), Grammar());
    Assert::IsFalse(cache.isCacheable(clause));

    clause = new With(Grammar(), Grammar());
    Assert::IsFalse(cache.isCacheable(clause));
  }

  TEST_METHOD(isPatternType_isRelationType) {
    Clause *clause;

    clause = new Pattern(Grammar(4, "smth"), Grammar(), Grammar(), false);
    Assert::IsTrue(clause->isPatternType());
    Assert::IsFalse(clause->isRelationType());

    clause = new Relation("Next*", Grammar(11, "_"), Grammar());
    Assert::IsTrue(clause->isRelationType());
    Assert::IsFalse(clause->isPatternType());
  }

  TEST_METHOD(cache_getCache) {
    Clause *clause;
    QueryCache cache = QueryCache();
    SET_OF_RESULTS *results, expected;

    expected = SET_OF_RESULTS();
    expected.insert({ "result1", {"1", "2", "3", "4"} });
    expected.insert({ "result2", { "1", "4" } });

    clause = new Relation("Parent*", Grammar(11, "_"), Grammar(2, "s1"));

    cache.cache(clause, expected);
    results = cache.getCache(clause);
    Assert::IsTrue(*results == expected);

    clause = new Relation("Parent*", Grammar(11, "_"), Grammar(2, "s7"));
    results = cache.getCache(clause);
    Assert::IsTrue(*results == expected);
  }

  TEST_METHOD(queryUtil_areBothSameSynonyms) {
    Assert::IsTrue(QueryUtil::areBothSameSynonyms(Grammar(2, "s7"), Grammar(2, "s7")));
    Assert::IsFalse(QueryUtil::areBothSameSynonyms(Grammar(2, "s7"), Grammar(2, "123")));
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
