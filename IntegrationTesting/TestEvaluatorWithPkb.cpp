#include "stdafx.h"
#include "CppUnitTest.h"
#include "PkbWriteOnly.h"
#include "PKB.h"
#include "QueryEvaluator.h"
#include "Grammar.h"
#include "Relation.h"
#include "Pattern.h"

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
  public:
    TEST_METHOD_INITIALIZE(InitialisePkbAndEvaluator)
    {
      //Initialise PKB
      m_pkb = new PKB();
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 1);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 2);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 3);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 4);
      m_pkb->insertTypeOfStatementTable(1, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(2, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(3, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(4, queryType::GType::ASGN);

      std::list<STMT_NUM> stmtInLst;
      m_pkb->insertFollowsRelation(stmtInLst, 1);
      stmtInLst.push_back(1);

      m_pkb->insertFollowsRelation(stmtInLst, 2);
      stmtInLst.push_back(2);

      m_pkb->insertFollowsRelation(stmtInLst, 3);
      stmtInLst.push_back(3);

      m_pkb->insertFollowsRelation(stmtInLst, 4);
      stmtInLst.push_back(4);
      
      //Initialise PQL
      Grammar* g1 = new Grammar(2, "s");
      Grammar* g2 = new Grammar(2, "s1");
      Grammar* g3 = new Grammar(2, "s2");
      Relation* r1 = new Relation("Follows", *g2, *g3);
      m_selects.push(*g1);
      m_relations.push(*r1);
      m_synonymsUsedInQuery["s"] = 1;
      m_synonymsUsedInQuery["s1"] = 1;
      m_synonymsUsedInQuery["s2"] = 1;
      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_synonymsUsedInQuery);
    }

    TEST_METHOD(TestEvaluatorAndPkbForStmt)
    {
      std::vector<std::string> expectedResult = { "1", "2", "3", "4" };
      std::vector<std::string> actualResult = m_qe->evaluateQuery();
      Assert::AreEqual(expectedResult.size(), actualResult.size());
    }
  };
}