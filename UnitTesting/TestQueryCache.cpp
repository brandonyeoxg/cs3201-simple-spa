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
    QueryCache cache = QueryCache();
    std::string key;

    pattern = Pattern(Grammar(4, "smth"), Grammar(), Grammar(), false);
    key = cache.getKey(pattern);
    Assert::IsTrue(key == "Pattern_While");
    Logger::WriteMessage(key.c_str());

    relation = Relation("Next*", Grammar(11, "_"), Grammar(9, "p1"));
    key = cache.getKey(relation);
    Assert::IsTrue(key == "Next*/_/s");
    Logger::WriteMessage(key.c_str());

    relation = Relation("Affects*", Grammar(3, "s4"), Grammar(3, "s5"));
    key = cache.getKey(relation);
    Assert::IsTrue(key == "Affects*/s1/s2");
    Logger::WriteMessage(key.c_str());

    relation = Relation("Parent*", Grammar(3, "s5"), Grammar(3, "s5"));
    key = cache.getKey(relation);
    Assert::IsTrue(key == "Parent*/s/s");
    Logger::WriteMessage(key.c_str());

    // Uses(a5, "s2")
    relation = Relation("Uses", Grammar(3, "a5"), Grammar(11, "s2"));
    key = cache.getKey(relation);
    Assert::IsTrue(key == "Uses/s/s2");
    Logger::WriteMessage(key.c_str());

    // 2 common synonyms
    relation = Relation("Uses", Grammar(3, "a5"), Grammar(7, "s2"));
    key = cache.getKey(relation);
    Assert::IsTrue(key == "Uses/s1/s2");
    Logger::WriteMessage(key.c_str());

    // Uses("s1", a5)
    relation = Relation("Uses", Grammar(11, "s1"), Grammar(3, "a5"));
    key = cache.getKey(relation);
    Assert::IsTrue(key == "Uses/s1/s");
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

    clause = new Relation("Next*", Grammar(11, "12"), Grammar(2, "s1"));
    Assert::IsTrue(cache.isCacheable(clause));
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

  TEST_METHOD(cache_getCache01) {
    Clause *clause;
    QueryCache cache = QueryCache();
    SET_OF_RESULTS_INDICES *results, expected;

    expected = SET_OF_RESULTS_INDICES();
    expected.insert({ 1, { 1, 2, 3, 4 } });
    expected.insert({ 2, { 1, 4 } });

    clause = new Relation("Parent*", Grammar(11, "_"), Grammar(2, "s1"));

    cache.cache(clause, expected);
    results = cache.getCache(clause);
    Assert::IsTrue(*results == expected);

    clause = new Relation("Parent*", Grammar(11, "_"), Grammar(2, "s7"));
    results = cache.getCache(clause);
    Assert::IsTrue(*results == expected);
  }

  TEST_METHOD(cache_getCache02) {
    Clause *clause;
    QueryCache cache = QueryCache();
    SET_OF_RESULTS_INDICES *results, expected;

    expected = SET_OF_RESULTS_INDICES();
    expected.insert({ 1, { 1, 2, 3, 4 } });
    expected.insert({ 2, { 1, 4 } });

    clause = new Relation("Affects*", Grammar(11, "55"), Grammar(3, "s1"));

    cache.cache(clause, expected);
    results = cache.getCache(clause);
    Assert::IsTrue(*results == expected);

    clause = new Relation("Affects*", Grammar(11, "55"), Grammar(3, "chicken"));

    results = cache.getCache(clause);
    Assert::IsTrue(*results == expected);

    clause = new Relation("Affects*", Grammar(11, "5"), Grammar(3, "chicken"));

    results = cache.getCache(clause);
    Assert::IsTrue(results == nullptr);
  }

  TEST_METHOD(cache_getCache03) {
    Clause *clause;
    QueryCache cache = QueryCache();
    SET_OF_RESULTS_INDICES *results, expected;

    expected = SET_OF_RESULTS_INDICES();
    expected.insert({ 1,{ 1, 2, 3, 4 } });
    expected.insert({ 2,{ 1, 4 } });

    clause = new Relation("Calls*", Grammar(3, "s1"), Grammar(11, "100"));

    cache.cache(clause, expected);
    results = cache.getCache(clause);
    Assert::IsTrue(*results == expected);

    clause = new Relation("Calls*", Grammar(3, "s4"), Grammar(11, "100"));

    results = cache.getCache(clause);
    Assert::IsTrue(*results == expected);
  }

  TEST_METHOD(queryUtil_areBothSameSynonyms) {
    Assert::IsTrue(QueryUtil::areBothSameSynonyms(Grammar(2, "s7"), Grammar(2, "s7")));
    Assert::IsFalse(QueryUtil::areBothSameSynonyms(Grammar(2, "s7"), Grammar(2, "123")));
  }

  TEST_METHOD(getCacheFromOtherClauses) {
    Clause *clause;
    QueryCache cache = QueryCache();
    SET_OF_RESULTS_INDICES *results, expected, toCache;

    toCache = SET_OF_RESULTS_INDICES();
    toCache.insert({ 1,{ 1, 2, 3, 4 } });
    toCache.insert({ 2,{ 1, 4 } });

    clause = new Relation("Next*", Grammar(9, "line1"), Grammar(9, "line2"));

    Assert::IsTrue(cache.isCacheable(clause));

    cache.cache(clause, toCache); // cache Next*(line1, line2)
    results = cache.getCache(clause);
    Assert::IsFalse(results == nullptr);

    expected = SET_OF_RESULTS_INDICES();
    expected.insert({ 1,{ 1, 2, 3, 4 } });

    clause = new Relation("Next*", Grammar(11, "1"), Grammar(9, "line3"));

    results = cache.getCache(clause);
    Assert::IsFalse(results == nullptr);
    Assert::IsTrue(*results == expected);
  }

private:
  void printVectorOfInt(std::vector<int> vector) {
    Logger::WriteMessage("Printing vector");
    for (auto iterator : vector) {
      Logger::WriteMessage(std::to_string(iterator).c_str());
    }
  }

  void printVectorOfStr(std::vector<std::string> vector) {
    Logger::WriteMessage("Printing vector");
    for (auto iterator : vector) {
      Logger::WriteMessage(iterator.c_str());
    }
  }

  void printMap(std::unordered_map<std::string, std::vector<std::string>> map) {
    for (auto iter : map) {
      Logger::WriteMessage(("Key: " + iter.first).c_str());
      printVectorOfStr(iter.second);
    }
  }

  };
}
