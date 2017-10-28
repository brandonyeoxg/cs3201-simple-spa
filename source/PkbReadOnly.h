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
  virtual BOOLEAN isFollows(STMT_NUM t_s1, STMT_NUM t_s2) = 0;

  /**
  * Method that checks if follows*(s1, s2) holds.
  * Checks if s2 exists within the vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual BOOLEAN isFollowsStar(STMT_NUM t_s1, STMT_NUM t_s2) = 0;

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
  virtual MAP_OF_STMT_NUMS getAllFollows() = 0;

  /**
  * Method that returns the entire map of line numbers that satisfy the follow* relationship.
  * @return the entire map that keep tracks of the follow relationship.
  */
  virtual MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllFollowsStar() = 0;

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
  virtual BOOLEAN hasFollowRelationship() = 0;

  /**
  * Method that checks if follows(_, t_s2) and follows*(_, t_s2) holds, where t_s2 is a statement number.
  * @return true if t_s2 exists in the allFollows map, return false if otherwise.
  */
  virtual BOOLEAN isFollowsAnything(STMT_NUM t_s2) = 0;

  /**
  * Method that checks if follows(2, _) and follows*(2, _) holds, where s2 is a statement number.
  * @return true if s2 exists in the allFollows map, return false if otherwise.
  */
  virtual BOOLEAN isFollowedByAnything(STMT_NUM t_s1) = 0;

  ///////////////////////////////////////////////////////
  //  ParentTable
  ///////////////////////////////////////////////////////
  /**
  * Method that checks if parent(s1, s2) holds.
  * Checks if s2 exists as value mapped to key s1 in parentMap.
  * @param t_s1 an integer argument.
  * @param t_s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual BOOLEAN isParent(STMT_NUM t_s1, STMT_NUM t_s2) = 0;

  /**
  * Method that checks if parent*(s1, s2) holds.
  * Checks if s2 exists in vector mapped to key s1 in parentStarMap.
  * @param t_s1 an integer argument.
  * @param t_s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual BOOLEAN isParentStar(STMT_NUM t_s1, STMT_NUM t_s2) = 0;

  /**
  * Method that returns the statement number that is the parent of s2.
  * Checks if key s2 exists in parentMap.
  * @throw invalid_arguement exception if key does not exist.
  * @param t_s2 an integer argument.
  * @return a statement number.
  */
  virtual STMT_NUM getParentOf(STMT_NUM t_s2) = 0;

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent(t_s1, x).
  * Returns empty vector if no relationship holds for all statement numbers.
  * @param t_s1 an integer argument.
  * @return a vector of statement numbers.
  */
  virtual LIST_OF_STMT_NUMS getChildrenOf(STMT_NUM t_s1) = 0;

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent*(x, t_s2).
  * Returns empty vector if no relationship holds for all statement numbers.
  * @param t_s2 an integer argument.
  * @return a vector of statement numbers.
  */
  virtual LIST_OF_STMT_NUMS getParentStarOf(STMT_NUM t_s2) = 0;

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent*(t_s1, x).
  * Returns empty vector if no relationship holds for all statement numbers.
  * @param t_s1 an integer argument.
  * @return a vector of statement numbers.
  */
  virtual LIST_OF_STMT_NUMS getChildrenStarOf(STMT_NUM t_s1) = 0;

  /**
  * Method that returns the entire map of line numbers that satisfy the parent relationship.
  * @return the unordered map that keep tracks of the parent relationship.
  */
  virtual MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllParents() = 0;

  /**
  * Method that returns the entire map of line numbers that satisfy the parent* relationship.
  * @return the unordered map that keep tracks of the parent* relationship.
  */
  virtual MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllParentsStar() = 0;

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent(_, x).
  * @return the vector of keys within the parentTable.
  */
  virtual LIST_OF_STMT_NUMS getChildrenOfAnything() = 0;

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent(x, _).
  * @return the vector of keys within the parentTable.
  */
  virtual LIST_OF_STMT_NUMS getParentOfAnything() = 0;

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent*(_, x).
  * @return the vector of keys within the parentTable.
  */
  virtual LIST_OF_STMT_NUMS getChildrenStarOfAnything() = 0;

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent*(x, _).
  * @return the vector of keys within the parentTable.
  */
  virtual LIST_OF_STMT_NUMS getParentStarOfAnything() = 0;

  /**
  * Method that checks if parent(_, _) holds.
  * @return true if there exists a parent relationship (i.e. parentTable is not empty), false if otherwise.
  */
  virtual BOOLEAN hasParentRelationship() = 0;

  /**
  * Method that checks if parent*(_, _) holds.
  * @return true if there exists a parent* relationship (i.e. parentStarTable is not empty), false if otherwise.
  */
  virtual BOOLEAN hasParentStarRelationship() = 0;

  /**
  * Method that checks if parent(_, t_s2) holds.
  * @param t_s2 an integer argument.
  * @return true if there exists a parent relationship with t_s2 being the child, false if otherwise.
  */
  virtual BOOLEAN isChildrenOfAnything(STMT_NUM t_s2) = 0;

  /**
  * Method that checks if parent(t_s1, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one parent relationship with t_s1 being the parent, false if otherwise.
  */
  virtual BOOLEAN isParentOfAnything(STMT_NUM t_s1) = 0;

  /**
  * Method that checks if parent*(_, t_s2) holds.
  * @param t_s2 an integer argument.
  * @return true if there exists at least one parent* relationship with t_s2 being the child, false if otherwise.
  */
  virtual BOOLEAN isChildrenOfStarAnything(STMT_NUM t_s2) = 0;

  /**
  * Method that checks if parent*(t_s1, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one parent* relationship with t_s1 being the parent, false if otherwise.
  */
  virtual BOOLEAN isParentOfStarAnything(STMT_NUM t_s1) = 0;

  //////////////////////////////////////////////////////////
  //  StatementTypeTable and TypeOfStatementTable
  //////////////////////////////////////////////////////////

  /**
  * Method that retrieves the TypeOfStatementTable.
  * @return the unordered_map of the statement's line number mapped to the type of the statement.
  */
  virtual MAP_OF_STMT_NUM_TO_GTYPE getTypeOfStatementTable() = 0;

  /**
  * Method that retrieves the StatementTypeTable.
  * @return the unordered_map of type of the statement mapped to its line number.
  */
  virtual MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS  getStatementTypeTable() = 0;

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
  virtual VAR_NAME getVarNameFromIdx(STMT_NUM t_index) = 0;

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
  virtual LIST_OF_STMT_NUMS getAllAssignStmts() = 0;

  /*
  * Returns all assignment statements in a representation.
  * The representation is a variable mapped to all statement number under that variable.
  */
  virtual MAP_OF_VAR_NAME_TO_STMT_NUMS getAllVarNameWithAssignStmt() = 0;


  /*
  * Returns all assignment statements in a representation.
  * The repsentation is a statement number mapped to the variable in that statement number.
  */
  virtual MAP_OF_STMT_NUM_TO_VAR_NAME getAllAssignStmtWithVarName() = 0;
  ///////////////////////////////////////////////////////
  //  ConstantTable
  ///////////////////////////////////////////////////////

  /**
  * Method that returns the list of all constants that are stored within ConstantTable.
  * @return a list of constants (strings).
  */
  virtual LIST_OF_RESULTS getAllConstants() = 0;

  ///////////////////////////////////////////////////////
  //  Pattern Matching
  ///////////////////////////////////////////////////////

  /** Pattern a(_, "x + y + h").
  *   Gets list of statements with exact pattern match on right hand side, and any variable on left hand side.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  virtual LIST_OF_ASSIGN_STMTS_FOR_PATTERN getAllAssignStmtByExactPattern(std::vector<std::string> t_patternTokens) = 0;

  /** Pattern a(_, _"x + y + h"_).
  *   Gets list of statements with subtree pattern match on right hand side, and any variable on left hand side.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  virtual LIST_OF_ASSIGN_STMTS_FOR_PATTERN getAllAssignStmtBySubtreePattern(std::vector<std::string> t_patternTokens) = 0;

  /** Pattern a("x", _""_).
  *   Gets list of statements with any expression on right hand side, and given variable on left hand side.
  *   @param t_varName name of variable on left hand side
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  virtual LIST_OF_STMT_NUMS getAllAssignStmtByVar(std::string t_varName) = 0;

  /** Pattern a("x", "y + x").
  *   Gets list of statements with given variable name on left hand side, and exact pattern match on right hand side.
  *   @param t_varName name of the variable modified in assignment statement
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  virtual LIST_OF_ASSIGN_STMTS_FOR_PATTERN getAllAssignStmtByVarAndExactPattern(std::string t_varName, std::vector<std::string> t_patternTokens) = 0;

  /** Pattern a("x", _"y + x"_).
  *   Gets list of statements with given variable name on left hand side, and subtree pattern match on right hand side.
  *   @param t_varName name of the variable modified in assignment statement
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  virtual LIST_OF_ASSIGN_STMTS_FOR_PATTERN getAllAssignStmtByVarAndSubtreePattern(std::string t_varName, std::vector<std::string> t_patternTokens) = 0;

  /** variable v; Pattern a(v, "x + y + h").
  *   Gets map of statements with exact pattern match on right hand side, and any variable on left hand side.
  *   Map will be returned with statement number as key, and variable name as value.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return map of statement numbers to their respective variable names (will be empty if none)
  *   @author jazlyn
  */
  virtual MAP_OF_STMT_NUM_TO_VAR_NAME getAllAssignStmtWithVarByExactPattern(std::vector<std::string> t_patternTokens) = 0;

  /** variable v; Pattern a(v, _"x + y + h"_).
  *   Gets map of statements with subtree pattern match on right hand side, and any variable on left hand side.
  *   Map will be returned with statement number as key, and variable name as value.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return map of statement numbers to their respective variable names (will be empty if none)
  *   @author jazlyn
  */
  virtual MAP_OF_STMT_NUM_TO_VAR_NAME getAllAssignStmtWithVarBySubtreePattern(std::vector<std::string> t_patternTokens) = 0;

  /** For Pattern w("x", _), where w is a common synonym for all while statements.
  *   Gets list of while statements that uses a given variable.
  *   @param t_varName variable name
  *   @return list of statement numbers
  */
  virtual LIST_OF_STMT_NUMS getWhileStmtByVar(STRING t_varName) = 0;

  /** For Pattern w(v, _), where w is a common synonym for all while statements.
  *   Gets map of while statements with the variable name used in each while statement.
  *   Map will be returned with statement number as key, and variable name as value.
  *   @return map of statement numbers to their respective variable names (will be empty if none)
  */
  virtual std::unordered_map<STMT_NUM, VAR_NAME> getAllWhileStmtsWithVar() = 0;

  /** For Pattern w(_,  _), where w is a common synonym for all while statements.
  *   Gets list of all while statements.
  *   @return list of statement numbers
  */
  virtual LIST_OF_STMT_NUMS getAllWhileStmts() = 0;

  /** For Pattern i("x", _), where i is a common synonym for all if statements.
  *   Gets list of if statements that uses a given variable (in the if statement itself, not nested statements).
  *   @param t_varName variable name
  *   @return list of statement numbers
  */
  virtual LIST_OF_STMT_NUMS getIfStmtByVar(STRING t_varName) = 0;

  /** For Pattern i(v, _), where i is a common synonym for all if statements.
  *   Gets map of if statements with the variable name used in each if statement.
  *   Map will be returned with statement number as key, and variable name as value.
  *   @return map of statement numbers to their respective variable names (will be empty if none)
  */
  virtual std::unordered_map<STMT_NUM, VAR_NAME> getAllIfStmtsWithVar() = 0;

  /** For Pattern i(_,  _), where i is a common synonym for all if statements.
  *   Gets list of all if statements.
  *   @return list of statement numbers
  */
  virtual LIST_OF_STMT_NUMS getAllIfStmts() = 0;

  ///////////////////////////////////////////////////////
  //  CallsTable methods
  ///////////////////////////////////////////////////////

  /**
  * Method to check if the relationship calls(t_proc1, t_proc2) holds.
  * @param t_proc1 a procedure that calls another procedure.
  * @param t_proc2 a procedure to be called.
  * @return true if relationship holds, false if otherwise. 
  */
  virtual BOOLEAN isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2) = 0;

  /**
  * Method to check if the relationship calls*(t_proc1, t_proc2) holds.
  * @param t_proc1 a procedure that calls* another procedure.
  * @param t_proc2 a procedure to be called.
  * @return true if relationship holds, false if otherwise.
  */
  virtual BOOLEAN isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2) = 0;

  /**
  * Method to get a vector of procedure p that the relationship calls(p, t_proc2) holds.
  * @param t_proc2 a procedure to be called.
  * @return a vector of precedures that calls t_proc2.
  */
  virtual LIST_OF_PROC_NAMES getCalls(PROC_NAME t_proc2) = 0;

  /**
  * Method to get a vector of procedure p that the relationship calls(t_proc1, p) holds.
  * @param t_proc1 a procedure that calls p.
  * @return a vector of precedures that are called by t_proc1.
  */
  virtual LIST_OF_PROC_NAMES getCalledBy(PROC_NAME t_proc1) = 0;

  /**
  * Method to get a vector of procedure p that the relationship calls*(p, t_proc2) holds.
  * @param t_proc2 a procedure to be called*.
  * @return a vector of precedure that fulfills the relationship.
  */
  virtual LIST_OF_PROC_NAMES getCallsStar(PROC_NAME t_proc2) = 0;

  /**
  * Method to get a vector of procedure p that the relationship calls*(t_proc1, p) holds.
  * @param t_proc1 a procedure that calls* p.
  * @return a vector of precedures that fulfills the relationship.
  */
  virtual LIST_OF_PROC_NAMES getCalledByStar(PROC_NAME t_proc1) = 0;

  /**
  * Method that returns the entire map of line numbers that satisfy the calls relationship.
  * @return the entire map that keep tracks of the calls relationship.
  */
  virtual MAP_OF_PROC_NAMES getAllCalls() = 0;

  /**
  * Method that returns the entire map of line numbers that satisfy the calls* relationship.
  * @return the entire map that keep tracks of the calls* relationship.
  */
  virtual MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES getAllCallsStar() = 0; //calls*(proc1, proc2) 

  /**
  * Method that returns the vector of procedure names that calls another procedure.
  * @return the vector of keys within the callsMap.
  */
  virtual LIST_OF_PROC_NAMES getCallsAnything() = 0;  //calls(proc1, _)

  /**
  * Method that returns the vector of procedure names that calls* another procedure.
  * @return the vector of keys within the callsStarMap.
  */
  virtual LIST_OF_PROC_NAMES getCallsStarAnything() = 0;  //calls*(proc1, _)

  /**
  * Method that returns the vector of procedure names that called by another procedure.
  * @return the vector of keys within the calledByMap.
  */
  virtual LIST_OF_PROC_NAMES getCalledByAnything() = 0; //calls(_, proc2)

  /**
  * Method that returns the vector of procedure names that called by* another procedure.
  * @return the vector of keys within the calledByStarMap.
  */
  virtual LIST_OF_PROC_NAMES getCalledByStarAnything() = 0; //calls*(_, proc2)

  /**
  * Method that checks if a calls relationship exists in the SIMPLE program.
  * @return true if relationship exists, false if otherwise.
  */
  virtual BOOLEAN hasCallsRelationship() = 0;  //calls(_, _)

  /**
  * Method that checks if procedure t_proc1 calls another procedure.
  * @return true if it does, false if otherwise.
  */
  virtual BOOLEAN isCallsAnything(PROC_NAME t_proc1) = 0;

  /**
  * Method that checks if procedure t_proc2 was called by another procedure.
  * @return true if it does, false if otherwise.
  */
  virtual BOOLEAN isCalledByAnything(PROC_NAME t_proc2) = 0;

  virtual PROC_NAME getProcNameFromCallStmtNum(STMT_NUM t_lineNum) = 0;
  virtual LIST_OF_STMT_NUMS getStmtNumsFromProcName(PROC_NAME t_procName) = 0;

  ///////////////////////////////////////////////////////
  //  ProcTable
  ///////////////////////////////////////////////////////
  /**
  * Returns all procedure name in the program
  */
  virtual LIST_OF_RESULTS getAllProcsName() = 0;

  ///////////////////////////////////////////////////////
  //  ModifiesP methods
  ///////////////////////////////////////////////////////

  /*
  * Returns true if the procedure has a modifies relation of that variable.
  * Used in the query evaluator for Modifies("First", "x").
  *
  * @param t_procName the procedure name that has the modifies relation.
  * @param t_varName the variable name of the variable that is modified.
  */
  virtual BOOLEAN isModifiesP(const PROC_NAME& t_procName, const VAR_NAME& t_varName) = 0;

  /*
  * Returns true if the procedure has a modifies relation.
  * Used in the query evaluator for Modifies("First", _).
  *
  * @param t_procName the procedure name that is checked.
  */
  virtual BOOLEAN isModifiesInProc(const PROC_NAME& t_procName) = 0;

  /*
  * Returns the list of variable names that are modified in the procedure specified.
  * Used in the query evaluator for Modifies("First", x).
  *
  * @param t_procName the procedure name that is checked.
  */
  virtual LIST_OF_VAR_NAMES getModifiesPVarNamesWithProcIdx(const PROC_NAME& t_procName) = 0;

  /*
  * Returns the list of procedure names that are modified by the variable.
  * Used in the query evaluator for Modifies(p, "x").
  *
  * @param t_varName the variable name that is checked.
  */
  virtual LIST_OF_PROC_NAMES getModifiesPProcNamesWithVarIdx(const VAR_NAME& t_varName) = 0;

  /*
  * Returns a results of a set of procedures mapped to a list of variables that they modifies.
  * Used in the query evaluator for Modifies(p, x);
  *
  */
  virtual MAP_OF_PROC_TO_VAR getModifiesPAllProcToVar() = 0;

  /*
  * Returns a list of procedures that modifies something.
  * Used in the query evaluator for  Modifies(p, _)
  *
  */
  virtual LIST_OF_PROC_NAMES getModifiesPAllProcNames() = 0;

  ///////////////////////////////////////////////////////
  //  UsesP methods
  ///////////////////////////////////////////////////////

  /*
  * Returns true if the procedure has a uses relation of that variable.
  * Used in the query evaluator for Uses("First", "x").
  *
  * @param t_procName the procedure name that has the uses relation.
  * @param t_varName the variable name of the variable that is used.
  */
  virtual BOOLEAN isUsesP(const PROC_NAME& t_procName, const VAR_NAME& t_varName) = 0;

  /*
  * Returns true if the procedure has a uses relation.
  * Used in the query evaluator for Uses("First", _).
  *
  * @param t_procName the procedure name that is checked.
  */
  virtual BOOLEAN isUsesInProc(const PROC_NAME& t_procName) = 0;

  /*
  * Returns the list of variable names that are used in the procedure specified.
  * Used in the query evaluator for Uses("First", x).
  *
  * @param t_procName the procedure name that is checked.
  */
  virtual LIST_OF_VAR_NAMES getUsesPVarNamesWithProcIdx(const PROC_NAME& t_procName) = 0;

  /*
  * Returns the list of procedure names that are used by the variable.
  * Used in the query evaluator for Uses(p, "x").
  *
  * @param t_varName the variable name that is checked.
  */
  virtual LIST_OF_PROC_NAMES getUsesPProcNamesWithVarIdx(const VAR_NAME& t_varName) = 0;

  /*
  * Returns a results of a set of procedures mapped to a list of variables that they uses.
  * Used in the query evaluator for Uses(p, x);
  *
  */
  virtual MAP_OF_PROC_TO_VAR getUsesPAllProcToVar() = 0;

  /*
  * Returns a list of procedures that uses something.
  * Used in the query evaluator for  Uses(p, _)
  *
  */
  virtual LIST_OF_PROC_NAMES getUsesPAllProcNames() = 0;

  ///////////////////////////////////////////////////////
  //  Uses methods
  ///////////////////////////////////////////////////////
  /**
  * Method that checks if uses(t_s1, t_varname) holds.
  * @param t_lineNum an integer argument.
  * @param s2 t_varName a string argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual BOOLEAN isUses(STMT_NUM t_lineNum, std::string t_varName) = 0;
  
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
  virtual MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS getAllStmtUses() = 0;

  /**
  * Method that checks if uses(t_lineNum, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one uses relationship with t_lineNum being the statement number, false if otherwise.
  */
  virtual BOOLEAN isUsesAnything(STMT_NUM t_lineNum) = 0;

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
  virtual BOOLEAN isModifies(STMT_NUM t_lineNum, std::string t_varName) = 0;

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
  virtual MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS getAllStmtModifies() = 0;

  /**
  * Method that checks if modifies(t_lineNum, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one modifies relationship with t_lineNum being the statement number, false if otherwise.
  */
  virtual BOOLEAN isModifiesAnything(STMT_NUM t_lineNum) = 0;

  /**
  * Method that returns the vector of line numbers that involves modification of variables.
  * For example: stmt s; modifies(s, _).
  * @return a vector of statement numbers that satisfy the condition.
  */
  virtual LIST_OF_STMT_NUMS getStmtModifiesAnything() = 0;

  ///////////////////////////////////////////////////////
  //  StmtListTable
  ///////////////////////////////////////////////////////

  /*
  * Returns all statemnet list in the program.
  */
  virtual LIST_OF_STMT_NUMS& getStmtList() = 0;

  ///////////////////////////////////////////////////////
  //  Next Table
  ///////////////////////////////////////////////////////

  /** Checks if Next(line1, line2) is true.
  *   @param t_line1 the program line before
  *   @param t_line2 the program line after
  *   @return true if relationship exists, else false
  */
  virtual BOOLEAN isNext(PROG_LINE t_line1, PROG_LINE t_line2) = 0;

  /** Checks if Next*(line1, line2) is true.
  *   @param t_line1 the program line before
  *   @param t_line2 the program line after
  *   @return true if relationship exists, else false
  */
  virtual BOOLEAN isNextStar(PROG_LINE t_line1, PROG_LINE t_line2) = 0;

  /** For Next(line, l) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed directly after given line.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  virtual LIST_OF_PROG_LINES getLinesAfter(PROG_LINE t_line) = 0;

  /** For Next(l, line) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed directly before given line.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  virtual LIST_OF_PROG_LINES getLinesBefore(PROG_LINE t_line) = 0;

  /** For Next*(line, l) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed after given line, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  virtual LIST_OF_PROG_LINES getAllLinesAfter(PROG_LINE t_line) = 0;

  /** For Next*(l, line) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed before given line, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  virtual LIST_OF_PROG_LINES getAllLinesBefore(PROG_LINE t_line) = 0;

  /** For Next(l1, l2) where l1, l2 is a common synonym for all lines.
  *   Gets map of all lines, each with a corresponding list of lines that can be executed directly after it.
  *   @return map of <program line number, list of lines executed after it>
  */
  virtual MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES getAllNext() = 0;

  /** For Next*(l1, l2) where l1, l2 is a common synonym for all lines.
  *   Gets map of all lines, each with a corresponding list of lines that can be executed after it, either directly or in some execution sequence.
  *   @return map of <program line number, list of lines executed after it>
  */
  virtual MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES getAllNextStar() = 0;

  /** For Next(_, l) and Next*(_, l) where l is a common synonym for all lines.
  *   Gets list of all lines that can be executed after any particular line.
  *   @return list of program line numbers
  */
  virtual LIST_OF_PROG_LINES getAllLinesAfterAnyLine() = 0;

  /** For Next(l, _) and Next*(l, _) where l is a common synonym for all lines.
  *   Gets list of all lines that can be executed before any particular line.
  *   @return list of program line numbers
  */
  virtual LIST_OF_PROG_LINES getAllLinesBeforeAnyLine() = 0;

  /** For Next(_, _) or Next*(_, _).
  *   Checks if any Next relationship exists.
  *   @return true if data structure contains at least one Next(), else false
  */
  virtual BOOLEAN hasNextRelationship() = 0;

  /** For Next(line, _) and Next*(line, _), where line is a given line number.
  *   Checks if given line has any lines that can be executed after it, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return true if given line has at least one line that can be executed after it, else false
  */
  virtual BOOLEAN hasNextLine(PROG_LINE t_line) = 0;

  /** For Next(_, line) and Next*(_, line), where line is a given line number.
  *   Checks if given line has any lines that can be executed before it, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return true if given line has at least one line that can be executed before it, else false
  */
  virtual BOOLEAN hasLineBefore(PROG_LINE t_line) = 0;

  ///////////////////////////////////////////////////////
  //  Affects Table
  ///////////////////////////////////////////////////////
  virtual SET_OF_AFFECTS getAllAffects() = 0; // affects(a1,a2)
  virtual LIST_OF_AFFECTS_STMTS getAffects(STMT_NUM t_modifiesLine) = 0; // affects(2,a)
  virtual LIST_OF_AFFECTS_STMTS getAffectedBy(STMT_NUM t_usesLine) = 0; // affects(a,12)
  virtual BOOLEAN isAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) = 0; // affects(1,12)
  virtual BOOLEAN hasAffectsRelationship() = 0; // affects(_,_)
  virtual LIST_OF_AFFECTS_STMTS getAffectsAnything() = 0;  // affects(a,_)
  virtual LIST_OF_AFFECTS_STMTS getAffectedByAnything() = 0; // affects(_,a)
  virtual BOOLEAN isAffectsAnything(STMT_NUM t_modifiesLine) = 0; // affects(1,_)
  virtual BOOLEAN isAffectedByAnything(STMT_NUM t_usesLines) = 0; // affects(_,12)
};