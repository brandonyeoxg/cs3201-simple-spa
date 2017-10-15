#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "FollowTable.h"
#include "ParentTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "VarTable.h"
#include "AssignTable.h"
#include "StatementTable.h"
#include "Grammar.h"
#include "ConstantTable.h"
#include "GlobalTypeDef.h"
#include "PatternMatch.h"
#include "PkbWriteOnly.h"
#include "PkbReadOnly.h"
#include "PkbTablesOnly.h"
#include "ModifiesP.h"
#include "UsesP.h"
#include "CallsTable.h"
#include "UsesTable.h"
#include "ModifiesTable.h"
#include "StmtListTable.h"
#include "NextTable.h"

class PKB: public PkbWriteOnly, public PkbReadOnly, public PkbTablesOnly {

public:
  PKB();
  ~PKB();

  ///////////////////////////////////////////////////////
  //  PKB building methods
  ///////////////////////////////////////////////////////

  /**
  * Inserts a procedure into the PKB. AST is built from this procedure node.
  * @param t_procName name of the procedure.
  * @return a reference to the StmtListNode created from inserting the procedure.
  */
  PROC_INDEX insertProcedure(const PROC_NAME& t_procName);

  /**
  * Inserts a follows relation in the PKB.
  * @param t_node reference to the StmtLst that this statement belongs to.
  * @param t_curLineNum the current line number.
  * @return true if the table is successfully added.
  */
  BOOLEAN insertFollowsRelation(const LIST_OF_STMT_NUMS& t_stmtInStmtList, int t_curLineNum);

  /** Insert relationship Next(line1, line2) into PKB.
  *   @param t_line1 the program line before
  *   @param t_line2 the program line after
  */
  void insertNextRelation(PROG_LINE t_line1, PROG_LINE t_line2);

  /**
  * Inserts a parent relation in the PKB.
  * @param t_node reference to the StmtLst that this statement belongs to.
  * @param t_curLineNum the current line number.
  * @return true if the table is successfully added.
  */
  BOOLEAN insertParentRelation(const LIST_OF_STMT_NUMS& t_nestedStmtInStmtList, int t_curLineNum);

  void insertModifies(PROC_INDEX t_procIdx, VAR_NAME t_varName, LIST_OF_STMT_NUMS t_nestedStmtLines, STMT_NUM t_curLineNum);

  void insertUses(PROC_INDEX t_procIdx, VAR_NAME t_varName, LIST_OF_STMT_NUMS t_nestedStmtLines, STMT_NUM t_curLineNum);
  
  /**
  * Inserts a variable that has been modified.
  * @param t_varName name of the variable being modified.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  void insertModifiesVariable(VAR_NAME t_varName, STMT_NUM t_curLineNum,
    LIST_OF_STMT_NUMS t_nestedStmtLines);

  /**
  * Inserts a variable that has been used.
  * @param t_varName name of the variable that is used.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  void insertUsesVariable(VAR_NAME t_varName, STMT_NUM m_curLineNum, LIST_OF_STMT_NUMS t_nestedStmtLines);

  /**
  * Inserts a variable that has been modified to ModifiesP
  * @param t_varName name of the variable being modified.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  void insertModifiesProc(PROC_INDEX t_procIdx, const VAR_NAME& t_varName);

  /**
  * Inserts a variable that has been used to UsesP
  * @param t_varName name of the variable that is used.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  void insertUsesProc(PROC_INDEX t_procIdx, const VAR_NAME& t_varName);

  /**
  * Inserts an assignment statement into the PKB
  * @param t_lineNum the line number that the assignment statement is at.
  * @param t_tokens tokenised expression for the right side of the "=" operator
  */
  void insertAssignStmt(STMT_NUM t_lineNum, VAR_NAME t_varName, LIST_OF_TOKENS t_stmtTokens);

  /**
  * Inserts a call statement into the PKB
  */
  void insertCallStmt(PROC_INDEX t_proc1, PROC_NAME t_proc2, STMT_NUM t_lineNum);

  /**
  * Inserts a while statement into the PKB.
  * @param t_parentNode reference to the parent node that this while loop belongs to.
  * @param t_varaibleNode reference to the variable node that this while loop contains.
  * @param t_curLineNum the current line number that this while statement is at.
  * @return a reference of the while node.
  */
  STMT_NUM insertWhileStmt(PROC_INDEX t_procIdx, VAR_NAME varName, LIST_OF_STMT_NUMS m_nestedStmtLineNum, STMT_NUM t_curLineNum);

  /**
  * Inserts a if statement into the PKB.
  * @param t_parentNode reference to the parent node that this while loop belongs to.
  * @param t_varaibleNode reference to the variable node that this while loop contains.
  * @param t_curLineNum the current line number that this while statement is at.
  * @return a reference of the while node.
  */
  STMT_NUM insertIfStmt(PROC_INDEX t_procIdx, VAR_NAME t_varName, LIST_OF_STMT_NUMS t_nestedStmtLinNum, STMT_NUM t_curLineNum);

  /**
  * Inserts a else statement into the PKB.
  * @param t_parentNode reference to the parent node that this while loop belongs to.
  * @param t_curLineNum the current line number that this while statement is at.
  * @return a reference of the while node.
  */
  STMT_NUM insertElseStmt(PROC_INDEX t_procIdx, LIST_OF_STMT_NUMS t_nestedStmtLineNum, STMT_NUM t_curLineNum);


  /**
  * Inserts a constant into the PKB.
  * @param t_constVal the constant to be added in string form.
  * @param t_curLineNum the current line of the constant.
  * @return a reference to the constant node.
  */
  void insertConstant(CONSTANT_TERM t_constVal);

  void insertStmtList(STMT_NUM t_line);
  ///////////////////////////////////////////////////////
  //  FollowTable methods
  ///////////////////////////////////////////////////////
  FollowTable* getFollowTable();

  /**
  * Method that checks if follows(s1, s2) holds.
  * Checks if s2 exists as the first element in vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  BOOLEAN isFollows(STMT_NUM t_s1, STMT_NUM t_s2);

  /**
  * Method that checks if follows*(s1, s2) holds.
  * Checks if s2 exists within the vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  BOOLEAN isFollowsStar(STMT_NUM t_s1, STMT_NUM t_s2);

  /**
  * Method that returns the line number that follows(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param s1 an integer argument.
  * @return the line number that line s1 follows.
  */
  STMT_NUM getFollows(STMT_NUM t_s1);

  /**
  * Method that returns the line number that follows(s, s2) holds, where s is a variable and s2 is a known line number.
  * @param s2 an integer argument.
  * @return the line number that is followed by line s2.
  */
  STMT_NUM getFollowedBy(STMT_NUM t_s2);

  /**
  * Method that returns the list of line numbers that follows*(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param s1 an integer argument.
  * @return the vector of line numbers that line s1 follows*.
  */
  LIST_OF_STMT_NUMS getFollowsStar(STMT_NUM t_s1);

  /**
  * Method that returns the list of line numbers that follows*(s, s2) holds, where s is a variable and s2 is a known line number.
  * Returns the vector of keys that the vector mapped to the key contains s2.
  * @param s1 an integer argument.
  * @return the vector of line numbers that are followedBy* s2.
  */
  LIST_OF_STMT_NUMS getFollowedByStar(STMT_NUM t_s2);

  /**
  * Method that returns the entire map of line numbers that satisfy the follow* relationship.
  * @return the entire map that keep tracks of the follow relationship.
  */
  MAP_OF_STMT_NUMS getAllFollows();

  /**
  * Method that returns the entire map of line numbers that satisfy the follow* relationship.
  * @return the entire map that keep tracks of the follow relationship.
  */
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllFollowsStar();

  /**
  * Method that returns the list of line numbers that is followed by another statement.
  * @return the vector of keys within the followTable.
  */
  LIST_OF_STMT_NUMS getFollowedByAnything();

  /**
  * Method that returns the list of line numbers that follows a statement.
  * @return the vector of keys within the followTable.
  */
  LIST_OF_STMT_NUMS getFollowsAnything();

  /**
  * Method that checks if follows(_, _) or follows*(_, _) holds.
  * @return true if the size of the followTable is more than zero, return false if otherwise.
  */
  BOOLEAN hasFollowRelationship();

  /**
  * Method that checks if follows(_, s2) and follows*(_, s2) holds, where s2 is a statement number.
  * @return true if s2 exists in the allFollows map, return false if otherwise.
  */
  BOOLEAN isFollowsAnything(STMT_NUM t_s2);

  /**
  * Method that checks if follows(2, _) and follows*(2, _) holds, where s2 is a statement number.
  * @return true if t_s2 exists in the allFollows map, return false if otherwise.
  */
  BOOLEAN isFollowedByAnything(STMT_NUM t_s1);

  ///////////////////////////////////////////////////////
  //  ParentTable methods
  ///////////////////////////////////////////////////////
  ParentTable* getParentTable();

  /**
  * Method that checks if parent(s1, s2) holds.
  * Checks if s2 exists as value mapped to key s1 in parentMap.
  * @param t_s1 an integer argument.
  * @param t_s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  BOOLEAN isParent(STMT_NUM t_s1, STMT_NUM t_s2);

  /**
  * Method that checks if parent*(s1, s2) holds.
  * Checks if s2 exists in vector mapped to key s1 in parentStarMap.
  * @param t_s1 an integer argument.
  * @param t_s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  BOOLEAN isParentStar(STMT_NUM t_s1, STMT_NUM t_s2);

  /**
  * Method that returns the statement number that is the parent of s2.
  * Checks if key s2 exists in parentMap.
  * @throw invalid_arguement exception if key does not exist.
  * @param t_s2 an integer argument.
  * @return a statement number. 
  */
  STMT_NUM getParentOf(STMT_NUM t_s2);

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent(t_s1, x).
  * Returns empty vector if no relationship holds for all statement numbers.
  * @param t_s1 an integer argument.
  * @return a vector of statement numbers.
  */
  LIST_OF_STMT_NUMS getChildrenOf(STMT_NUM t_s1);

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent*(x, t_s2).
  * Returns empty vector if no relationship holds for all statement numbers.
  * @param t_s2 an integer argument.
  * @return a vector of statement numbers.
  */
  LIST_OF_STMT_NUMS getParentStarOf(STMT_NUM t_s2);

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent*(t_s1, x).
  * Returns empty vector if no relationship holds for all statement numbers.
  * @param t_s1 an integer argument.
  * @return a vector of statement numbers.
  */
  LIST_OF_STMT_NUMS getChildrenStarOf(STMT_NUM t_s1);

  /**
  * Method that returns the entire map of line numbers that satisfy the parent relationship.
  * @return the unordered map that keep tracks of the parent relationship.
  */
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllParents();

  /**
  * Method that returns the entire map of line numbers that satisfy the parent* relationship.
  * @return the unordered map that keep tracks of the parent* relationship.
  */
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllParentsStar();

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent(_, x).
  * @return the vector of keys within the parentTable.
  */
  LIST_OF_STMT_NUMS getChildrenOfAnything();

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent(x, _).
  * @return the vector of keys within the parentTable.
  */
  LIST_OF_STMT_NUMS getParentOfAnything();

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent*(_, x).
  * @return the vector of keys within the parentTable.
  */
  LIST_OF_STMT_NUMS getChildrenStarOfAnything();

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent*(x, _).
  * @return the vector of keys within the parentTable.
  */
  LIST_OF_STMT_NUMS getParentStarOfAnything();

  /**
  * Method that checks if parent(_, _) holds.
  * @return true if there exists a parent relationship (i.e. parentTable is not empty), false if otherwise.
  */
  BOOLEAN hasParentRelationship();

  /**
  * Method that checks if parent*(_, _) holds.
  * @return true if there exists a parent* relationship (i.e. parentStarTable is not empty), false if otherwise.
  */
  BOOLEAN hasParentStarRelationship();

  /**
  * Method that checks if parent(_, t_s2) holds.
  * @param t_s2 an integer argument.
  * @return true if there exists a parent relationship with t_s2 being the child, false if otherwise.
  */
  BOOLEAN isChildrenOfAnything(STMT_NUM t_s2);

  /**
  * Method that checks if parent(t_s1, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one parent relationship with t_s1 being the parent, false if otherwise.
  */
  BOOLEAN isParentOfAnything(STMT_NUM t_s1);

  /**
  * Method that checks if parent*(_, t_s2) holds.
  * @param t_s2 an integer argument.
  * @return true if there exists at least one parent* relationship with t_s2 being the child, false if otherwise.
  */
  BOOLEAN isChildrenOfStarAnything(STMT_NUM t_s2);

  /**
  * Method that checks if parent*(t_s1, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one parent* relationship with t_s1 being the parent, false if otherwise.
  */
  BOOLEAN isParentOfStarAnything(STMT_NUM t_s1);

  //////////////////////////////////////////////////////////
  //  statementTypeTable and typeOfStatementTable Methods
  //////////////////////////////////////////////////////////
  StatementTable* getStatementTable();

  /**
  * Method that returns the entire map that tracks every statement's type.
  * @return the unordered map that keep tracks of type of statements.
  */
  MAP_OF_STMT_NUM_TO_GTYPE getTypeOfStatementTable();

  /**
  * Method that insert statement number and GType into typeofStatementTable.
  * @param t_lineNum a STMT_NUM argument.
  * @param t_type a GType argument.
  * @return true if insertion is successful, false if otherwise.
  */
  BOOLEAN insertTypeOfStatementTable(STMT_NUM t_lineNum, queryType::GType t_type);

  /**
  * Method that returns the entire map of the list of statements that each type of statement has.
  * @return the unordered map that keep tracks of type of statements.
  */
  MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS  getStatementTypeTable();

  /**
  * Method that insert statement number and GType into statementTypeTable.
  * @param t_lineNum a STMT_NUM argument.
  * @param t_type a GType argument.
  * @return true if insertion is successful, false if otherwise.
  */
  BOOLEAN insertStatementTypeTable(queryType::GType t_type, STMT_NUM t_lineNum);

  ///////////////////////////////////////////////////////
  //  VarTable methods
  ///////////////////////////////////////////////////////
  VarTable* getVarTable();
  VAR_INDEX insertVar(VAR_NAME t_name);

  /**
  * Method that returns the index of the variable t_varName in VarTable.
  * @param t_varName a string argument.
  * @return the index of variable.
  */
  VAR_NAME getVarNameFromIdx(VAR_INDEX t_idx);

  /**
  * Method that returns the index of the variable t_varName in VarTable.
  * @param t_varName a string argument.
  * @return the index of variable.
  */
  VAR_INDEX getVarIdxFromName(VAR_NAME t_varName);

  /**
  * Method that returns the vector of variables that are stored within VarTable.
  * @return a vector of statement numbers.
  */
  LIST_OF_VAR_NAMES& getAllVarNames();
  LIST_OF_STMT_NUMS getListOfStatements(queryType::GType t_type);

  ///////////////////////////////////////////////////////
  //  AssignTable
  ///////////////////////////////////////////////////////
  AssignTable* getAssignTable();
  /**
  * Returns all assignment statements number that modifies the variable name.
  * @param t_varName the name of the variable.
  */
  LIST_OF_STMT_NUMS getAllAssignStmtListByVar(VAR_NAME& t_varName);

  /**
  * Returns all assignment statements.
  */
  LIST_OF_STMT_NUMS getAllAssignStmts();

  /**
  * Returns all assignment statements in a representation.
  * The representation is a variable mapped to all statement number under that variable.
  */
  MAP_OF_VAR_NAME_TO_STMT_NUMS getAllVarNameWithAssignStmt();

  /**
  * Returns all assignment statements in a representation.
  * The repsentation is a statement number mapped to the variable in that statement number.
  */
  MAP_OF_STMT_NUM_TO_VAR_NAME getAllAssignStmtWithVarName();
  ///////////////////////////////////////////////////////
  //  ProcTable
  ///////////////////////////////////////////////////////
  ProcTable* getProcTable();

  /**
  * Returns all procedure name in the program
  */
  LIST_OF_RESULTS getAllProcsName();
  ///////////////////////////////////////////////////////
  //  ConstantTable methods
  ///////////////////////////////////////////////////////
  LIST_OF_RESULTS getAllConstants();

  ///////////////////////////////////////////////////////
  //  Pattern Matching
  ///////////////////////////////////////////////////////

  ////////////////// OLD METHOD
  std::list<STMT_NUM> getAllAssignStmtByExactPattern(std::string t_patternTokens);

  ////////////////// OLD METHOD
  std::list<STMT_NUM> getAllAssignStmtBySubtreePattern(std::string t_patternTokens);

  ////////////////// OLD METHOD
  std::list<STMT_NUM> getAllAssignStmtByVarAndExactPattern(std::string t_varName, std::string t_pattern);

  ////////////////// OLD METHOD
  std::list<STMT_NUM> getAllAssignStmtByVarAndSubtreePattern(std::string t_varName, std::string t_pattern);

  ////////////////// OLD METHOD
  std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVarByExactPattern(std::string t_pattern);

  ////////////////// OLD METHOD
  std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVarBySubtreePattern(std::string t_pattern);


  /** Pattern a(_, "x + y + h").
  *   Gets list of statements with exact pattern match on right hand side, and any variable on left hand side.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  std::list<STMT_NUM> getAllAssignStmtByExactPattern(std::vector<std::string> t_patternTokens);

  /** Pattern a(_, _"x + y + h"_).
  *   Gets list of statements with subtree pattern match on right hand side, and any variable on left hand side.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  std::list<STMT_NUM> getAllAssignStmtBySubtreePattern(std::vector<std::string> t_patternTokens);

  /** Pattern a("x", _""_).
  *   Gets list of statements with any expression on right hand side, and given variable on left hand side.
  *   @param t_varName name of variable on left hand side
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  LIST_OF_STMT_NUMS getAllAssignStmtByVar(std::string t_varName);

  /** Pattern a("x", "y + x").
  *   Gets list of statements with given variable name on left hand side, and exact pattern match on right hand side.
  *   @param t_varName name of the variable modified in assignment statement
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  std::list<STMT_NUM> getAllAssignStmtByVarAndExactPattern(std::string t_varName, std::vector<std::string> t_patternTokens);

  /** Pattern a("x", _"y + x"_).
  *   Gets list of statements with given variable name on left hand side, and subtree pattern match on right hand side.
  *   @param t_varName name of the variable modified in assignment statement
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  std::list<STMT_NUM> getAllAssignStmtByVarAndSubtreePattern(std::string t_varName, std::vector<std::string> t_patternTokens);

  /** variable v; Pattern a(v, "x + y + h").
  *   Gets map of statements with exact pattern match on right hand side, and any variable on left hand side.
  *   Map will be returned with statement number as key, and variable name as value.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return map of statement numbers to their respective variable names (will be empty if none)
  *   @author jazlyn
  */
  std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVarByExactPattern(std::vector<std::string> t_patternTokens);

  /** variable v; Pattern a(v, _"x + y + h"_).
  *   Gets map of statements with subtree pattern match on right hand side, and any variable on left hand side.
  *   Map will be returned with statement number as key, and variable name as value.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return map of statement numbers to their respective variable names (will be empty if none)
  *   @author jazlyn
  */
  std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVarBySubtreePattern(std::vector<std::string> t_patternTokens);

  /** For Pattern w("x", _) */
  LIST_OF_STMT_NUMS getWhileStmtByVar(STRING varName);

  /** For Pattern w(v, _) */
  std::unordered_map<STMT_NUM, VAR_NAME> getAllWhileStmtsWithVar();

  /** For Pattern w(_,  _) */
  LIST_OF_STMT_NUMS getAllWhileStmts();

  /** For Pattern i("x", _) */
  LIST_OF_STMT_NUMS getIfStmtByVar(STRING varName);

  /** For Pattern i(v, _) */
  std::unordered_map<STMT_NUM, VAR_NAME> getIfStmtWithVar();

  /** For Pattern i(_,  _) */
  LIST_OF_STMT_NUMS getAllIfStmts();

  ///////////////////////////////////////////////////////
  //  CallsTable methods
  ///////////////////////////////////////////////////////
  CallsTable* getCallsTable();
  BOOLEAN isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);
  BOOLEAN isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2);
  LIST_OF_PROC_NAMES getCalls(PROC_NAME t_proc2);
  LIST_OF_PROC_NAMES getCalledBy(PROC_NAME t_proc1);
  LIST_OF_PROC_NAMES getCallsStar(PROC_NAME t_proc2);
  LIST_OF_PROC_NAMES getCalledByStar(PROC_NAME t_proc1);
  std::unordered_map<PROC_NAME, PROC_NAME> getAllCalls();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> getAllCallsStar(); //calls*(proc1, proc2)
  LIST_OF_PROC_NAMES getCallsAnything();  //calls(proc1, _)
  LIST_OF_PROC_NAMES getCallsStarAnything();  //calls*(proc1, _)
  LIST_OF_PROC_NAMES getCalledByAnything(); //calls(_, proc2)
  LIST_OF_PROC_NAMES getCalledByStarAnything(); //calls*(_, proc2)
  BOOLEAN hasCallsRelationship();  //calls(_, _)
  BOOLEAN isCallsAnything(PROC_NAME t_proc1);
  BOOLEAN isCalledByAnything(PROC_NAME t_proc2);

  ///////////////////////////////////////////////////////
  //  ModifiesP methods
  ///////////////////////////////////////////////////////
  ModifiesP* getModifiesP();

  /**
  * Returns true if the procedure has a modifies relation of that variable.
  * Used in the query evaluator for Modifies("First", "x").
  *
  * @param t_procName the procedure name that has the modifies relation.
  * @param t_varName the variable name of the variable that is modified.
  */
  BOOLEAN isModifiesP(const PROC_NAME& t_procName, const VAR_NAME& t_varName);
  
  /**
  * Returns true if the procedure has a modifies relation.
  * Used in the query evaluator for Modifies("First", _).
  *
  * @param t_procName the procedure name that is checked.
  */
  BOOLEAN isModifiesInProc(const PROC_NAME& t_procName);

  /**
  * Returns the list of variable names that are modified in the procedure specified.
  * Used in the query evaluator for Modifies("First", x).
  *
  * @param t_procName the procedure name that is checked.
  */
  LIST_OF_VAR_NAMES getModifiesPVarNamesWithProcIdx(const PROC_NAME& t_procName);
  
  /**
  * Returns the list of procedure names that are modified by the variable.
  * Used in the query evaluator for Modifies(p, "x").
  *
  * @param t_varName the variable name that is checked.
  */
  LIST_OF_PROC_NAMES getModifiesPProcNamesWithVarIdx(const VAR_NAME& t_varName);
  
  /**
  * Returns a results of a set of procedures mapped to a list of variables that they modifies.
  * Used in the query evaluator for Modifies(p, x);
  *
  */
  MAP_OF_PROC_TO_VAR getModifiesPAllProcToVar();

  /**
  * Returns a list of procedures that modifies something.
  * Used in the query evaluator for  Modifies(p, _)
  *
  */
  LIST_OF_PROC_NAMES getModifiesPAllProcNames();

  ///////////////////////////////////////////////////////
  //  UsesP methods
  ///////////////////////////////////////////////////////
  UsesP* getUsesP();

  /**
  * Returns true if the procedure has a uses relation of that variable.
  * Used in the query evaluator for Uses("First", "x").
  *
  * @param t_procName the procedure name that has the uses relation.
  * @param t_varName the variable name of the variable that is used.
  */
  BOOLEAN isUsesP(const PROC_NAME& t_procName, const VAR_NAME& t_varName);
  
  /**
  * Returns true if the procedure has a uses relation.
  * Used in the query evaluator for Uses("First", _).
  *
  * @param t_procName the procedure name that is checked.
  */
  BOOLEAN isUsesInProc(const PROC_NAME& t_procName);
  
  /**
  * Returns the list of variable names that are used in the procedure specified.
  * Used in the query evaluator for Uses("First", x).
  *
  * @param t_procName the procedure name that is checked.
  */
  LIST_OF_VAR_NAMES getUsesPVarNamesWithProcIdx(const PROC_NAME& t_procName);

  /**
  * Returns the list of procedure names that are used by the variable.
  * Used in the query evaluator for Uses(p, "x").
  *
  * @param t_varName the variable name that is checked.
  */
  LIST_OF_PROC_NAMES getUsesPProcNamesWithVarIdx(const VAR_NAME& t_varName);

  /**
  * Returns a results of a set of procedures mapped to a list of variables that they uses.
  * Used in the query evaluator for Uses(p, x);
  *
  */
  MAP_OF_PROC_TO_VAR getUsesPAllProcToVar();

  /**
  * Returns a list of procedures that uses something.
  * Used in the query evaluator for  Uses(p, _)
  *
  */
  LIST_OF_PROC_NAMES getUsesPAllProcNames();

  ///////////////////////////////////////////////////////
  //  UsesTable methods
  ///////////////////////////////////////////////////////
  UsesTable* getUsesTable();
  void insertUsesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum);
  BOOLEAN isUses(STMT_NUM t_lineNum, VAR_NAME t_varName);
  LIST_OF_VAR_NAMES getUses(STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getStmtUses(VAR_NAME t_varName);
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS getAllStmtUses();
  BOOLEAN isUsesAnything(STMT_NUM t_lineNum);  //uses(2, _)
  LIST_OF_STMT_NUMS getStmtUsesAnything(); //uses(s, _)

  ///////////////////////////////////////////////////////
  //  ModifiesTable methods
  ///////////////////////////////////////////////////////
  ModifiesTable* getModifiesTable();
  void insertModifiesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum);
  BOOLEAN isModifies(STMT_NUM t_lineNum, VAR_NAME t_varName);
  LIST_OF_VAR_NAMES getModifies(STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getStmtModifies(VAR_NAME t_varName);
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS getAllStmtModifies();
  BOOLEAN isModifiesAnything(STMT_NUM t_lineNum);  //modifies(2, _)
  LIST_OF_STMT_NUMS getStmtModifiesAnything(); //modifies(s, _)

  ///////////////////////////////////////////////////////
  //  StmtListTable
  ///////////////////////////////////////////////////////

  /*
  * Returns all statemnet list in the program.
  *
  */
  LIST_OF_STMT_NUMS& getStmtList();

  ///////////////////////////////////////////////////////
  //  NextTable methods
  ///////////////////////////////////////////////////////
  NextTable* getNextTable();
  /** To be executed after all Next relationships are added to NextTable.
  *   Populates additional design abstractions.
  */
  void executeAfterAllNextInserts();

  /** Checks if Next(line1, line2) is true.
  *   @param t_line1 the program line before
  *   @param t_line2 the program line after
  *   @return true if relationship exists, else false
  */
  BOOLEAN isNext(PROG_LINE t_line1, PROG_LINE t_line2);

  /** Checks if Next*(line1, line2) is true.
  *   @param t_line1 the program line before
  *   @param t_line2 the program line after
  *   @return true if relationship exists, else false
  */
  BOOLEAN isNextStar(PROG_LINE t_line1, PROG_LINE t_line2);

  /** For Next(line, l) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed directly after given line.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  std::vector<PROG_LINE> getLinesAfter(PROG_LINE t_line);

  /** For Next(l, line) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed directly before given line.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  std::vector<PROG_LINE> getLinesBefore(PROG_LINE t_line);

  /** For Next*(line, l) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed after given line, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  std::vector<PROG_LINE> getAllLinesAfter(PROG_LINE t_line);

  /** For Next*(l, line) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed before given line, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  std::vector<PROG_LINE> getAllLinesBefore(PROG_LINE t_line);

  /** For Next(l1, l2) where l1, l2 is a common synonym for all lines.
  *   Gets map of all lines, each with a corresponding list of lines that can be executed directly after it.
  *   @return map of <program line number, list of lines executed after it>
  */
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> getAllNext();

  /** For Next*(l1, l2) where l1, l2 is a common synonym for all lines.
  *   Gets map of all lines, each with a corresponding list of lines that can be executed after it, either directly or in some execution sequence.
  *   @return map of <program line number, list of lines executed after it>
  */
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> getAllNextStar();

  /** For Next(_, l) and Next*(_, l) where l is a common synonym for all lines.
  *   Gets list of all lines that can be executed after any particular line.
  *   @return list of program line numbers
  */
  std::vector<PROG_LINE> getAllLinesAfterAnyLine();

  /** For Next(l, _) and Next*(l, _) where l is a common synonym for all lines.
  *   Gets list of all lines that can be executed before any particular line.
  *   @return list of program line numbers
  */
  std::vector<PROG_LINE> getAllLinesBeforeAnyLine();

  /** For Next(_, _) or Next*(_, _).
  *   Checks if any Next relationship exists.
  *   @return true if data structure contains at least one Next(), else false
  */
  BOOLEAN hasNextRelationship();

  /** For Next(line, _) and Next*(line, _), where line is a given line number.
  *   Checks if given line has any lines that can be executed after it, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return true if given line has at least one line that can be executed after it, else false
  */
  BOOLEAN hasNextLine(PROG_LINE t_line);

  /** For Next(_, line) and Next*(_, line), where line is a given line number.
  *   Checks if given line has any lines that can be executed before it, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return true if given line has at least one line that can be executed before it, else false
  */
  BOOLEAN hasLineBefore(PROG_LINE t_line);

private:
  FollowTable* m_followTable;
  ParentTable* m_parentTable;
  VarTable* m_varTable;
  ProcTable* m_procTable;
  AssignTable* m_assignTable;
  ConstantTable* m_constantTable;
  StatementTable* m_statementTable;
  ModifiesP* m_modifiesP;
  UsesP* m_usesP;
  CallsTable* m_callsTable;
  UsesTable* m_usesTable;
  ModifiesTable* m_modifiesTable;
  StmtListTable* m_stmtListTable;
  NextTable* m_nextTable;
  PatternMatch* m_patternMatch;
};
