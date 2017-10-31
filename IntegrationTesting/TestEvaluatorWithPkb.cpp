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
      PROC_INDEX idx = m_pkb->insertProcedure("First");

      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 1);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 2);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 3);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 4);
      m_pkb->insertStatementTypeTable(queryType::GType::WHILE, 5);
      m_pkb->insertStatementTypeTable(queryType::GType::ASGN, 6);
      m_pkb->insertStatementTypeTable(queryType::GType::CALL, 7);
      m_pkb->insertStatementTypeTable(queryType::GType::CALL, 8);
      m_pkb->insertTypeOfStatementTable(1, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(2, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(3, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(4, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(5, queryType::GType::WHILE);
      m_pkb->insertTypeOfStatementTable(6, queryType::GType::ASGN);
      m_pkb->insertTypeOfStatementTable(7, queryType::GType::CALL);
      m_pkb->insertTypeOfStatementTable(8, queryType::GType::CALL);

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
      
      m_pkb->getFollowTable()->populateAllFollowsMap();

      //insert parent
      LIST_OF_STMT_NUMS stmtInLst2;
      stmtInLst2.push_back(5);
      m_pkb->insertParentRelation(stmtInLst2, 6);
      stmtInLst2.push_back(6);

      //insert uses
      m_pkb->insertUsesForStmt("x", 1);
      m_pkb->insertUsesForStmt("c", 1);
      m_pkb->insertUsesForStmt("x", 2);
      m_pkb->insertUsesForStmt("a", 3);
      m_pkb->insertUsesForStmt("b", 4); 
      m_pkb->insertVar("x");
      m_pkb->insertVar("c");
      m_pkb->insertVar("a");
      m_pkb->insertVar("b");


      //insert modifies
      m_pkb->insertModifiesForStmt("y", 1);
      m_pkb->insertModifiesForStmt("z", 2);
      m_pkb->insertModifiesForStmt("w", 3);
      m_pkb->insertModifiesForStmt("z", 4);
      m_pkb->insertVar("y");
      m_pkb->insertVar("z");
      m_pkb->insertVar("w");
      
      //insert calls
      m_pkb->insertProcedure("Second");
      m_pkb->insertCallStmt(idx, "Second", 7);
      m_pkb->insertProcedure("Third");
      m_pkb->insertCallStmt(idx, "Third", 8);

      //insert next
      m_pkb->insertNextRelation(1, 2);
      m_pkb->insertNextRelation(2, 3);
      m_pkb->insertNextRelation(3, 4);
      m_pkb->insertNextRelation(4, 5);
      m_pkb->insertNextRelation(5, 6);
      m_pkb->insertNextRelation(6, 5);
      m_pkb->insertNextRelation(5, 7);
      m_pkb->insertNextRelation(7, 8);

      //SIMPLE program as follows:
      /*Procedure First {
      * y = x + c;
      * z = x;
      * w = a;
      * z = b;
      * while i {
      *   x = ok; }
      * calls Second;
      * calls Third; }
      */
    }

    TEST_METHOD(TestEvaluatorAndPkbForFollows)
    {
      //Select s such that follow(s1, s2)
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
      std::vector<std::string> expectedResult = { "1", "2", "3", "4", "5", "6", "7", "8" };
      std::vector<std::string> actualResult = m_qe->evaluateQuery();
      Assert::AreEqual(expectedResult.size(), actualResult.size());

      //Select s such that follow*(2, s)
      std::swap(m_selects, m_emptySelects);
      std::swap(m_relations, m_emptyRelations);
      Grammar* g4 = new Grammar(10, "2");
      Relation* r2 = new Relation("Follows*", *g4, *g1);
      m_selects.push(*g1);
      m_relations.push(*r2);
      
      m_synonymsUsedInQuery.clear(); 
      m_synonymsUsedInQuery["s"] = 2;

      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_withs, m_synonymsUsedInQuery);
      expectedResult = { "3", "4" };
      actualResult = m_qe->evaluateQuery();
      Assert::IsTrue(actualResult == expectedResult);
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

      //Select s such that parent(s1, s2)
      m_selects.push(*g1);
      m_relations.push(*r1);
      m_synonymsUsedInQuery["s"] = 1;
      m_synonymsUsedInQuery["s1"] = 1;
      m_synonymsUsedInQuery["s2"] = 1;
      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_withs, m_synonymsUsedInQuery);
      std::vector<std::string> expectedResult = { "1", "2", "3", "4", "5", "6", "7", "8" };
      std::vector<std::string> actualResult = m_qe->evaluateQuery();
      std::sort(actualResult.begin(), actualResult.end());
      Assert::IsTrue(actualResult == expectedResult );

      //Select s such that parent(5, s)
      m_synonymsUsedInQuery.clear();
      m_synonymsUsedInQuery["s"] = 2;
      std::swap(m_relations, m_emptyRelations);
      Grammar* g4 = new Grammar(10, "5");
      Relation* r2 = new Relation("Parent", *g4, *g1);
      m_relations.push(*r2);
      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_withs, m_synonymsUsedInQuery);
      expectedResult = { "6" };
      actualResult = m_qe->evaluateQuery();
      std::sort(actualResult.begin(), actualResult.end());
      Assert::IsTrue(actualResult == expectedResult);
    }
    TEST_METHOD(TestEvaluatorAndPkbForUses)
    {
      Grammar* g1 = new Grammar(7, "v");
      Grammar* g2 = new Grammar(10, "1");
      Relation* r1 = new Relation("Uses", *g2, *g1);

      //Select v such that uses(1, v)
      //clear the queues
      std::swap(m_selects, m_emptySelects);
      std::swap(m_relations, m_emptyRelations);
      m_selects.push(*g1);
      m_relations.push(*r1);
      m_synonymsUsedInQuery["v"] = 2;
      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_withs, m_synonymsUsedInQuery);
      std::vector<std::string> expectedResult = { "x", "c" };
      std::vector<std::string> actualResult = m_qe->evaluateQuery();
      Assert::IsTrue(actualResult == expectedResult);
    }

    TEST_METHOD(TestEvaluatorAndPkbForModifies)
    {
      Grammar* g1 = new Grammar(7, "v");
      Grammar* g2 = new Grammar(10, "3");
      Relation* r1 = new Relation("Modifies", *g2, *g1);

      //Select v such that modifies(3, v)
      //clear the queues
      std::swap(m_selects, m_emptySelects);
      std::swap(m_relations, m_emptyRelations);
      m_selects.push(*g1);
      m_relations.push(*r1);
      m_synonymsUsedInQuery["v"] = 2;
      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_withs, m_synonymsUsedInQuery);
      std::vector<std::string> expectedResult = { "w" };
      std::vector<std::string> actualResult = m_qe->evaluateQuery();
      Assert::IsTrue(actualResult == expectedResult);
    }

    TEST_METHOD(TestEvaluatorAndPkbForCalls)
    {
      //Select p such that calls(p, "Third")

      Grammar* g1 = new Grammar(0, "Third");
      Grammar* g2 = new Grammar(0, "p");
      Relation* r1 = new Relation("Calls", *g2, *g1);
      //clear the queues
      std::swap(m_selects, m_emptySelects);
      std::swap(m_relations, m_emptyRelations);
      m_selects.push(*g2);
      m_relations.push(*r1);
      m_synonymsUsedInQuery["p"] = 2;
      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_withs, m_synonymsUsedInQuery);
      std::vector<std::string> expectedResult = { "First" };
      std::vector<std::string> actualResult = m_qe->evaluateQuery();
      Assert::IsTrue(actualResult == expectedResult);
    }

    TEST_METHOD(TestEvaluatorAndPkbForNext)
    {
      //Select n such that next(5, n)
      Grammar* g1 = new Grammar(2, "n");
      Grammar* g2 = new Grammar(10, "5");
      Relation* r1 = new Relation("Next", *g2, *g1);
      //clear the queues
      std::swap(m_selects, m_emptySelects);
      std::swap(m_relations, m_emptyRelations);
      m_selects.push(*g1);
      m_relations.push(*r1);
      m_synonymsUsedInQuery.clear();
      m_synonymsUsedInQuery["n"] = 2;
      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_withs, m_synonymsUsedInQuery);
      std::vector<std::string> expectedResult = { "6", "7" };
      std::vector<std::string> actualResult = m_qe->evaluateQuery();
      Assert::IsTrue(actualResult == expectedResult);

      //Select n such that next*(5, n)
      Relation* r2 = new Relation("Next*", *g2, *g1);
      //clear the queues
      std::swap(m_relations, m_emptyRelations);
      m_relations.push(*r2);
      m_qe = new QueryEvaluator(m_pkb, m_selects, m_relations, m_patterns, m_withs, m_synonymsUsedInQuery);
      expectedResult = { "5", "6", "7", "8" };
      actualResult = m_qe->evaluateQuery();
      std::sort(actualResult.begin(), actualResult.end());
      Assert::IsTrue(actualResult == expectedResult);
    }
  };
}