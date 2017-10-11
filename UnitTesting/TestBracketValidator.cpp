#include "stdafx.h"
#include "CppUnitTest.h"
#include "BracketValidator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestBracketValidator) {
  public:
    BracketValidator *m_bracketValidator;
    TEST_METHOD_INITIALIZE(initialiseBracketValidator) 
    {
      m_bracketValidator = new BracketValidator;
    }
    TEST_METHOD_CLEANUP(cleanUpBracketValidator)
    {
      delete m_bracketValidator;
    }
    TEST_METHOD(TestInsertBracketToken) 
    {
      // "("
      m_bracketValidator->insertBracketToken("(");
      Assert::AreEqual(m_bracketValidator->getBraceStack().size(), size_t(1));
      
      // "()"
      m_bracketValidator->insertBracketToken(")");
      Assert::AreEqual(m_bracketValidator->getBraceStack().size(), size_t(0));

      // "())"
      m_bracketValidator->insertBracketToken(")");
      Assert::AreEqual(m_bracketValidator->getBraceStack().size(), size_t(1));

      // "())("
      m_bracketValidator->insertBracketToken("(");
      Assert::AreEqual(m_bracketValidator->getBraceStack().size(), size_t(2));

      // "())(("
      m_bracketValidator->insertBracketToken("(");
      Assert::AreEqual(m_bracketValidator->getBraceStack().size(), size_t(3));
    }
    TEST_METHOD(TestIsWellFormed)
    {
      // "("
      m_bracketValidator->insertBracketToken("(");
      Assert::IsFalse(m_bracketValidator->isWellFormed());

      // "()"
      m_bracketValidator->insertBracketToken(")");
      Assert::IsTrue(m_bracketValidator->isWellFormed());

      // "())"
      m_bracketValidator->insertBracketToken(")");
      Assert::IsFalse(m_bracketValidator->isWellFormed());

      // "())("
      m_bracketValidator->insertBracketToken("(");
      Assert::IsFalse(m_bracketValidator->isWellFormed());

      // "())(("
      m_bracketValidator->insertBracketToken("(");
      Assert::IsFalse(m_bracketValidator->isWellFormed());
    }
  };
}