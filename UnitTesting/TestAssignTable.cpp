#include "stdafx.h"
#include "CppUnitTest.h"
#include "AssignTable.h"
#include "ASTBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
  TEST_CLASS(TestAssignTable) {
  public:
    TEST_METHOD(insertAssignRelation) {
      AssignTable assignTable;
      ASTBuilder builder;
      VariableNode* varNode = builder.createVariable(1, "x");
      VariableNode* rhsVarNode = builder.createVariable(1, "y");
      AssignNode* assignNode = builder.buildAssignment(1, varNode, rhsVarNode);
      
      VAR_INDEX_NO varIndex= assignTable.insertAssignRelation(1, assignNode);
      Assert::AreEqual(varIndex, 1);
    }

    TEST_METHOD(getAllStmtList) {
      AssignTable assignTable;
      ASTBuilder builder;
      STMT_NO curStmtNo = 1;
      VAR_INDEX_NO varIndex = 1;
      VariableNode* varNode = builder.createVariable(curStmtNo, "x");
      VariableNode* rhsVarNode = builder.createVariable(curStmtNo, "y");
      AssignNode* assignNode = builder.buildAssignment(curStmtNo, varNode, rhsVarNode);

      varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      std::list<STMT_NO> stmts = assignTable.getAllStmtList();
      
      int listSize = stmts.size();
      Assert::AreEqual(listSize, 1);
      Assert::AreEqual(stmts.front(), (STMT_NO)1);
    
      varIndex = 1;
      curStmtNo = 2;
      assignNode = builder.buildAssignment(curStmtNo, builder.createVariable(curStmtNo,"x"), builder.createVariable(curStmtNo,"z"));
      varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      stmts = assignTable.getAllStmtList();
      listSize = stmts.size();
      Assert::AreEqual(listSize, 2);
      Assert::AreEqual(stmts.front() + 1, (STMT_NO)2);

      varIndex = 2;
      curStmtNo = 3;
      assignNode = builder.buildAssignment(curStmtNo, builder.createVariable(curStmtNo, "z"), builder.createVariable(curStmtNo, "c"));
      varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      stmts = assignTable.getAllStmtList();
      listSize = stmts.size();
      Assert::AreEqual(listSize, 3);
      Assert::AreEqual(stmts.front() + 2, (STMT_NO)curStmtNo);
    }

    TEST_METHOD(getAllStmtListByVar)
    {
      AssignTable assignTable;
      ASTBuilder builder;
      VariableNode* varNode = builder.createVariable(1, "x");
      VariableNode* rhsVarNode = builder.createVariable(1, "y");
      AssignNode* assignNode = builder.buildAssignment(1, varNode, rhsVarNode);
      VAR_INDEX_NO varIndex = 1;
      STMT_NO curLineNo = 1;
      varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      std::list<STMT_NO> stmts = assignTable.getAllStmtListByVar(varIndex);
      int listSize = stmts.size();
      Assert::AreEqual(listSize, 1);
      Assert::AreEqual(stmts.front(), curLineNo);

      varIndex = 1;
      curLineNo = 2;
      assignNode = builder.buildAssignment(curLineNo, builder.createVariable(curLineNo, "x"), builder.createVariable(curLineNo, "x"));
      assignTable.insertAssignRelation(1, assignNode);
      stmts = assignTable.getAllStmtListByVar(varIndex);
      listSize = stmts.size();
      Assert::AreEqual(listSize, 2);
      Assert::AreEqual(stmts.front() + 1, curLineNo);

      varIndex = 2;
      curLineNo = 3;
      assignNode = builder.buildAssignment(curLineNo, builder.createVariable(curLineNo, "y"), builder.createVariable(curLineNo, "c"));
      assignTable.insertAssignRelation(varIndex, assignNode);
      stmts = assignTable.getAllStmtListByVar(varIndex);
      listSize = stmts.size();
      Assert::AreEqual(listSize, 1);
      Assert::AreEqual(stmts.front(), curLineNo);
    }

    TEST_METHOD(getAllAssignStmtWithVar)
    {
      AssignTable assignTable;
      ASTBuilder builder;
      VAR_INDEX_NO varIndex = 1;
      STMT_NO curLineNo = 1;
      VariableNode* varNode = builder.createVariable(curLineNo, "x");
      VariableNode* rhsVarNode = builder.createVariable(curLineNo, "y");
      AssignNode* assignNode = builder.buildAssignment(curLineNo, varNode, rhsVarNode);

      varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      std::unordered_map<std::string, std::list<STMT_NO>> stmtMap = assignTable.getAllAssignStmtWithVar();

      int listSize = stmtMap.size();
      Assert::AreEqual(listSize, 1);
      std::unordered_map<std::string, std::list<STMT_NO>>::iterator mapItr = stmtMap.find(std::string("x"));
      Assert::AreEqual((*mapItr).first, std::string("x"));
      Assert::AreEqual((int)(*mapItr).second.size(), 1);

      varIndex = 1;
      curLineNo = 2;
      assignNode = builder.buildAssignment(curLineNo, builder.createVariable(curLineNo, "x"), builder.createVariable(curLineNo, "c"));
      varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      stmtMap = assignTable.getAllAssignStmtWithVar();

      listSize = stmtMap.size();
      Assert::AreEqual(listSize, 1);
      mapItr = stmtMap.find(std::string("x"));
      Assert::AreEqual((*mapItr).first, std::string("x"));
      Assert::AreEqual((int)(*mapItr).second.size(), 2);

      varIndex = 2;
      curLineNo = 3;
      assignNode = builder.buildAssignment(curLineNo, builder.createVariable(curLineNo, "y"), builder.createVariable(curLineNo, "c"));
      varIndex = assignTable.insertAssignRelation(varIndex, assignNode);
      stmtMap = assignTable.getAllAssignStmtWithVar();

      listSize = stmtMap.size();
      Assert::AreEqual(listSize, 2);
      mapItr = stmtMap.find(std::string("y"));
      Assert::AreEqual((*mapItr).first, std::string("y"));
      Assert::AreEqual((int)(*mapItr).second.size(), 1);
    }
  };
}