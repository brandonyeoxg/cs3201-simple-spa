#include "stdafx.h"
#include "CppUnitTest.h"
#include "ConstantTable.h"
#include <stdexcept>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestConstantTable) {
  public:
    TEST_METHOD(TestInsertConstant) {
      ConstantTable *constantTable = new ConstantTable();
      int index = constantTable->insertConstant("25");
      index = constantTable->insertConstant("378");
      Assert::IsTrue(index == 1);
    }

    TEST_METHOD(TestGetAllConstants) {
      ConstantTable *constantTable = new ConstantTable();
      int index = constantTable->insertConstant("25");
      index = constantTable->insertConstant("378");
      LIST_OF_RESULTS expected = constantTable->getAllConstants();

      LIST_OF_RESULTS actual;
      actual.push_back("25");
      actual.push_back("378");

      Assert::IsTrue(expected == actual);
    }
  };
}