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
      std::list<STMT_NUM> nestedStmtLine;

      m_pkb->insertParentRelation(nestedStmtLine, 1);
      nestedStmtLine.push_back(1);

      m_pkb->insertParentRelation(nestedStmtLine, 2);
      nestedStmtLine.push_back(2);

      m_pkb->insertParentRelation(nestedStmtLine, 3);
      nestedStmtLine.push_back(3);

      m_pkb->insertParentRelation(nestedStmtLine, 4);
      nestedStmtLine.push_back(4);

      m_designExtractor = new DesignExtractor(m_pkb);
    }

    TEST_METHOD_CLEANUP(cleanupDummyPKB) 
    {
      delete m_pkb;
      delete m_designExtractor;
    }

  };
}