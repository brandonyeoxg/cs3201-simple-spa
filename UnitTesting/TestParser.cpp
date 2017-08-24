#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include <cstdio>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
  TEST_CLASS(TestParser)
  {
  public:

    TEST_METHOD(parse)
    {
      //std::string tmpName = std::tmpnam(nullptr);
      //std::FILE* tmpFile = fopen(tmpName.c_str(), "w");
      //if (tmpFile != nullptr) {
      //  fputs("Hello world\n", tmpFile);
      //  fclose(tmpFile);
      //}

      //Parser *parser = new Parser(tmpName);
      //int actual = parser->parse();
      //Assert::AreEqual(0, actual);
    }
  };
}