#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql/QueryCache.h"
#include "Pattern.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestQueryCache) {
public:
  TEST_METHOD(testKey) {
    Pattern pattern = Pattern(Grammar(4, "smth"), Grammar(), Grammar(), false);
    QueryCache cache = QueryCache();
    std::string key;

    key = cache.getKey(pattern);

    Logger::WriteMessage(key.c_str());

    Relation relation = Relation("Next*", Grammar(11, "_"), Grammar());
    key = cache.getKey(relation);

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
  }

  TEST_METHOD(isPatternType_isRelationType) {
    Clause clause;

    clause = Pattern(Grammar(4, "smth"), Grammar(), Grammar(), false);
    Assert::IsTrue(clause.isPatternType());
    Assert::IsFalse(clause.isRelationType());

    clause = Relation("Next*", Grammar(11, "_"), Grammar());
    Assert::IsTrue(clause.isRelationType());
    Assert::IsFalse(clause.isPatternType());
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
