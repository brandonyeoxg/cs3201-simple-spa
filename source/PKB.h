#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "AST.h"
#include "FollowTable.h"
#include "ParentTable.h"
#include "VarTable.h"
#include "ASTBuilder.h"
#include "ProcTable.h"
#include "VarTable.h"
#include "AssignTable.h"
#include "Grammar.h"

const int VARIABLE_S1 = -1;
const int VARIABLE_S2 = -2;


class TNode;

//class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
  PKB();
  //getter methods
  FollowTable* getFollowTable();
  ParentTable* getParentTable();
  VarTable* getVarTable();
  //setter methods
  void setFollowTable(std::unordered_map<int, std::vector<int>> &table);

  //to be removed
  std::unordered_map<int, std::vector<int>> returnFollowTable(int s1, int s2);
  std::unordered_map<int, std::vector<int>> returnParentTable(int s1, int s2);
  std::unordered_map<std::string, std::vector<int>> returnVarTable(std::string var);


  //FollowTable Methods
  bool insertFollows(int s1, int s2);
  bool isFollows(int s1, int s2);
  bool isFollowsStar(int s1, int s2);
  int getFollows(int s1);
  int getFollowedBy(int s2);
  std::vector<int> getFollowsStar(int s1);
  std::vector<int> getFollowedByStar(int s2);
  
  PROC_INDEX_NO insertProcToAST(ProcedureNode* t_node);
  ProcedureNode* getRootAST(PROC_INDEX_NO t_index);
  
  //ParentTable Methods
  bool insertParent(int s1, int s2);
  bool isParent(int s1, int s2);
  bool isParentStar(int s1, int s2);
  int getParentOf(int s2);
  std::vector<int> getChildrenOf(int s1);
  std::vector<int> getParentStarOf(int s2);
  std::vector<int> getChildrenStarOf(int s1);

  //statementTypeTable and typeOfStatementTable Methods
  std::unordered_map<int, Grammar::GType> getTypeOfStatementTable();
  bool insertTypeOfStatementTable(int line_num, Grammar::GType t_type);
  std::unordered_map<Grammar::GType, std::vector<int>>  getStatementTypeTable();
  bool insertStatementTypeTable(Grammar::GType t_type, int line_num);

  //VarTable Methods
  int insertUsesForStmt(std::string t_varName, int t_lineNum);
  int insertUsesForStmt(int index, std::string varName, int lineNum);
  int insertModifiesForStmt(std::string t_varName, int t_lineNum);
  int insertModifiesForStmt(int index, std::string varName, int lineNum);
  bool isModifies(int lineNum, std::string varName);
  bool isUses(int lineNum, std::string varName);
  std::vector<std::string> getModifies(int lineNum);
  std::vector<std::string> getUses(int line_num);
  std::vector<int> getStmtModifies(std::string varName);
  std::vector<int> getStmtUses(std::string varName);
  std::unordered_map<std::string, std::vector<int>> getAllStmtModifies();
  std::unordered_map<std::string, std::vector<int>> getAllStmtUses();
  int getIndexOfVar(std::string varName);

  //AssignTable Methods
  VAR_INDEX_NO insertAssignRelation(const VAR_INDEX_NO& t_index, AssignNode* t_node);
  std::list<STMT_NO> getAllStmtListByVar(VAR_INDEX_NO& t_index);
  std::list<STMT_NO> getAllStmtList();
  std::unordered_map<std::string, std::list<STMT_NO>> getAllAssignStmtWithVar();

  //ProcTable Methods
  bool insertProcModifies(PROC_INDEX_NO& t_procIdx, std::string& t_varIdx);
  bool insertProcUses(PROC_INDEX_NO& t_procIdx, std::string& t_varIdx);
  void convertProcSetToList();

  bool isModifies(PROC_INDEX_NO& t_procIdx, std::string t_varName); /*< Modifies("First", "x")*/
  std::list<std::string>& getVarOfProcModifies(PROC_INDEX_NO& t_procIdx); /*< Modifies("First", x) */
  std::list<std::string>& getProcNameThatModifiesVar(std::string& t_varName); /*< Modifies(p, "x") */
  std::unordered_map<std::string, std::list<std::string>>& getProcAndVarModifies(); /*< Modifies(p, v) */
  bool isModifiesInProc(std::string& t_procName); /*< Modifies("First", _) */
  std::list<std::string>& getProcThatModifies(); /*< Modifies(p, _) */

  bool isUses(PROC_INDEX_NO& t_procIdx, std::string& t_varName);
  std::list<std::string>& getVarOfProcUses(PROC_INDEX_NO& t_procIdx); /*< Uses("First", x) */
  std::list<std::string>& getProcNameThatUsesVar(std::string& t_varName); /*< Uses(p, "x") */
  std::unordered_map<std::string, std::list<std::string>>& getProcAndVarUses(); /*< Uses(p, v) */
  bool isUsesInProc(std::string& t_procName); /*< Uses("First", _) */
  std::list<std::string>& getProcThatUses(); /*< Uses(p, _) */

private:
  FollowTable* m_followTable;
  ParentTable* m_parentTable;
  VarTable* m_varTable;
  ProcTable* m_procTable;
  AssignTable* m_assignTable;
  std::unordered_map<int, Grammar::GType> m_typeOfStatementTable;
  std::unordered_map<Grammar::GType, std::vector<int>> m_statementTypeTable;

  AST m_programNode;
  ASTBuilder m_builder;
};