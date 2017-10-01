#include "stdafx.h"
#include "CppUnitTest.h"
#include "AssignTable.h"
#include "ASTBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestAssignTable) {
  public:

    const int DUMMY_VAR_INDEX = 0;

    TEST_METHOD(insertAssignRelation) {
      AssignTable assignTable;
      ASTBuilder builder;
      VariableNode* varNode = builder.createVariable(1, "x", DUMMY_VAR_INDEX);
      VariableNode* rhsVarNode = builder.createVariable(1, "y", DUMMY_VAR_INDEX);
      AssignNode* assignNode = builder.buildAssignment(1, varNode, rhsVarNode);
      
      VAR_INDEX varIndex= assignTable.insertAssignRelation(1, assignNode);
      Assert::AreEqual(varIndex, 1);
    }

    TEST_METHOD(getAllStmtList) {
      AssignTable assignTable;
      assignTable.insertAssignStmt(1);
      LIST_OF_STMT_NUMS stmts = assignTable.getAllAssignStmtList();
      
      int listSize = stmts.size();
      Assert::AreEqual(listSize, 1);
      Assert::AreEqual(stmts.front(), (STMT_NUM)1);

      assignTable.insertAssignStmt(2);
      stmts = assignTable.getAllAssignStmtList();
      listSize = stmts.size();
      Assert::AreEqual(listSize, 2);
      Assert::AreEqual(stmts.front() + 1, (STMT_NUM)2);

      assignTable.insertAssignStmt(3);
      stmts = assignTable.getAllAssignStmtList();
      listSize = stmts.size();
      Assert::AreEqual(listSize, 3);
      Assert::AreEqual(stmts.front() + 2, (STMT_NUM)3);
    }

    TEST_METHOD(getAllStmtListByVar)
    {
      AssignTable assignTable;
      ASTBuilder builder;
      VariableNode* varNode = builder.createVariable(1, "x", DUMMY_VAR_INDEX);
      VariableNode* rhsVarNode = builder.createVariable(1, "y", DUMMY_VAR_INDEX);
      AssignNode* assignNode = builder.buildAssignment(1, varNode, rhsVarNode);
      VAR_INDEX varIndex = 1;
      STMT_NUM curLineNo = 1;
      varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      std::list<STMT_NUM> stmts = assignTable.getAllAssignStmtListByVar(varIndex);
      int listSize = stmts.size();
      Assert::AreEqual(listSize, 1);
      Assert::AreEqual(stmts.front(), curLineNo);

      varIndex = 1;
      curLineNo = 2;
      assignNode = builder.buildAssignment(curLineNo, builder.createVariable(curLineNo, "x", DUMMY_VAR_INDEX), builder.createVariable(curLineNo, "x", DUMMY_VAR_INDEX));
      assignTable.insertAssignRelation(1, assignNode);
      stmts = assignTable.getAllAssignStmtListByVar(varIndex);
      listSize = stmts.size();
      Assert::AreEqual(listSize, 2);
      Assert::AreEqual(stmts.front() + 1, curLineNo);

      varIndex = 2;
      curLineNo = 3;
      assignNode = builder.buildAssignment(curLineNo, builder.createVariable(curLineNo, "y", DUMMY_VAR_INDEX), builder.createVariable(curLineNo, "c", DUMMY_VAR_INDEX));
      assignTable.insertAssignRelation(varIndex, assignNode);
      stmts = assignTable.getAllAssignStmtListByVar(varIndex);
      listSize = stmts.size();
      Assert::AreEqual(listSize, 1);
      Assert::AreEqual(stmts.front(), curLineNo);
    }

    TEST_METHOD(getAllAssignStmtWithVar)
    {
      AssignTable assignTable;
      ASTBuilder builder;
      VAR_INDEX varIndex = 1;
      STMT_NUM curLineNo = 1;
      VariableNode* varNode = builder.createVariable(curLineNo, "x", DUMMY_VAR_INDEX);
      VariableNode* rhsVarNode = builder.createVariable(curLineNo, "y", DUMMY_VAR_INDEX);
      AssignNode* assignNode = builder.buildAssignment(curLineNo, varNode, rhsVarNode);

      varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      VarTable varTable;
      varTable.insertModifiesForStmt("x", 1);
      assignTable.populateAssignToVarMap(&varTable);
      auto actual = assignTable.getAllAssignStmtWithVar();
      std::unordered_map<STMT_NUM, VAR_NAME> expected;
      expected.emplace((STMT_NUM)1, VAR_NAME("x"));
      Assert::IsTrue(actual == expected);
      //std::unordered_map<std::string, std::list<STMT_NUM>> stmtMap = assignTable.getAllAssignStmtWithVar();

      //int listSize = stmtMap.size();
      //Assert::AreEqual(listSize, 1);
      //std::unordered_map<std::string, std::list<STMT_NUM>>::iterator mapItr = stmtMap.find(std::string("x"));
      //Assert::AreEqual((*mapItr).first, std::string("x"));
      //Assert::AreEqual((int)(*mapItr).second.size(), 1);

      //varIndex = 1;
      //curLineNo = 2;
      //assignNode = builder.buildAssignment(curLineNo, builder.createVariable(curLineNo, "x", DUMMY_VAR_INDEX), builder.createVariable(curLineNo, "c", DUMMY_VAR_INDEX));
      //varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      //stmtMap = assignTable.getAllAssignStmtWithVar();

      //listSize = stmtMap.size();
      //Assert::AreEqual(listSize, 1);
      //mapItr = stmtMap.find(std::string("x"));
      //Assert::AreEqual((*mapItr).first, std::string("x"));
      //Assert::AreEqual((int)(*mapItr).second.size(), 2);

      //varIndex = 2;
      //curLineNo = 3;
      //assignNode = builder.buildAssignment(curLineNo, builder.createVariable(curLineNo, "y", DUMMY_VAR_INDEX), builder.createVariable(curLineNo, "c", DUMMY_VAR_INDEX));
      //varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      //stmtMap = assignTable.getAllAssignStmtWithVar();

      //listSize = stmtMap.size();
      //Assert::AreEqual(listSize, 2);
      //mapItr = stmtMap.find(std::string("y"));
      //Assert::AreEqual((*mapItr).first, std::string("y"));
      //Assert::AreEqual((int)(*mapItr).second.size(), 1);
    }
  };
}