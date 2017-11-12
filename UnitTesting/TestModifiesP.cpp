#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb/relationshipTables/ModifiesP.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestModifiesP) {
  public:
    ModifiesP *m_modifiesP;

    TEST_METHOD_INITIALIZE(instantiateModifiesP) {
      m_modifiesP = new ModifiesP;
      m_modifiesP->insertModifiesP(0, "F", 0, "a");
      m_modifiesP->insertModifiesP(0, "F", 1, "b");
      m_modifiesP->insertModifiesP(0, "F", 2, "c");
      m_modifiesP->insertModifiesP(1, "s", 2, "c");
    }

    TEST_METHOD_CLEANUP(cleanupModifiesP) 
    {
      delete m_modifiesP;
      m_modifiesP = nullptr;
    }

    TEST_METHOD(TestInsertModifiesP) 
    {
      int actualSize = m_modifiesP->getAllProcNames().size();
      int expectedSize = 2;
      Assert::AreEqual(actualSize, expectedSize);

      m_modifiesP->insertModifiesP(2, "t", 0, "a");
      actualSize = m_modifiesP->getAllProcNames().size();
      expectedSize = 3;
      Assert::AreEqual(actualSize, expectedSize);
    }

    TEST_METHOD(TestIsModifies)
    {
      Assert::IsTrue(m_modifiesP->isModifiesP(0, 0));
      Assert::IsTrue(m_modifiesP->isModifiesP(1, 2));
      Assert::IsFalse(m_modifiesP->isModifiesP(3, 2));
      Assert::IsFalse(m_modifiesP->isModifiesP(0, 4));
    }

    TEST_METHOD(TestIsModifiesInProc) 
    {
      Assert::IsTrue(m_modifiesP->isModifiesInProc(0));
      Assert::IsTrue(m_modifiesP->isModifiesInProc(1));
      Assert::IsFalse(m_modifiesP->isModifiesInProc(2));
      Assert::IsFalse(m_modifiesP->isModifiesInProc(4));
    }

    TEST_METHOD(TestGetVarNamesWithProcIdx) 
    {
      auto actualList = m_modifiesP->getVarNamesWithProcIdx(0);
      LIST_OF_VAR_NAMES expectedList = { "a", "b", "c" };
      Assert::IsTrue(actualList == expectedList);

      actualList = m_modifiesP->getVarNamesWithProcIdx(1);
      Assert::IsFalse(actualList == expectedList);
      expectedList = { "c" };
      Assert::IsTrue(actualList == expectedList);

      actualList = m_modifiesP->getVarNamesWithProcIdx(3);
      Assert::IsTrue(actualList.empty());
    }

    TEST_METHOD(getProcNamesWithVarIdx)
    {
      auto actualList = m_modifiesP->getProcNamesWithVarIdx(0);
      LIST_OF_PROC_NAMES expectedList = { "F" };
      Assert::IsTrue(actualList == expectedList);
      
      actualList = m_modifiesP->getProcNamesWithVarIdx(2);
      expectedList = { "F", "s" };
      Assert::IsTrue(actualList == expectedList);

      actualList = m_modifiesP->getProcNamesWithVarIdx(5);
      expectedList = {};
      Assert::IsTrue(actualList == expectedList);
    }

    TEST_METHOD(TestGetAllProcToVar) 
    {
      auto actualMap = m_modifiesP->getAllProcToVar();
      MAP_OF_PROC_TO_VAR expectedMap = {  {"F", "a"}, 
                                          {"F", "b"}, 
                                          {"F", "c"}, 
                                          {"s", "c"} };
      Assert::IsTrue(actualMap == expectedMap);
    }

    TEST_METHOD(TestGetAllProcNames)
    {
      auto actualList = m_modifiesP->getAllProcNames();
      LIST_OF_PROC_NAMES expectedList = { "F", "s" };
      Assert::IsTrue(actualList == expectedList);
    }
  };
}