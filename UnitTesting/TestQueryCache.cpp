#include "stdafx.h"
#include "CppUnitTest.h"
#include "pql/QueryCache.h"
#include "Pattern.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestQueryCache) {
public:
  TEST_METHOD(testQueryCache) {
    Pattern pattern = Pattern(Grammar(4, "smth"), Grammar(), Grammar(), false);
    QueryCache cache = QueryCache();

    std::string key = cache.getKeyWithPattern(pattern);

    Logger::WriteMessage(key.c_str());

    Relation relation = Relation("Next", Grammar(), Grammar());
    key = cache.getKeyWithRelation(relation);

    Logger::WriteMessage(key.c_str());
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
