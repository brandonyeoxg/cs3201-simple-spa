#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowTable.h"
#include "ParentTable.h"
#include "VarTable.h"
#include "PKB.h"
#include "Grammar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestFollowTable) {
  public:

    TEST_METHOD(TestPKBInsertFollow) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      PKB *pkb = new PKB();
      std::list<STMT_NUM> stmtInLst;
      pkb->insertFollowsRelation(stmtInLst, 1);
      stmtInLst.push_back(1);

      pkb->insertFollowsRelation(stmtInLst, 2);
      stmtInLst.push_back(2);

      pkb->insertFollowsRelation(stmtInLst, 3);
      stmtInLst.push_back(3);

      pkb->insertFollowsRelation(stmtInLst, 4);
      stmtInLst.push_back(4);

      Assert::IsTrue(pkb->getFollowTable()->getFollowTable() == test);
    }

    TEST_METHOD(TestPKBGetFollows) {
      std::unordered_map<int, std::vector<int>> test = {
        { 1,{ 2, 3, 4 } },
        { 2,{ 3, 4 } },
        { 3,{ 4 } }
      };
      Logger::WriteMessage("Running follow table test getFollows");
      PKB *pkb = new PKB();
      pkb->getFollowTable()->setFollowTable(test);
      //test getFollows method (correct behaviour)
      int expected = pkb->getFollows(1);
      Assert::IsTrue(expected == 2);
    }

    TEST_METHOD(TestPKBTypeOfStatementTable) {
      std::unordered_map<int, queryType::GType> testTypeOfStatementTable = {
        {1, queryType::GType::ASGN},
        {2, queryType::GType::WHILE },
        {3, queryType::GType::IF }
      };     
      PKB *pkb = new PKB();
      pkb->insertTypeOfStatementTable(1, queryType::GType::ASGN);
      pkb->insertTypeOfStatementTable(2, queryType::GType::WHILE);
      pkb->insertTypeOfStatementTable(3, queryType::GType::IF);
      Assert::IsTrue(testTypeOfStatementTable == pkb->getTypeOfStatementTable());
      //test duplicate line number.
      bool actual = pkb->insertTypeOfStatementTable(1, queryType::GType::WHILE);
      //Assert::IsFalse(actual);

    }

    TEST_METHOD(TestPKBInsertStatementTypeTable) {
      std::unordered_map<queryType::GType, std::vector<int>> testStatementTypeTable = {
        { queryType::GType::ASGN, {1, 2, 3}},
        { queryType::GType::WHILE, {4, 5}},
        { queryType::GType::IF, {6}}
      };
      PKB *pkb = new PKB();
      pkb->insertStatementTypeTable(queryType::GType::ASGN, 1);
      pkb->insertStatementTypeTable(queryType::GType::ASGN, 2);
      pkb->insertStatementTypeTable(queryType::GType::ASGN, 3);
      pkb->insertStatementTypeTable(queryType::GType::WHILE, 4);
      pkb->insertStatementTypeTable(queryType::GType::WHILE, 5);
      pkb->insertStatementTypeTable(queryType::GType::IF, 6);
      Assert::IsTrue(testStatementTypeTable == pkb->getStatementTypeTable());
      //test for duplicate entry. 
      bool actual = pkb->insertStatementTypeTable(queryType::GType::IF, 6);
      Assert::IsFalse(actual);

    }

    TEST_METHOD(TestPKBGetAssignStmtByVarPattern) 
    {
      PKB *pkb = new PKB();
      ASTBuilder builder;
      STMT_NUM stmtNum = 1;
      //1) x = y
      VariableNode *varLeftNode = builder.createVariable(stmtNum, "x", 0);
      pkb->insertModifiesForStmt("x", stmtNum);
      VariableNode *varRightNode = builder.createVariable(stmtNum, "y", 1);
      pkb->insertUsesForStmt("y", stmtNum);
      AssignNode *aNode = builder.buildAssignment(stmtNum, varLeftNode, varRightNode);
      pkb->getAssignTable()->insertAssignRelation(0, aNode);
      pkb->populateAssignTableAbstractions();

      auto actual = pkb->getAssignStmtByVarPattern("x", "y", true);
      std::list<STMT_NUM> expected = { 1 };
      Assert::IsTrue(actual == expected);
      //TODO once jazlyn is done
      //actual = pkb->getAssignStmtByVarPattern("x", "y", false);
      //Assert::IsTrue(actual == expected);
      
      //2) x = y + 5
      varLeftNode = builder.createVariable(2, "x", 0);
      varRightNode = builder.createVariable(2, "y", 1);
      ConstantNode* constRightNode = builder.createConstant(2, 5);
      TNode* expNode = builder.buildAddition(2, varRightNode, constRightNode);
      aNode = builder.buildAssignment(2, varLeftNode, expNode);
      pkb->getAssignTable()->insertAssignRelation(0, aNode);
      pkb->populateAssignTableAbstractions();

      actual = pkb->getAssignStmtByVarPattern("x", "y", true);
      expected = { 1 };
      Assert::IsTrue(actual == expected);

      // TODO once jazlyn is done
      //actual = pkb->getAssignStmtByVarPattern("x", "y", false);
      //expected = { 1, 2};
      expected = {};
      actual = pkb->getAssignStmtByVarPattern("y", "y", true);
      Assert::IsTrue(actual == expected);
    }
    //TODO: combine the two stmt tables for insertion, test for duplicate line number entries.
    TEST_METHOD(getAllAssignStmtAndVar)
    {
      PKB *pkb = new PKB();
      ASTBuilder builder;
      STMT_NUM stmtNum = 1;

      //1) x = y + z
      VariableNode *varLeftNode = builder.createVariable(stmtNum, "x", 0);
      pkb->insertModifiesForStmt("x", stmtNum);
      VariableNode *parseFirstVar = builder.createVariable(stmtNum, "y", 1);
      VariableNode *parseSecondVar = builder.createVariable(stmtNum, "z", 2);
      pkb->insertUsesForStmt("y", stmtNum);
      pkb->insertUsesForStmt("z", stmtNum);
      TNode* expNode = builder.buildAddition(1, parseFirstVar, parseSecondVar);
      AssignNode *aNode = builder.buildAssignment(stmtNum, varLeftNode, expNode);
      pkb->getAssignTable()->insertAssignRelation(0, aNode);

      //2) x = y + 5
      varLeftNode = builder.createVariable(2, "x", 0);
      parseFirstVar = builder.createVariable(2, "y", 1);
      ConstantNode* constRightNode = builder.createConstant(2, 5);
      expNode = builder.buildAddition(2, parseFirstVar, constRightNode);
      aNode = builder.buildAssignment(2, varLeftNode, expNode);
      pkb->getAssignTable()->insertAssignRelation(0, aNode);
      pkb->populateAssignTableAbstractions();

      auto actualMap = pkb->getAllAssignStmtAndVarByPattern("y", false);
      std::unordered_map<STMT_NUM, VAR_NAME> expectedMap = { {1, "x"}, {2, "x"} };
      Assert::IsTrue(actualMap == expectedMap);
    }

  };
}