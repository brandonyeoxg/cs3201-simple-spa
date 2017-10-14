#include "stdafx.h"
#include "CppUnitTest.h"
#include "PkbWriteOnly.h"
#include "PKB.h"
#include "QueryEvaluator.h"
#include "Grammar.h"
#include "Relation.h"
#include "Pattern.h"
#include "With.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
  TEST_CLASS(TestEvaluatorWithPkb)
  {
  private:
    PKB* m_pkb;
    QueryEvaluator* m_qe;
    std::unordered_map<std::string, int> m_synonymsUsedInQuery; 
    std::queue<Grammar> m_selects;
    std::queue<Relation> m_relations;
    std::queue<Pattern> m_patterns;
    std::queue<With> m_withs;
    std::queue<Grammar> m_emptySelects;
    std::queue<Relation> m_emptyRelations;
    std::queue<Pattern> m_emptyPatterns;

  public:
    TEST_METHOD_INITIALIZE(InitialisePkbAndEvaluator)
    {
      //Initialise PKB
      m_pkb = new PKB();
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 1);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 2);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 3);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 4);
      m_pkb->insertStatementTypeTable(queryType::GType::WHILE, 5);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 6);
      m_pkb->insertTypeOfStatementTable(1, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(2, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(3, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(4, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(5, queryType::GType::WHILE);
      m_pkb->insertTypeOfStatementTable(6, queryType::GType::ASGN);

      LIST_OF_STMT_NUMS stmtInLst;
      //insert follows
      m_pkb->insertFollowsRelation(stmtInLst, 1);
      stmtInLst.push_back(1);

      m_pkb->insertFollowsRelation(stmtInLst, 2);
      stmtInLst.push_back(2);

      m_pkb->insertFollowsRelation(stmtInLst, 3);
      stmtInLst.push_back(3);

      m_pkb->insertFollowsRelation(stmtInLst, 4);
      stmtInLst.push_back(4);
      
      //insert parent
      LIST_OF_STMT_NUMS stmtInLst2;
      stmtInLst2.push_back(5);
      m_pkb->insertParentRelation(stmtInLst2, 6);
      stmtInLst2.push_back(6);
      
    }

    TEST_METHOD(TestEvaluatorAndPkbForFollows)
    {
      Grammar* g1 = new Grammar(2, "s");
      Grammar* g2 = new Grammar(2, "s1");
      Grammar* g3 = new Grammar(2, "s2");
      Relation* r1 = new Relation("Follows", *g2, *g3);
      m_selects.push(*g1);
      m_relations.push(*r1);
      m_synonymsUsedInQuery["s"] = 1;
      m_synonymsUsedInQuery["s1"] = 1;
      m_synonymsUsedInQuery["s2"] = 1;
      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_withs, m_synonymsUsedInQuery);
      std::vector<std::string> expectedResult = { "1", "2", "3", "4", "5", "6" };
      std::vector<std::string> actualResult = m_qe->evaluateQuery();
      Assert::AreEqual(expectedResult.size(), actualResult.size());

      delete m_qe;
    }

    TEST_METHOD(TestEvaluatorAndPkbForParent)
    {
      Grammar* g1 = new Grammar(2, "s");
      Grammar* g2 = new Grammar(2, "s1");
      Grammar* g3 = new Grammar(2, "s2");
      Relation* r1 = new Relation("Parent", *g2, *g3);

      //clear the queues
      std::swap(m_selects, m_emptySelects);
      std::swap(m_relations, m_emptyRelations);
      std::swap(m_patterns, m_emptyPatterns);

      m_selects.push(*g1);
      m_relations.push(*r1);
      m_synonymsUsedInQuery["s"] = 1;
      m_synonymsUsedInQuery["s1"] = 1;
      m_synonymsUsedInQuery["s2"] = 1;
      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_withs, m_synonymsUsedInQuery);
      std::vector<std::string> expectedResult = { "1", "2", "3", "4", "5", "6" };
      std::vector<std::string> actualResult = m_qe->evaluateQuery();
      std::sort(actualResult.begin(), actualResult.end());
      Assert::IsTrue(actualResult == expectedResult );
    }

  };
}