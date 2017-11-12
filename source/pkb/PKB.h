#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "../GlobalTypeDef.h"

#include "PkbWriteOnly.h"
#include "PkbReadOnly.h"
#include "PkbTablesOnly.h"

#include "relationshipTables/FollowTable.h"
#include "relationshipTables/ParentTable.h"
#include "relationshipTables/VarTable.h"
#include "relationshipTables/ProcTable.h"
#include "relationshipTables/VarTable.h"
#include "relationshipTables/AssignTable.h"
#include "relationshipTables/StatementTable.h"
#include "relationshipTables/ConstantTable.h"
#include "patternMatch/PatternMatch.h"
#include "relationshipTables/ModifiesP.h"
#include "relationshipTables/UsesP.h"
#include "relationshipTables/CallsTable.h"
#include "relationshipTables/UsesTable.h"
#include "relationshipTables/ModifiesTable.h"
#include "relationshipTables/StmtListTable.h"
#include "relationshipTables/NextTable.h"
#include "../design-extractor/DesignExtractor.h"

#include "../pql/query-types/Grammar.h"

class PKB: public PkbWriteOnly, public PkbReadOnly, public PkbTablesOnly {

public:
  PKB();
  PKB(DesignExtractor *t_de);
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

  /**
  * Inserts a modifies relation in the PKB.
  * @param t_procIdx the index of the procedure where the stmt is at.
  * @param t_varName the variable being modified.
  * @param t_nestedStmtLines the stmtLst where the stmt is at.
  * @param t_curLineNum the stmt#.
  */
  void insertModifies(PROC_INDEX t_procIdx, VAR_NAME t_varName, LIST_OF_STMT_NUMS t_nestedStmtLines, STMT_NUM t_curLineNum);

  /**
  * Inserts a uses relation in the PKB.
  * @param t_procIdx the index of the procedure where the stmt is at.
  * @param t_varName the variable being uses.
  * @param t_nestedStmtLines the stmtLst where the stmt is at.
  * @param t_curLineNum the stmt#.
  */
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
  void insertAssignStmt(STMT_NUM t_lineNum, VAR_NAME t_varName, LIST_OF_TOKENS t_stmtTokens, PROC_INDEX t_procName);

  /**
  * Inserts a call statement into the PKB
  * @param t_proc1 the procedure that calls the other procedure.
  * @param t_proc2 the procedure being called.
  * @param t_lineNum the stmt# of the call statement.
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
  */
  void insertConstant(CONSTANT_TERM t_constVal);

  /**
  * Inserts a statement list into the PKB.
  * @param t_line the first stmt# of the statement list.
  */
  void insertStmtList(STMT_NUM t_line);
  ///////////////////////////////////////////////////////
  //  FollowTable methods
  ///////////////////////////////////////////////////////
  FollowTable* getFollowTable();

  /**
  * Method that checks if follows(s1, s2) holds.
  * Checks if s2 exists as the first element in vector mapped to key s1.
  * @param t_s1 the stmt# that is being followed by t_s2.
  * @param t_s2 the stmt# that follows t_s1.
  * @return true if the relationship holds, false if otherwise.
  */
  BOOLEAN isFollows(STMT_NUM t_s1, STMT_NUM t_s2);

  /**
  * Method that checks if follows*(s1, s2) holds.
  * Checks if (s1, s2) is true in the 2D matrix.
  * @param t_s1 the s1 in the above relation.
  * @param s2 an the s2 in the above relation.
  * @return true if the relationship holds, false if otherwise.
  */
  BOOLEAN isFollowsStar(STMT_NUM t_s1, STMT_NUM t_s2);

  /**
  * Method that returns the line number that follows(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param t_s1 the stmt# that followed by the answer.
  * @return the line number that line s1 follows.
  */
  STMT_NUM getFollows(STMT_NUM t_s1);

  /**
  * Method that returns the line number that follows(s, s2) holds, where s is a variable and s2 is a known line number.
  * @param t_s2 the stmt# that follows the ansewer.
  * @return the line number that is followed by line s2.
  */
  STMT_NUM getFollowedBy(STMT_NUM t_s2);

  /**
  * Method that returns the list of line numbers that follows*(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param t_s1 the s1 in the above relation.
  * @return the vector of line numbers.
  */
  LIST_OF_STMT_NUMS getFollowsStar(STMT_NUM t_s1);

  /**
  * Method that returns the list of line numbers that follows*(s, s2) holds, where s is a variable and s2 is a known line number.
  * Returns the vector of keys that the vector mapped to the key contains s2.
  * @param t_s1 the s1 in the above relation.
  * @return the vector of line numbers.
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

  void populateFollowsMatrix(TOTAL_NUMBER_OF_STMTS total);
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
  * Inserts the statement into an unordered_map of stmt# mapped to a type of statement.
  *
  * @param t_lineNum is the stmt#.
  * @param t_type the type of statement.
  * @return true if insertion is successful.
  */
  BOOLEAN insertTypeOfStatementTable(STMT_NUM t_lineNum, queryType::GType t_type);

  /**
  * Method that returns the entire map of the list of statements that each type of statement has.
  * @return the unordered map that keep tracks of type of statements.
  */
  MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS  getStatementTypeTable();

  /**
  * Inserts the statement into an unordered_map of a statement type mapped to a vector of stmt#.
  * @param t_type the type of statement.
  * @param t_lineNum is the stmt#.
  * @return true if insertion is successful.
  */
  BOOLEAN insertStatementTypeTable(queryType::GType t_type, STMT_NUM t_lineNum);

  /**
  * Method to return the total number of statements appeared in the source program.
  * @return the size of statementTypeTable, which equals to total number of lines.
  */
  TOTAL_NUMBER_OF_STMTS getNumberOfStatements();

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
  LIST_OF_VAR_INDICES& getAllVarIndices();

  /**
  * Get the list of statements that are of the same type as the parameter.
  * @param t_type the type of statement.
  * @return the list of stmt#.
  */
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
  MAP_OF_VAR_INDEX_TO_STMT_NUMS getAllVarIndicesWithAssignStmt();

  /**
  * Returns all assignment statements in a representation.
  * The repsentation is a statement number mapped to the variable in that statement number.
  */
  MAP_OF_STMT_NUM_TO_VAR_NAME getAllAssignStmtWithVarName();
  MAP_OF_STMT_NUM_TO_VAR_INDEX getAllAssignStmtWithVarIndex();
  ///////////////////////////////////////////////////////
  //  ProcTable
  ///////////////////////////////////////////////////////
  ProcTable* getProcTable();

  /**
  * Gets all procedure name in the source program.
  * @return the list of procedures.
  */
  LIST_OF_RESULTS getAllProcsName();
  LIST_OF_PROC_INDICES getAllProcsIndices();

  /**
  * Gets the procedure given its index in the procedure table.
  * @param t_idx the procedure's index.
  * @return the name of the procedure.
  */
  PROC_NAME getProcNameFromIdx(PROC_INDEX t_idx);

  /**
  * Gets the procedure index given its name in the procedure table.
  * @param t_name the procedure's name.
  * @return the index of the procedure.
  */
  PROC_INDEX getProcIdxFromName(PROC_NAME t_name);
  ///////////////////////////////////////////////////////
  //  ConstantTable methods
  ///////////////////////////////////////////////////////

  /**
  * Gets all the constants in the source program.
  * @return the list of all constants.
  */
  LIST_OF_RESULTS getAllConstants();
  LIST_OF_CONSTANT_INDICES getAllConstantsByIdx();

  /**
  * Gets the constant index given its string in the Constant table.
  * @param t_constant the constant string.
  * @return the index of the constant.
  */
  CONSTANT_INDEX getConstantIdxFromConstant(CONSTANT_TERM t_constant);

  /**
  * Gets the constant string given its index in the Constant table.
  * @param t_constantIdx the index of the constant.
  * @return the constant string.
  */
  CONSTANT_TERM getConstantFromIdx(int t_constantIdx);

  ///////////////////////////////////////////////////////
  //  Pattern Matching
  ///////////////////////////////////////////////////////

  /** For Pattern a(_, "x + y + h"), where a is a common synonym for all assignment statements.
  *   Gets list of statements with exact pattern match on right hand side, and any variable on left hand side.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  LIST_OF_STMT_NUMS getAllAssignStmtByExactPattern(std::vector<std::string> t_patternTokens);

  /** For Pattern a(_, _"x + y + h"_), where a is a common synonym for all assignment statements.
  *   Gets list of statements with subtree pattern match on right hand side, and any variable on left hand side.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  LIST_OF_STMT_NUMS getAllAssignStmtBySubtreePattern(std::vector<std::string> t_patternTokens);

  /** For Pattern a("x", _""_), where a is a common synonym for all assignment statements.
  *   Gets list of statements with any expression on right hand side, and given variable on left hand side.
  *   @param t_varName name of variable on left hand side
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  LIST_OF_STMT_NUMS getAllAssignStmtByVar(std::string t_varName);

  /** For Pattern a("x", "y + x"), where a is a common synonym for all assignment statements.
  *   Gets list of statements with given variable name on left hand side, and exact pattern match on right hand side.
  *   @param t_varName name of the variable modified in assignment statement
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  LIST_OF_STMT_NUMS getAllAssignStmtByVarAndExactPattern(std::string t_varName, std::vector<std::string> t_patternTokens);

  /** For Pattern a("x", _"y + x"_), where a is a common synonym for all assignment statements.
  *   Gets list of statements with given variable name on left hand side, and subtree pattern match on right hand side.
  *   @param t_varName name of the variable modified in assignment statement
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return list of statement numbers with match (will be empty list if there is none)
  *   @author jazlyn
  */
  LIST_OF_STMT_NUMS getAllAssignStmtByVarAndSubtreePattern(std::string t_varName, std::vector<std::string> t_patternTokens);

  /** For Pattern a(v, "x + y + h"), where v is a common synonym for all variables.
  *   Gets map of statements with exact pattern match on right hand side, and any variable on left hand side.
  *   Map will be returned with statement number as key, and variable name as value.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return map of statement numbers to their respective variable names (will be empty if none)
  *   @author jazlyn
  */
  MAP_OF_STMT_NUM_TO_VAR_INDEX getAllAssignStmtWithVarByExactPattern(std::vector<std::string> t_patternTokens);

  /** For Pattern a(v, _"x + y + h"_), where v is a common synonym for all variables.
  *   Gets map of statements with subtree pattern match on right hand side, and any variable on left hand side.
  *   Map will be returned with statement number as key, and variable name as value.
  *   @param t_pattern pattern to be matched, in tokens (having whitespaces will not affect result)
  *   @return map of statement numbers to their respective variable names (will be empty if none)
  *   @author jazlyn
  */
  MAP_OF_STMT_NUM_TO_VAR_INDEX getAllAssignStmtWithVarBySubtreePattern(std::vector<std::string> t_patternTokens);

  /** For Pattern w("x", _), where w is a common synonym for all while statements.
  *   Gets list of while statements that uses a given variable (in the while statement itself, not nested statements).
  *   @param t_varName variable name
  *   @return list of statement numbers
  */
  LIST_OF_STMT_NUMS getWhileStmtByVar(STRING t_varName);

  /** For Pattern w(v, _), where w is a common synonym for all while statements.
  *   Gets map of while statements with the variable name used in each while statement.
  *   Map will be returned with statement number as key, and variable name as value.
  *   @return map of statement numbers to their respective variable names (will be empty if none)
  */
  MAP_OF_STMT_NUM_TO_VAR_INDEX getAllWhileStmtsWithVar();

  /** For Pattern w(_,  _), where w is a common synonym for all while statements.
  *   Gets list of all while statements.
  *   @return list of statement numbers
  */
  LIST_OF_STMT_NUMS getAllWhileStmts();

  /** For Pattern i("x", _), where i is a common synonym for all if statements.
  *   Gets list of if statements that uses a given variable (in the if statement itself, not nested statements).
  *   @param t_varName variable name
  *   @return list of statement numbers
  */
  LIST_OF_STMT_NUMS getIfStmtByVar(STRING t_varName);

  /** For Pattern i(v, _, _), where i is a common synonym for all if statements.
  *   Gets map of if statements with the variable name used in each if statement.
  *   Map will be returned with statement number as key, and variable name as value.
  *   @return map of statement numbers to their respective variable names (will be empty if none)
  */
  MAP_OF_STMT_NUM_TO_VAR_INDEX getAllIfStmtsWithVar();

  /** For Pattern i(_,  _, _), where i is a common synonym for all if statements.
  *   Gets list of all if statements.
  *   @return list of statement numbers
  */
  LIST_OF_STMT_NUMS getAllIfStmts();

  ///////////////////////////////////////////////////////
  //  CallsTable methods
  ///////////////////////////////////////////////////////
  CallsTable* getCallsTable();

  /**
  * Check if calls(p1, p2) is true.
  * @param t_proc1 a procedure name
  * @param t_proc2 a procedure name
  * @return true if calls(p1, p2) is true.
  */
  BOOLEAN isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);

  /**
  * Check if calls*(p1, p2) is true.
  * @param t_proc1 a procedure name
  * @param t_proc2 a procedure name
  * @return true if calls*(p1, p2) is true.
  */
  BOOLEAN isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2);

  /**
  * Get the list of procedures that calls proc2.
  * i.e. calls(p, p2) where p2 is a known procedure
  * @param t_proc2 the procedure being called
  * @return the list of procedures that call t_proc2.
  */
  LIST_OF_PROC_INDICES getCallsByIdx(PROC_INDEX t_proc2Idx);

  /**
  * Get the list of procedures that are called by proc1.
  * i.e. calls(p1, p) where p1 is a known procedure
  * @param t_proc1 the procedure that calls other procedures
  * @return the list of procedures that are called by t_proc1.
  */
  LIST_OF_PROC_INDICES getCalledByByIdx(PROC_INDEX t_proc1Idx);

  /**
  * Get the list of procedures that directly/indirectly calls proc2.
  * i.e. calls*(p, p2) where p2 is a known procedure
  * @param t_proc2 the procedure being called directly/indirectly
  * @return the list of procedures that call* t_proc2.
  */
  LIST_OF_PROC_INDICES getCallsStarByIdx(PROC_INDEX t_proc2Idx);

  /**
  * Get the list of procedures that are directly/indirectly called by proc1.
  * i.e. calls*(p1, p) where p1 is a known procedure
  * @param t_proc1 the procedure that directly/indirectly calls other procedures
  * @return the list of procedures that are called by t_proc1.
  */
  LIST_OF_PROC_INDICES getCalledByStarByIdx(PROC_INDEX t_proc1Idx);

  /**
  * Get the map of calls relationship i.e. for calls(p1, p2) where p1, p2 are synonyms.
  * @return the map that contains all calls relationship.
  */
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES getAllCallsByIdx();

  /**
  * Get the map of calls* relationship i.e. for calls*(p1, p2) where p1, p2 are synonyms.
  * @return the map that contains all calls* relationship.
  */
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES getAllCallsStarByIdx();

  /**
  * Get the list of procedures that calls any other procedures i.e. calls(p1, _).
  * @return the list of procedures.
  */
  LIST_OF_PROC_INDICES getCallsAnythingByIdx();

  /**
  * Get the list of procedures that call* any other procedures i.e. calls*(p1, _).
  * @return the list of procedures.
  */
  LIST_OF_PROC_INDICES getCallsStarAnythingByIdx();

  /**
  * Get the list of procedures that called by any other procedures i.e. calls(_, p2).
  * @return the list of procedures.
  */
  LIST_OF_PROC_NAMES getCalledByAnything();
  LIST_OF_PROC_INDICES getCalledByAnythingByIdx();

  /**
  * Get the list of procedures that called by* any other procedures i.e. calls*(_, p2).
  * @return the list of procedures.
  */
  LIST_OF_PROC_INDICES getCalledByStarAnythingByIdx();

  /**
  * Check if there exists any calls relationship in the PKB.
  * @return true if there exists.
  */
  BOOLEAN hasCallsRelationship();

  /**
  * Checks if there exist any procedures that are called by proc1.
  * i.e. calls(p1, _) where p1 is a known procedure
  * @param t_proc1 the procedure that calls other procedures
  * @return true if there exists.
  */
  BOOLEAN isCallsAnything(PROC_NAME t_proc1);

  /**
  * Checks if there exist any procedures that calls proc2.
  * i.e. calls(_, p2) where p2 is a known procedure
  * @param t_proc2 the procedure that are called by other procedures
  * @return true if there exists
  */
  BOOLEAN isCalledByAnything(PROC_NAME t_proc2);

  /**
  * Method that returns the procedure name given the stmt# of the call statement.
  * @param t_lineNum the stmt#
  * @return the procedure being called in the stmt#.
  */
  PROC_NAME getProcNameFromCallStmtNum(STMT_NUM t_lineNum);

  /**
  * Get the list of stmt# that calls a particular procedure.
  * @param t_procName the procedure being called.
  * @return the list of stmt#.
  */
  LIST_OF_STMT_NUMS getStmtNumsFromProcName(PROC_NAME t_procName);

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
  LIST_OF_VAR_INDICES getModifiesPVarIndicesWithProcIdx(const PROC_NAME& t_procName);
  /**
  * Returns the list of procedure names that are modified by the variable.
  * Used in the query evaluator for Modifies(p, "x").
  *
  * @param t_varName the variable name that is checked.
  */
  LIST_OF_PROC_NAMES getModifiesPProcNamesWithVarIdx(const VAR_NAME& t_varName);
  LIST_OF_PROC_INDICES getModifiesPProcIndicesWithVarIdx(const VAR_NAME& t_varName);
  /**
  * Returns a results of a set of procedures mapped to a list of variables that they modifies.
  * Used in the query evaluator for Modifies(p, x);
  *
  */
  MAP_OF_PROC_TO_VAR getModifiesPAllProcToVar();
  MAP_OF_PROC_INDEX_TO_VAR_INDEX getModifiesPAllProcToVarByIdx();
  /**
  * Returns a list of procedures that modifies something.
  * Used in the query evaluator for  Modifies(p, _)
  *
  */
  LIST_OF_PROC_NAMES getModifiesPAllProcNames();
  LIST_OF_PROC_INDICES getModifiesPAllProcIndices();
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
  LIST_OF_VAR_INDICES getUsesPVarIndicesWithProcIdx(const PROC_NAME& t_procName);

  /**
  * Returns the list of procedure names that are used by the variable.
  * Used in the query evaluator for Uses(p, "x").
  *
  * @param t_varName the variable name that is checked.
  */
  LIST_OF_PROC_NAMES getUsesPProcNamesWithVarIdx(const VAR_NAME& t_varName);
  LIST_OF_PROC_INDICES getUsesPProcIndicesWithVarIdx(const VAR_NAME& t_varName);
  /**
  * Returns a results of a set of procedures mapped to a list of variables that they uses.
  * Used in the query evaluator for Uses(p, x);
  *
  */
  MAP_OF_PROC_TO_VAR getUsesPAllProcToVar();
  MAP_OF_PROC_INDEX_TO_VAR_INDEX getUsesPAllProcToVarByIdx();
  /**
  * Returns a list of procedures that uses something.
  * Used in the query evaluator for  Uses(p, _)
  *
  */
  LIST_OF_PROC_NAMES getUsesPAllProcNames();
  LIST_OF_PROC_INDICES getUsesPAllProcIndices();

  ///////////////////////////////////////////////////////
  //  UsesTable methods
  ///////////////////////////////////////////////////////
  UsesTable* getUsesTable();

  /**
  * Establish the uses(lineNum, varName) relationship.
  * @param t_varName the variable being used
  * @param t_lineNum the stmt#
  * @param t_varIdx the index of the t_varName
  */
  void insertUsesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum, VAR_INDEX t_varIdx);

  /**
  * Checks if uses(lineNum, varName) is true.
  * @param t_lineNum the stmt#
  * @param t_varName the variable being used
  * @return true if the relationship holds.
  */
  BOOLEAN isUses(STMT_NUM t_lineNum, VAR_NAME t_varName);

  /**
  * Get the list of variables that are being used in stmt# lineNum.
  * @param t_lineNum the stmt#
  * @return the list of variables.
  */
  LIST_OF_VAR_INDICES getUsesByIdx(STMT_NUM t_lineNum);

  /**
  * Get the list of stmt# that use the variable varName.
  * @param t_varName the variable
  * @return the list of stmt#.
  */
  LIST_OF_STMT_NUMS getStmtUses(VAR_NAME t_varName);

  /**
  * Get the map of all uses relationship i.e. for uses(s, v) where s and v are synonyms.
  * @return the map that contains all uses relationship.
  */
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS getAllStmtUsesByIdx();


  /**
  * Checks if the stmt# uses any variables.
  * i.e. uses(2, _)
  * @param t_lineNum the stmt#
  * @return true if uses(2, _) hold.
  */
  BOOLEAN isUsesAnything(STMT_NUM t_lineNum);

  /**
  * Get the list of stmt# that use any variables i.e. uses(s, _).
  * @return the list of stmt#
  */
  LIST_OF_STMT_NUMS getStmtUsesAnything();

  ///////////////////////////////////////////////////////
  //  ModifiesTable methods
  ///////////////////////////////////////////////////////
  ModifiesTable* getModifiesTable();

  /**
  * Establish the modifies(lineNum, varName) relationship.
  * @param t_varName the variable being modified
  * @param t_lineNum the stmt#
  * @param t_varIdx the index of the t_varName
  */
  void insertModifiesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum, VAR_INDEX t_varIdx);

  /**
  * Checks if modifies(lineNum, varName) is true.
  * @param t_lineNum the stmt#
  * @param t_varName the variable being modified
  * @return true if the relationship holds.
  */
  BOOLEAN isModifies(STMT_NUM t_lineNum, VAR_NAME t_varName);

  /**
  * Get the list of variables that are being modified in stmt# lineNum.
  * @param t_lineNum the stmt#
  * @return the list of variables.
  */
  LIST_OF_VAR_NAMES getModifies(STMT_NUM t_lineNum);
  LIST_OF_VAR_INDICES getModifiesByIdx(STMT_NUM t_lineNum);

  /**
  * Get the list of stmt# that modifies the variable varName.
  * @param t_varName the variable
  * @return the list of stmt#.
  */
  LIST_OF_STMT_NUMS getStmtModifies(VAR_NAME t_varName);

  /**
  * Get the map of all modifies relationship i.e. for modifies(s, v) where s and v are synonyms.
  * @return the map that contains all uses relationship.
  */
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS getAllStmtModifiesByIdx();

  /**
  * Checks if the stmt# modifies any variables.
  * i.e. modifies(2, _)
  * @param t_lineNum the stmt#
  * @return true if modifies(2, _) hold.
  */
  BOOLEAN isModifiesAnything(STMT_NUM t_lineNum);

  /**
  * Get the list of stmt# that modifies any variables i.e. modifies(s, _).
  * @return the list of stmt#
  */
  LIST_OF_STMT_NUMS getStmtModifiesAnything();

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
  LIST_OF_PROG_LINES getLinesAfter(PROG_LINE t_line);

  /** For Next(l, line) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed directly before given line.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getLinesBefore(PROG_LINE t_line);

  /** For Next*(line, l) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed after given line, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getAllLinesAfter(PROG_LINE t_line);

  /** For Next*(l, line) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed before given line, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getAllLinesBefore(PROG_LINE t_line);

  /** For Next(l1, l2) where l1, l2 is a common synonym for all lines.
  *   Gets map of all lines, each with a corresponding list of lines that can be executed directly after it.
  *   @return map of <program line number, list of lines executed after it>
  */
  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES getAllNext();

  /** For Next*(l1, l2) where l1, l2 is a common synonym for all lines.
  *   Gets map of all lines, each with a corresponding list of lines that can be executed after it, either directly or in some execution sequence.
  *   @return map of <program line number, list of lines executed after it>
  */
  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES getAllNextStar();

  /** For Next(_, l) and Next*(_, l) where l is a common synonym for all lines.
  *   Gets list of all lines that can be executed after any particular line.
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getAllLinesAfterAnyLine();

  /** For Next(l, _) and Next*(l, _) where l is a common synonym for all lines.
  *   Gets list of all lines that can be executed before any particular line.
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getAllLinesBeforeAnyLine();

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

  ///////////////////////////////////////////////////////
  //  Affects Table
  ///////////////////////////////////////////////////////
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllAffects(); // affects(a1,a2)
  LIST_OF_AFFECTS_STMTS getAffects(STMT_NUM t_usesLine); // affects(a,12)
  LIST_OF_AFFECTS_STMTS getAffectedBy(STMT_NUM t_modifiesLine); // affects(2,a)
  BOOLEAN isAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine); // affects(1,12)
  BOOLEAN hasAffectsRelationship(); // affects(_,_)
  LIST_OF_AFFECTS_STMTS getAffectsAnything();  // affects(a,_)
  LIST_OF_AFFECTS_STMTS getAffectedByAnything(); // affects(_,a)
  BOOLEAN isAffectsAnything(STMT_NUM t_modifiesLine); // affects(1,_)
  BOOLEAN isAffectedByAnything(STMT_NUM t_usesLines); // affects(_,12)

  ///////////////////////////////////////////////////////
  //  Affects* Extractor
  ///////////////////////////////////////////////////////
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllAffectsStar(); // affects(a1,a2)
  LIST_OF_AFFECTS_STMTS getAffectsStar(STMT_NUM t_modifiesLine); // affects(2,a)
  LIST_OF_AFFECTS_STMTS getAffectedByStar(STMT_NUM t_usesLine); // affects(a,12)
  BOOLEAN isAffectsStar(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine); // affects(1,12)
  BOOLEAN hasAffectsRelationshipStar(); // affects(_,_)
  LIST_OF_AFFECTS_STMTS getAffectsAnythingStar();  // affects(a,_)
  LIST_OF_AFFECTS_STMTS getAffectedByAnythingStar(); // affects(_,a)
  BOOLEAN isAffectsAnythingStar(STMT_NUM t_modifiesLine); // affects(1,_)
  BOOLEAN isAffectedByAnythingStar(STMT_NUM t_usesLines); // affects(_,12)

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
  DesignExtractor *m_designExtractor;
};
