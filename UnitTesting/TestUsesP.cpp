#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb/relationshipTables/UsesP.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestUsesP) {
public:
  UsesP *m_UsesP;

  TEST_METHOD_INITIALIZE(instantiateUsesP) {
    m_UsesP = new UsesP;
    m_UsesP->insertUsesP(0,"F", 0, "a");
    m_UsesP->insertUsesP(0, "F", 1, "b");
    m_UsesP->insertUsesP(0, "F", 2, "c");
    m_UsesP->insertUsesP(1, "s", 2, "c");
  }

  TEST_METHOD_CLEANUP(cleanupUsesP)
  {
    delete m_UsesP;
    m_UsesP = nullptr;
  }

  TEST_METHOD(TestInsertUsesP)
  {
    int actualSize = m_UsesP->getAllProcNames().size();
    int expectedSize = 2;
    Assert::AreEqual(actualSize, expectedSize);

    m_UsesP->insertUsesP(2, "t", 0, "a");
    actualSize = m_UsesP->getAllProcNames().size();
    expectedSize = 3;
    Assert::AreEqual(actualSize, expectedSize);
  }

  TEST_METHOD(TestIsUses)
  {
    Assert::IsTrue(m_UsesP->isUsesP(0, 0));
    Assert::IsTrue(m_UsesP->isUsesP(1, 2));
    Assert::IsFalse(m_UsesP->isUsesP(3, 2));
    Assert::IsFalse(m_UsesP->isUsesP(0, 4));
  }

  TEST_METHOD(TestIsUsesInProc)
  {
    Assert::IsTrue(m_UsesP->isUsesInProc(0));
    Assert::IsTrue(m_UsesP->isUsesInProc(1));
    Assert::IsFalse(m_UsesP->isUsesInProc(2));
    Assert::IsFalse(m_UsesP->isUsesInProc(4));
  }

  TEST_METHOD(TestGetVarNamesWithProcIdx)
  {
    auto actualList = m_UsesP->getVarNamesWithProcIdx(0);
    LIST_OF_VAR_NAMES expectedList = { "a", "b", "c" };
    Assert::IsTrue(actualList == expectedList);

    actualList = m_UsesP->getVarNamesWithProcIdx(1);
    Assert::IsFalse(actualList == expectedList);
    expectedList = { "c" };
    Assert::IsTrue(actualList == expectedList);

    actualList = m_UsesP->getVarNamesWithProcIdx(3);
    Assert::IsTrue(actualList.empty());
  }

  TEST_METHOD(getProcNamesWithVarIdx)
  {
    auto actualList = m_UsesP->getProcNamesWithVarIdx(0);
    LIST_OF_PROC_NAMES expectedList = { "F" };
    Assert::IsTrue(actualList == expectedList);

    actualList = m_UsesP->getProcNamesWithVarIdx(2);
    expectedList = { "F", "s" };
    Assert::IsTrue(actualList == expectedList);

    actualList = m_UsesP->getProcNamesWithVarIdx(5);
    expectedList = {};
    Assert::IsTrue(actualList == expectedList);
  }

  TEST_METHOD(TestGetAllProcToVar)
  {
    auto actualMap = m_UsesP->getAllProcToVar();
    MAP_OF_PROC_TO_VAR expectedMap = {{ "F", "a" },
                                      { "F", "b" },
                                      { "F", "c" },
                                      { "s", "c" } };
    Assert::IsTrue(actualMap == expectedMap);
  }

  TEST_METHOD(TestGetAllProcNames)
  {
    auto actualList = m_UsesP->getAllProcNames();
    LIST_OF_PROC_NAMES expectedList = { "F", "s" };
    Assert::IsTrue(actualList == expectedList);
  }
  };
}