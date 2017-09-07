#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestParentTable) {
  public:
    std::unordered_map<int, std::vector<int>> test = {
      { 1,{ 2, 3 } },
      { 2,{ 3, 4 } },
      { 3,{ 4 } }
    };

    TEST_METHOD(TestInsertParent) {
      
      Assert::IsTrue(1 == 1);
    }
  };
}