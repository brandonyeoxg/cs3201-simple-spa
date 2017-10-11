#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "DesignExtractor.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestDesignExtractor)
  {
  public:
    PKB* m_pkb;
    DesignExtractor *m_designExtractor;

    TEST_METHOD_INITIALIZE(initaliseDummyPKB)
    {
      m_pkb = new PKB();
      m_pkb->insertParent(1, 2);
      m_pkb->insertParent(1, 3);
      m_pkb->insertParent(2, 4);

      m_designExtractor = new DesignExtractor(m_pkb);
    }

    TEST_METHOD_CLEANUP(cleanupDummyPKB) 
    {
      delete m_pkb;
      delete m_designExtractor;
    }

  };
}