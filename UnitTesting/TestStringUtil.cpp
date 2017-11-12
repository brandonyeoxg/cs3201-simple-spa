#include "stdafx.h"
#include "CppUnitTest.h"
#include "util/StringUtil.h"

#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestStringUtil) {
  public:
    TEST_METHOD(reduceString) {
      std::string expected = "Hello world";

      std::string normalInput = " Hello  world  ";
      std::string actual = StringUtil::reduceString(normalInput);
      Assert::AreEqual(actual, expected);

      normalInput = "Hello world";
      actual = StringUtil::reduceString(normalInput);
      Assert::AreEqual(actual, expected);

      normalInput = "Helloworld";
      actual = StringUtil::reduceString(normalInput);
      Assert::AreNotEqual(actual, expected);

      normalInput = "";
      expected = "";
      actual = StringUtil::reduceString(normalInput);
      Assert::AreEqual(actual, expected);
    }

    TEST_METHOD(trimString) {
      std::string expected = "Hello world";

      std::string normalInput = " Hello world ";
      std::string actual = StringUtil::trimString(normalInput);
      Assert::AreEqual(actual, expected);

      normalInput = "Hello world ";
      actual = StringUtil::trimString(normalInput);
      Assert::AreEqual(actual, expected);

      normalInput = " Hello world";
      actual = StringUtil::trimString(normalInput);
      Assert::AreEqual(actual, expected);

      normalInput = " ";
      expected = "";
      actual = StringUtil::trimString(normalInput);
      Assert::AreEqual(actual, expected);

      normalInput = "";
      actual = StringUtil::trimString(normalInput);
      Assert::AreEqual(actual, expected);
    }
  };
}