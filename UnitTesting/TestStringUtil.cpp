#include "stdafx.h"
#include "CppUnitTest.h"
#include "StringUtil.h"

#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestStringUtil) {
  public:
    TEST_METHOD(splitString) {
      std::string normalInput = "Hello World";
      std::vector<std::string> actual;
      actual = StringUtil::splitString(normalInput, ' ');

      std::vector<std::string> expected = { "Hello", "World" };

      for (unsigned int i = 0; i < actual.size(); i++) {
        Assert::AreEqual(expected.at(i), actual.at(i));
      }
    }

    TEST_METHOD(reduceString) {
      std::string normalInput = " Hello  world  ";

      std::string actual = StringUtil::reduceString(normalInput);

      std::string expected = "Hello world";
      Assert::AreEqual(actual, expected);
    }

    TEST_METHOD(splitStringWithReduceString) {
      std::string normalInput = "   Hello  World   ";
      std::vector<std::string> actual;
      normalInput = StringUtil::reduceString(normalInput);
      actual = StringUtil::splitString(normalInput, ' ');

      std::vector<std::string> expected = { "Hello", "World" };

      for (unsigned int i = 0; i < actual.size(); i++) {
        Assert::AreEqual(expected.at(i), actual.at(i));
      }
    }
  };
}