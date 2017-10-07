#pragma once
#include "GlobalTypeDef.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Grammar.h"

/**
* Represents an interface for ReadOnly methods in the PKB.
* The methods listed in this API is used by the Query Evaluator.
*
* @date 24/9/2017
*
*/
class PkbReadOnly {
public:

  ///////////////////////////////////////////////////////
  //  FollowTable
  ///////////////////////////////////////////////////////
  /**
  * Method that checks if follows(s1, s2) holds.
  * Checks if s2 exists as the first element in vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isFollows(STMT_NUM t_s1, STMT_NUM t_s2) = 0;

  /**
  * Method that checks if follows*(s1, s2) holds.
  * Checks if s2 exists within the vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isFollowsStar(STMT_NUM t_s1, STMT_NUM t_s2) = 0;

  /**
  * Method that returns the line number that follows(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param s1 an integer argument.
  * @return the line number that line s1 follows.
  */
  virtual STMT_NUM getFollows(STMT_NUM t_s1) = 0;

  /**
  * Method that returns the line number that follows(s, s2) holds, where s is a variable and s2 is a known line number.
  * @param s2 an integer argument.
  * @return the line number that is followed by line s2.
  */
  virtual STMT_NUM getFollowedBy(STMT_NUM t_s2) = 0;

  /**
  * Method that returns the list of line numbers that follows*(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param s1 an integer argument.
  * @return the vector of line numbers that line s1 follows*.
  */
  virtual LIST_OF_STMT_NUMS getFollowsStar(STMT_NUM t_s1) = 0;

  /**
  * Method that returns the list of line numbers that follows*(s, s2) holds, where s is a variable and s2 is a known line number.
  * Returns the vector of keys that the vector mapped to the key contains s2.
  * @param s1 an integer argument.
  * @return the vector of line numbers that are followedBy* s2.
  */
  virtual LIST_OF_STMT_NUMS getFollowedByStar(STMT_NUM t_s2) = 0;

  /**
  * Method that returns the entire map of line numbers that satisfy the follow* relationship.
  * @return the entire map that keep tracks of the follow relationship.
  */
  virtual std::unordered_map<STMT_NUM, STMT_NUM> getAllFollows() = 0;

  /**
  * Method that returns the entire map of line numbers that satisfy the follow* relationship.
  * @return the entire map that keep tracks of the follow relationship.
  */
  virtual std::unordered_map<STMT_NUM, std::vector<int>> getAllFollowsStar() = 0;

  /**
  * Method that returns the list of line numbers that is followed by another statement.
  * @return the vector of keys within the followTable.
  */
  virtual LIST_OF_STMT_NUMS getFollowedByAnything() = 0;

  /**
  * Method that returns the list of line numbers that follows a statement.
  * @return the vector of keys within the followTable.
  */
  virtual LIST_OF_STMT_NUMS getFollowsAnything() = 0;

  /**
  * Method that checks if follows(_, _) or follows*(_, _) holds.
  * @return true if the size of the followTable is more than zero, return false if otherwise.
  */
  virtual bool hasFollowRelationship() = 0;

  /**
  * Method that checks if follows(_, t_s2) and follows*(_, t_s2) holds, where t_s2 is a statement number.
  * @return true if t_s2 exists in the allFollows map, return false if otherwise.
  */
  virtual bool isFollowsAnything(STMT_NUM t_s2) = 0;

  /**
  * Method that checks if follows(2, _) and follows*(2, _) holds, where s2 is a statement number.
  * @return true if s2 exists in the allFollows map, return false if otherwise.
  */
  virtual bool isFollowedByAnything(STMT_NUM t_s1) = 0;

  ///////////////////////////////////////////////////////
  //  ParentTable
  ///////////////////////////////////////////////////////
  /**
  * Method that checks if parent(t_s1, t_s2) holds.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isParent(STMT_NUM t_s1, STMT_NUM t_s2) = 0;

  /**
  * Method that checks if parent*(t_s1, t_s2) holds.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isParentStar(STMT_NUM t_s1, STMT_NUM t_s2) = 0;

  /**
  * Method that returns the line numuber that is the parent of t_s2.
  * For example: stmt s; parent(s, 4).
  * @param t_s2 an integer argument.
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual STMT_NUM getParentOf(STMT_NUM t_s2) = 0;

  /**
  * Method that returns the vector of line numubers that are the children of t_s1.
  * For example: stmt s; parent(4, s).
  * @param t_s1 an integer argument.
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getChildrenOf(STMT_NUM t_s1) = 0;

  /**
  * Method that returns the vector of line numubers that are the parent* of t_s2.
  * For example: stmt s; parent*(s, 4).
  * @param t_s2 an integer argument.
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getParentStarOf(STMT_NUM t_s2) = 0;

  /**
  * Method that returns the vector of line numubers that are the children* of t_s1.
  * For example: stmt s; parent*(4, s).
  * @param t_s1 an integer argument.
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getChildrenStarOf(STMT_NUM t_s1) = 0;

  /**
  * Method that returns the entire of parent relationship.
  * @return an unordered_map i.e. parentMap.
  */
  virtual std::unordered_map<STMT_NUM, LIST_OF_STMT_NUMS> getAllParents() = 0;

  /**
  * Method that returns the entire of parent* relationship.
  * @return an unordered_map i.e. parentStarMap.
  */
  virtual std::unordered_map<STMT_NUM, LIST_OF_STMT_NUMS> getAllParentsStar() = 0;

  /**
  * Method that returns the vector of line numubers that are the children of any line.
  * For example: stmt s2; parent(_, s2).
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getChildrenOfAnything() = 0;

  /**
  * Method that returns the vector of line numubers that are the parent of any line.
  * For example: stmt s1; parent(s1, _).
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getParentOfAnything() = 0;

  /**
  * Method that returns the vector of line numubers that are the parent of any line.
  * For example: stmt s2; parent*(_, s2).
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getChildrenStarOfAnything() = 0;

  /**
  * Method that returns the vector of line numubers that are the parent of any line.
  * For example: stmt s1; parent*(s1, _).
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getParentStarOfAnything() = 0;

  /**
  * Method that checks if parent(_, _) holds.
  * @return true if there exists a parent relationship (i.e. parentTable is not empty), false if otherwise.
  */
  virtual bool hasParentRelationship() = 0;

  /**
  * Method that checks if parent*(_, _) holds.
  * @return true if there exists a parent* relationship (i.e. parentStarTable is not empty), false if otherwise.
  */
  virtual bool hasParentStarRelationship() = 0;

  /**
  * Method that checks if parent(_, t_s2) holds.
  * @param t_s2 an integer argument.
  * @return true if there exists a parent relationship with t_s2 being the child, false if otherwise.
  */
  virtual bool isChildrenOfAnything(STMT_NUM t_s2) = 0;

  /**
  * Method that checks if parent(t_s1, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one parent relationship with t_s1 being the parent, false if otherwise.
  */
  virtual bool isParentOfAnything(STMT_NUM t_s1) = 0;

  /**
  * Method that checks if parent*(_, t_s2) holds.
  * @param t_s2 an integer argument.
  * @return true if there exists at least one parent* relationship with t_s2 being the child, false if otherwise.
  */
  virtual bool isChildrenOfStarAnything(STMT_NUM t_s2) = 0;

  /**
  * Method that checks if parent*(t_s1, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one parent* relationship with t_s1 being the parent, false if otherwise.
  */
  virtual bool isParentOfStarAnything(STMT_NUM t_s1) = 0;

  //////////////////////////////////////////////////////////
  //  StatementTypeTable and TypeOfStatementTable
  //////////////////////////////////////////////////////////

  /**
  * Method that retrieves the TypeOfStatementTable.
  * @return the unordered_map of the statement's line number mapped to the type of the statement.
  */
  virtual std::unordered_map<STMT_NUM, queryType::GType> getTypeOfStatementTable() = 0;

  /**
  * Method that retrieves the StatementTypeTable.
  * @return the unordered_map of type of the statement mapped to its line number.
  */
  virtual std::unordered_map<queryType::GType, LIST_OF_STMT_NUMS>  getStatementTypeTable() = 0;

  virtual LIST_OF_STMT_NUMS getListOfStatements(queryType::GType t_type) = 0;

  ///////////////////////////////////////////////////////
  //  VarTable 
  ///////////////////////////////////////////////////////

  /**
  * Method that returns the index of the variable t_varName in VarTable.
  * @param t_varName a string argument.
  * @return the index of variable.
  */
  virtual STMT_NUM getVarIdxFromName(std::string t_varName) = 0;

  /**
  * Method that returns the name of the variable in VarTable given its index.
  * @param t_index an integer argument.
  * @return the name of the variable.
  */
  virtual std::string getVarNameFromIdx(STMT_NUM t_index) = 0;

  /**
  * Method that returns the vector of variables that are stored within VarTable.
  * @return a vector of statement numbers.
  */
  virtual LIST_OF_VAR_NAMES& getAllVarNames() = 0;

  ///////////////////////////////////////////////////////
  //  AssignTable
  ///////////////////////////////////////////////////////

  /*
  * Returns all assignment statements number that modifies the variable name.
  * @param t_varName the name of the variable.
  */
  virtual LIST_OF_STMT_NUMS getAllAssignStmtListByVar(VAR_NAME& t_varName) = 0;

  /*
  * Returns all assignment statements.
  */
  virtual LIST_OF_STMT_NUMS getAllAssignStmtList() = 0;

  /*
  * Returns all assignment statements in a representation.
  * The representation is a variable mapped to all statement number under that variable.
  */
  virtual std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> getAllVarNameWithAssignStmt() = 0;

  /*
  * Returns all assignment statements in a representation.
  * The repsentation is a statement number mapped to the variable in that statement number.
  */
  virtual std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVarName() = 0;
  ///////////////////////////////////////////////////////
  //  ConstantTable
  ///////////////////////////////////////////////////////

  /**
  * Method that returns the list of all constants that are stored within ConstantTable.
  * @return a list of constants (strings).
  */
  virtual std::list<std::string> getAllConstants() = 0;

  ///////////////////////////////////////////////////////
  //  Pattern Matching
  ///////////////////////////////////////////////////////
  /** Pattern a(_, "x + y + h").
  *   Gets list of statements with exact pattern match on right hand side, and any variable on left hand side.
  *   @param t_pattern pattern to be matched (having whitespaces will not affect result) i.e. "x + y + h", "x"
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  virtual std::list<STMT_NUM> getAllAssignStmtByExactPattern(std::string t_pattern) = 0;
  
  /** Pattern a(_, _"x + y + h"_).
  *   Gets list of statements with subtree pattern match on right hand side, and any variable on left hand side.
  *   @param t_pattern pattern to be matched (having whitespaces will not affect result) i.e. "x + y + h", "x+y"
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  virtual std::list<STMT_NUM> getAllAssignStmtBySubtreePattern(std::string t_pattern) = 0;

  virtual LIST_OF_STMT_NUMS getAllAssignStmtByVar(std::string t_varName) = 0;

  /** Pattern a("x", "y + x").
  *   Gets list of statements with given variable name on left hand side, and exact pattern match on right hand side.
  *   @param t_varName name of the variable modified in assignment statement
  *   @param t_pattern pattern to be matched (having whitespaces will not affect result) i.e. "x + y + h", "x+y"
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  virtual std::list<STMT_NUM> getAllAssignStmtByVarAndExactPattern(std::string t_varName, std::string t_pattern) = 0;

  /** Pattern a("x", _"y + x"_).
  *   Gets list of statements with given variable name on left hand side, and subtree pattern match on right hand side.
  *   @param t_varName name of the variable modified in assignment statement
  *   @param t_pattern pattern to be matched (having whitespaces will not affect result) i.e. "x + y + h", "x+y"
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  virtual std::list<STMT_NUM> getAllAssignStmtByVarAndSubtreePattern(std::string t_varName, std::string t_pattern) = 0;

  virtual std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVarByExactPattern(std::string t_pattern) = 0;

  virtual std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVarBySubtreePattern(std::string t_pattern) = 0;

  ///////////////////////////////////////////////////////
  //  CallsTable methods
  ///////////////////////////////////////////////////////
  virtual bool isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2) = 0;
  virtual bool isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2) = 0;
  virtual LIST_OF_PROC_NAMES getCalls(PROC_NAME t_proc2) = 0;
  virtual LIST_OF_PROC_NAMES getCalledBy(PROC_NAME t_proc1) = 0;
  virtual LIST_OF_PROC_NAMES getCallsStar(PROC_NAME t_proc2) = 0;
  virtual LIST_OF_PROC_NAMES getCalledByStar(PROC_NAME t_proc1) = 0;
  virtual std::unordered_map<PROC_NAME, PROC_NAME> getAllCalls() = 0;
  virtual std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> getAllCallsStar() = 0; //calls*(proc1, proc2) 
  virtual LIST_OF_PROC_NAMES getCallsAnything() = 0;  //calls(proc1, _)
  virtual LIST_OF_PROC_NAMES getCallsStarAnything() = 0;  //calls*(proc1, _)
  virtual LIST_OF_PROC_NAMES getCalledByAnything() = 0; //calls(_, proc2)
  virtual LIST_OF_PROC_NAMES getCalledByStarAnything() = 0; //calls*(_, proc2)
  virtual bool hasCallsRelationship() = 0;  //calls(_, _)
  virtual bool isCallsAnything(PROC_NAME t_proc1) = 0;
  virtual bool isCalledByAnything(PROC_NAME t_proc2) = 0;

  ///////////////////////////////////////////////////////
  //  ProcTable
  ///////////////////////////////////////////////////////
  /**
  * Returns all procedure name in the program
  */
  virtual std::vector<PROC_NAME>& getAllProcsName() = 0;

  ///////////////////////////////////////////////////////
  //  ModifiesP methods
  ///////////////////////////////////////////////////////
  virtual bool isModifiesP(const PROC_NAME& t_procName, const VAR_NAME& t_varName) = 0; /*< Modifies("First", "x") */
  virtual bool isModifiesInProc(const PROC_NAME& t_procName) = 0; /*< Modifies("First", _) */
  virtual LIST_OF_VAR_NAMES getModifiesPVarNamesWithProcIdx(const PROC_NAME& t_procName) = 0; /*< Modifies("First", x) */
  virtual LIST_OF_PROC_NAMES getModifiesPProcNamesWithVarIdx(const VAR_NAME& t_varName) = 0; /*< Modifies(p, "x") */
  virtual MAP_OF_PROC_TO_VAR& getModifiesPAllProcToVar() = 0; /*< Modifies(p, x) */
  virtual LIST_OF_PROC_NAMES& getModifiesPAllProcNames() = 0; /*< Modifies(p, _) */

  ///////////////////////////////////////////////////////
  //  UsesP methods
  ///////////////////////////////////////////////////////
  virtual bool isUsesP(const PROC_NAME& t_procName, const VAR_NAME& t_varName) = 0; /*< Uses("First", "x") */
  virtual bool isUsesInProc(const PROC_NAME& t_procName) = 0; /*< Modifies("First", _) */
  virtual LIST_OF_VAR_NAMES getUsesPVarNamesWithProcIdx(const PROC_NAME& t_procName) = 0; /*< Uses("First", x) */
  virtual LIST_OF_PROC_NAMES getUsesPProcNamesWithVarIdx(const VAR_NAME& t_varName) = 0; /*< Uses(p, "x") */
  virtual MAP_OF_PROC_TO_VAR& getUsesPAllProcToVar() = 0; /*< Uses(p, x) */
  virtual LIST_OF_PROC_NAMES& getUsesPAllProcNames() = 0; /*< Uses(p, _) */

  ///////////////////////////////////////////////////////
  //  Uses methods
  ///////////////////////////////////////////////////////
  /**
  * Method that checks if uses(t_s1, t_varname) holds.
  * @param t_lineNum an integer argument.
  * @param s2 t_varName a string argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isUses(STMT_NUM t_lineNum, std::string t_varName) = 0;
  
  /**
  * Method that returns the vector of variables that are used in line number t_lineNum.
  * For example: uses(2, "x").
  * @param t_lineNum an integer argument.
  * @return a vector of variables that satisfy the condition.
  */
  virtual LIST_OF_VAR_NAMES getUses(STMT_NUM t_lineNum) = 0;

  /**
  * Method that returns the vector of statement numbers that uses variable t_varName.
  * For example: stmt s; uses(s, "x").
  * @param t_lineNum an integer argument.
  * @return a vector of statement numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getStmtUses(std::string t_varName) = 0;

  /**
  * Method that returns the entire map of uses relationship.
  * For example: stmt s, variable v; uses(s, v).
  * @return an unordered_map that satisfy the condition.
  */
  virtual std::unordered_map<std::string, LIST_OF_STMT_NUMS> getAllStmtUses() = 0;

  /**
  * Method that checks if uses(t_lineNum, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one uses relationship with t_lineNum being the statement number, false if otherwise.
  */
  virtual bool isUsesAnything(STMT_NUM t_lineNum) = 0;

  /**
  * Method that returns the vector of line numbers that involves use of variables.
  * For example: stmt s; uses(s, _).
  * @return a vector of statement numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getStmtUsesAnything() = 0;

  ///////////////////////////////////////////////////////
  //  Modifies methods
  ///////////////////////////////////////////////////////
  /**
  * Method that checks if modifies(t_s1, t_varname) holds.
  * @param t_lineNum an integer argument.
  * @param s2 t_varName a string argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isModifies(STMT_NUM t_lineNum, std::string t_varName) = 0;

  /**
  * Method that returns the vector of variables that are modified in line number t_lineNum.
  * For example: modifies(2, "x").
  * @param t_lineNum an integer argument.
  * @return a vector of variables that satisfy the condition.
  */
  virtual LIST_OF_VAR_NAMES getModifies(STMT_NUM t_lineNum) = 0;

  /**
  * Method that returns the vector of statement numbers that modifies variable t_varName.
  * For example: stmt s; modifies(s, "x").
  * @param t_lineNum an integer argument.
  * @return a vector of statement numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getStmtModifies(std::string t_varName) = 0;

  /**
  * Method that returns the entire map of modifies relationship.
  * For example: stmt s, variable v; modifies(s, v).
  * @return an unordered_map that satisfy the condition.
  */
  virtual std::unordered_map<std::string, LIST_OF_STMT_NUMS> getAllStmtModifies() = 0;

  /**
  * Method that checks if modifies(t_lineNum, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one modifies relationship with t_lineNum being the statement number, false if otherwise.
  */
  virtual bool isModifiesAnything(STMT_NUM t_lineNum) = 0;

  /**
  * Method that returns the vector of line numbers that involves modification of variables.
  * For example: stmt s; modifies(s, _).
  * @return a vector of statement numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getStmtModifiesAnything() = 0;


  //  StmtListTable
  ///////////////////////////////////////////////////////
  virtual LIST_OF_STMT_NUMS& getStmtList() = 0;
};