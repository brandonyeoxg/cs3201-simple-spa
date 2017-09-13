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
#include "GlobalTypeDef.h"


class TNode;

//class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
  PKB();
  ///////////////////////////////////////////////////////
  //  Getter and Setter methods 
  ///////////////////////////////////////////////////////
  FollowTable* getFollowTable();
  ParentTable* getParentTable();
  VarTable* getVarTable();
  void setFollowTable(std::unordered_map<int, std::vector<int>> &table);

  ///////////////////////////////////////////////////////
  //  FollowTable methods 
  ///////////////////////////////////////////////////////
  bool insertFollows(int t_s1, int t_s2);
  bool isFollows(int t_s1, int t_s2);
  bool isFollowsStar(int t_s1, int t_s2);
  int getFollows(int t_s1);
  int getFollowedBy(int t_s2);
  std::vector<int> getFollowsStar(int t_s1);
  std::vector<int> getFollowedByStar(int t_s2);
  std::unordered_map<int, int> getAllFollows();
  std::unordered_map<int, std::vector<int>> getAllFollowsStar();
  std::vector<int> getFollowedByAnything();
  std::vector<int> getFollowsAnything();
  bool hasFollowRelationship();
  bool isFollowsAnything(int t_s2);
  bool isFollowedByAnything(int t_s1);

  
  PROC_INDEX insertProcToAST(ProcedureNode* t_node);
  ProcedureNode* getRootAST(PROC_INDEX t_index);
  
  ///////////////////////////////////////////////////////
  //  ParentTable methods 
  ///////////////////////////////////////////////////////
  bool insertParent(int s1, int s2);
  void populateParentStarMap();
  void populateParentedByStarMap();
  bool isParent(int s1, int s2);
  bool isParentStar(int s1, int s2);
  int getParentOf(int s2);
  std::vector<int> getChildrenOf(int s1);
  std::vector<int> getParentStarOf(int s2);
  std::vector<int> getChildrenStarOf(int s1);
  std::unordered_map<int, std::vector<int>> getAllParents();
  std::unordered_map<int, std::vector<int>> getAllParentsStar();
  std::vector<int> getChildrenOfAnything();
  std::vector<int> getParentOfAnything();
  std::vector<int> getChildrenStarOfAnything();
  std::vector<int> getParentStarOfAnything();
  bool hasParentRelationship();
  bool hasParentStarRelationship();
  bool isChildrenOfAnything(int t_s2);
  bool isParentOfAnything(int t_s1);
  bool isChildrenOfStarAnything(int t_s2);
  bool isParentOfStarAnything(int t_s1);


  
  //////////////////////////////////////////////////////////
  //  statementTypeTable and typeOfStatementTable Methods
  //////////////////////////////////////////////////////////
  
  std::unordered_map<int, Grammar::GType> getTypeOfStatementTable();
  bool insertTypeOfStatementTable(int line_num, Grammar::GType t_type);
  std::unordered_map<Grammar::GType, std::vector<int>>  getStatementTypeTable();
  bool insertStatementTypeTable(Grammar::GType t_type, int line_num);

  ///////////////////////////////////////////////////////
  //  VarTable methods 
  ///////////////////////////////////////////////////////
  int insertUsesForStmt(std::string t_varName, int t_lineNum);
  int insertModifiesForStmt(std::string t_varName, int t_lineNum);
  bool isModifies(int lineNum, std::string varName);
  bool isUses(int lineNum, std::string varName);
  std::vector<std::string> getModifies(int lineNum);
  std::vector<std::string> getUses(int line_num);
  std::vector<int> getStmtModifies(std::string varName);
  std::vector<int> getStmtUses(std::string varName);
  std::unordered_map<std::string, std::vector<int>> getAllStmtModifies();
  std::unordered_map<std::string, std::vector<int>> getAllStmtUses();
  int getIndexOfVar(std::string varName);
  std::string getVarNameFromIndex(int t_index);
  bool isModifiesAnything(int t_line_num);
  bool isUsesAnything(int t_line_num);
  std::vector<int> getStmtModifiesAnything();
  std::vector<int> getStmtUsesAnything();
  std::vector<std::string> getAllVariables();

  ///////////////////////////////////////////////////////
  VAR_INDEX insertAssignRelation(const VAR_INDEX& t_index, AssignNode* t_node);
  std::list<STMT_NUM> getAllAssignStmtListByVar(VAR_NAME& t_varName);
  std::list<STMT_NUM> getAllAssignStmtList();
  std::unordered_map<std::string, std::list<STMT_NUM>> getAllVarNameWithAssignStmt();
  std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVarName();
  void populateAssignTableAbstractions();

  //ProcTable Methods
  bool insertProcModifies(PROC_INDEX& t_procIdx, std::string& t_varIdx);
  bool insertProcUses(PROC_INDEX& t_procIdx, std::string& t_varIdx);
  void convertProcSetToList();

  bool isModifies(std::string& t_procName, std::string t_varName); /*< Modifies("First", "x")*/
  std::list<std::string>& getVarOfProcModifies(PROC_INDEX& t_procIdx); /*< Modifies("First", x) */
  std::list<std::string>& getProcNameThatModifiesVar(std::string& t_varName); /*< Modifies(p, "x") */
  std::unordered_map<std::string, std::list<std::string>>& getProcAndVarModifies(); /*< Modifies(p, v) */
  bool isModifiesInProc(std::string& t_procName); /*< Modifies("First", _) */
  std::list<std::string>& getProcThatModifies(); /*< Modifies(p, _) */

  bool isUses(std::string& t_procName, std::string& t_varName); /*< Uses("First", "x") */
  std::list<std::string>& getVarOfProcUses(PROC_INDEX& t_procIdx); /*< Uses("First", x) */
  std::list<std::string>& getProcNameThatUsesVar(std::string& t_varName); /*< Uses(p, "x") */
  std::unordered_map<std::string, std::list<std::string>>& getProcAndVarUses(); /*< Uses(p, v) */
  bool isUsesInProc(std::string& t_procName); /*< Uses("First", _) */
  std::list<std::string>& getProcThatUses(); /*< Uses(p, _) */

  // Pattern methods
  std::list<STMT_NUM> getAssignStmtByVarPattern(std::string t_varName, std::string pattern, bool t_isExact); /*< Pattern a("x", "y") or Pattern a("x", _"y"_)*/
  std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtAndVarByPattern(std::string t_pattern, bool t_isExact); /* Pattern a(v,"y") or Pattern a(v, _"y"_)*/
  // Jazlyn's stuff for patterns
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