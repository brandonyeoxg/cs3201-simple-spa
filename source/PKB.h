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
#include "ConstantTable.h"
#include "GlobalTypeDef.h"



class TNode;

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

  /**
  * Method that inserts the line number (s2) to the unordered map of vectors containing line number s1 as key.
  * Returns false if current s1, s2 pair already exists in the map.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return The status of the insertion.
  */
  bool insertFollows(int t_s1, int t_s2);

  /**
  * Method that checks if follows(s1, s2) holds.
  * Checks if s2 exists as the first element in vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  bool isFollows(int t_s1, int t_s2);

  /**
  * Method that checks if follows*(s1, s2) holds.
  * Checks if s2 exists within the vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  bool isFollowsStar(int t_s1, int t_s2);

  /**
  * Method that returns the line number that follows(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param s1 an integer argument.
  * @return the line number that line s1 follows.
  */
  int getFollows(int t_s1);

  /**
  * Method that returns the line number that follows(s, s2) holds, where s is a variable and s2 is a known line number.
  * @param s2 an integer argument.
  * @return the line number that is followed by line s2.
  */
  int getFollowedBy(int t_s2);

  /**
  * Method that returns the list of line numbers that follows*(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param s1 an integer argument.
  * @return the vector of line numbers that line s1 follows*.
  */
  std::vector<int> getFollowsStar(int t_s1);

  /**
  * Method that returns the list of line numbers that follows*(s, s2) holds, where s is a variable and s2 is a known line number.
  * Returns the vector of keys that the vector mapped to the key contains s2.
  * @param s1 an integer argument.
  * @return the vector of line numbers that are followedBy* s2.
  */
  std::vector<int> getFollowedByStar(int t_s2);

  /**
  * Method that returns the entire map of line numbers that satisfy the follow* relationship.
  * @return the entire map that keep tracks of the follow relationship.
  */
  std::unordered_map<int, int> getAllFollows();

  /**
  * Method that returns the entire map of line numbers that satisfy the follow* relationship.
  * @return the entire map that keep tracks of the follow relationship.
  */
  std::unordered_map<int, std::vector<int>> getAllFollowsStar();

  /**
  * Method that returns the list of line numbers that is followed by another statement.
  * @return the vector of keys within the followTable.
  */
  std::vector<int> getFollowedByAnything();

  /**
  * Method that returns the list of line numbers that follows a statement.
  * @return the vector of keys within the followTable.
  */
  std::vector<int> getFollowsAnything();

  /**
  * Method that checks if follows(_, _) or follows*(_, _) holds.
  * @return true if the size of the followTable is more than zero, return false if otherwise.
  */
  bool hasFollowRelationship();

  /**
  * Method that checks if follows(_, s2) and follows*(_, s2) holds, where s2 is a statement number.
  * @return true if s2 exists in the allFollows map, return false if otherwise.
  */
  bool isFollowsAnything(int t_s2);

  /**
  * Method that checks if follows(2, _) and follows*(2, _) holds, where s2 is a statement number.
  * @return true if s2 exists in the allFollows map, return false if otherwise.
  */
  bool isFollowedByAnything(int t_s1);

  
  PROC_INDEX insertProcToAST(ProcedureNode* t_node);
  ProcedureNode* getRootAST(PROC_INDEX t_index);
  
  ///////////////////////////////////////////////////////
  //  ParentTable methods 
  ///////////////////////////////////////////////////////
  bool insertParent(int t_s1, int t_s2);
  void populateParentStarMap();
  void populateParentedByStarMap();
  bool isParent(int t_s1, int t_s2);
  bool isParentStar(int t_s1, int t_s2);
  int getParentOf(int t_s2);
  std::vector<int> getChildrenOf(int t_s1);
  std::vector<int> getParentStarOf(int t_s2);
  std::vector<int> getChildrenStarOf(int t_s1);
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
  bool insertTypeOfStatementTable(int t_lineNum, Grammar::GType t_type);
  std::unordered_map<Grammar::GType, std::vector<int>>  getStatementTypeTable();
  bool insertStatementTypeTable(Grammar::GType t_type, int t_lineNum);

  ///////////////////////////////////////////////////////
  //  VarTable methods 
  ///////////////////////////////////////////////////////
  int insertUsesForStmt(std::string t_varName, int t_lineNum);
  int insertModifiesForStmt(std::string t_varName, int t_lineNum);
  bool isModifies(int t_lineNum, std::string t_varName);
  bool isUses(int t_lineNum, std::string t_varName);
  std::vector<std::string> getModifies(int t_lineNum);
  std::vector<std::string> getUses(int t_lineNum);
  std::vector<int> getStmtModifies(std::string t_varName);
  std::vector<int> getStmtUses(std::string t_varName);
  std::unordered_map<std::string, std::vector<int>> getAllStmtModifies();
  std::unordered_map<std::string, std::vector<int>> getAllStmtUses();
  int getIndexOfVar(std::string t_varName);
  std::string getVarNameFromIndex(int t_index);
  bool isModifiesAnything(int t_lineNum);
  bool isUsesAnything(int t_lineNum);
  std::vector<int> getStmtModifiesAnything();
  std::vector<int> getStmtUsesAnything();
  std::vector<std::string> getAllVariables();

  ///////////////////////////////////////////////////////
  //  AssignTable
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

  ///////////////////////////////////////////////////////
  //  ConstantTable methods 
  ///////////////////////////////////////////////////////
  int insertConstant(std::string t_constant);
  std::list<std::string> getAllConstants();

  ///////////////////////////////////////////////////////
  //  Pattern Matching
  ///////////////////////////////////////////////////////
  std::list<STMT_NUM> getAssignStmtByVarPattern(std::string t_varName, std::string pattern, bool t_isExact); /*< Pattern a("x", "y") or Pattern a("x", _"y"_)*/
  std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtAndVarByPattern(std::string t_pattern, bool t_isExact); /* Pattern a(v,"y") or Pattern a(v, _"y"_)*/
  
  /** Pattern a(_, "x + y + h").
  *   Gets list of statements with exact pattern match on right hand side, and any variable on left hand side.
  *   @param t_pattern pattern to be matched (having whitespaces will not affect result) i.e. "x + y + h", "x"
  *   @return list of statement numbers with match
  *   @author jazlyn
  */
  std::list<STMT_NUM> getAllAssignStmtByExactPattern(std::string t_pattern);
  
  /** Pattern a(_, _"x + y + h"_).
  *   Gets list of statements with subtree pattern match on right hand side, and any variable on left hand side.
  *   @param t_pattern pattern to be matched (having whitespaces will not affect result) i.e. "x + y + h", "x+y"
  *   @return list of statement numbers with match
  *   @author jazlyn
  */
  std::list<STMT_NUM> getAllAssignStmtBySubtreePattern(std::string t_pattern);

private:
  FollowTable* m_followTable;
  ParentTable* m_parentTable;
  VarTable* m_varTable;
  ProcTable* m_procTable;
  AssignTable* m_assignTable;
  ConstantTable* m_constantTable;
  std::unordered_map<int, Grammar::GType> m_typeOfStatementTable;
  std::unordered_map<Grammar::GType, std::vector<int>> m_statementTypeTable;

  AST m_programNode;
  ASTBuilder m_builder;
};