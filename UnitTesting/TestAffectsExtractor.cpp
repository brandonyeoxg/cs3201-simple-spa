#include "stdafx.h"
#include "CppUnitTest.h"
#include "AffectsExtractor.h"
#include "PKB.h"
#include "PkbTablesOnly.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestAffectsExtractor) {
  public:
    AffectsExtractor* m_affectsExtractor;
    //PkbTablesOnly* m_pkb;
    PKB* m_pkb = new PKB();

    TEST_METHOD_INITIALIZE(initTestAffectsExtractor) {
      m_affectsExtractor = new AffectsExtractor(m_pkb);
      //initialise pkb procTable

    }

  };
}