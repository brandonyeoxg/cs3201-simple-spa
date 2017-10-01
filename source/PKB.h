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
#include "StatementTable.h"
#include "Grammar.h"
#include "ConstantTable.h"
#include "GlobalTypeDef.h"
#include "PkbWriteOnly.h"
#include "PkbReadOnly.h"
#include "ModifiesP.h"
#include "UsesP.h"
#include "CallsTable.h"

class TNode;

class PKB: public PkbWriteOnly, public PkbReadOnly {

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
  bool insertFollowsRelation(std::list<STMT_NUM> t_stmtInStmtList, int t_curLineNum);

  /**
  * Inserts a parent relation in the PKB.
  * @param t_node reference to the StmtLst that this statement belongs to.
  * @param t_curLineNum the current line number.
  * @return true if the table is successfully added.
  */
  bool insertParentRelation(std::list<STMT_NUM> t_nestedStmtInStmtList, int t_curLineNum);

  /**
  * Inserts a variable that has been modified.
  * @param t_varName name of the variable being modified.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  VariableNode* insertModifiesVariable(std::string t_varName, int t_curLineNum,
    std::list<STMT_NUM> t_nestedStmtLines);

  /**
  * Inserts a variable that has been used.
  * @param t_varName name of the variable that is used.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  VariableNode* insertUsesVariable(std::string t_varName, int m_curLineNum, std::list<STMT_NUM> t_nestedStmtLines);

  /**
  * Inserts a variable that has been modified.
  * @param t_varName name of the variable being modified.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  void insertModifiesVariableNew(std::string t_varName, int t_curLineNum,
    std::list<STMT_NUM> t_nestedStmtLines);

  /**
  * Inserts a variable that has been used.
  * @param t_varName name of the variable that is used.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  void insertUsesVariableNew(std::string t_varName, int m_curLineNum, std::list<STMT_NUM> t_nestedStmtLines);

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
  * @param t_parentNode reference to the parent node that the assignment statement belongs to.
  * @param t_varNode reference to the variable node that is at this assignment statement.
  * @param t_exprNode reference to the expr node of the assignment statement.
  * @param t_curLineNum the current line that this assignment is at.
  */
  void insertAssignStmt(VariableNode* t_varNode, TNode* t_exprNode, int t_curLineNum);

  /**
  * Inserts an assignment statement into the PKB
  * @param t_lineNum the line number that the assignment statement is at.
  * @param t_tokens tokenised expression for the right side of the "=" operator
  */
  void insertAssignStmt(STMT_NUM t_lineNum, const LIST_OF_TOKENS& t_tokens);

  /**
  * Inserts a call statement into the PKB
  */
  void insertCallStmt(STMT_NUM t_lineNum);

  /**
  * Inserts a while statement into the PKB.
  * @param t_parentNode reference to the parent node that this while loop belongs to.
  * @param t_varaibleNode reference to the variable node that this while loop contains.
  * @param t_curLineNum the current line number that this while statement is at.
  * @return a reference of the while node.
  */
  STMT_NUM insertWhileStmt(std::string varName, std::list<STMT_NUM> m_nestedStmtLineNum, int t_curLineNum);

  /**
  * Inserts a if statement into the PKB.
  * @param t_parentNode reference to the parent node that this while loop belongs to.
  * @param t_varaibleNode reference to the variable node that this while loop contains.
  * @param t_curLineNum the current line number that this while statement is at.
  * @return a reference of the while node.
  */
  STMT_NUM insertIfStmt(std::string t_varName, std::list<STMT_NUM> t_nestedStmtLinNum, int t_curLineNum);

  /**
  * Inserts a constant into the PKB.
  * @param t_constVal the constant to be added in string form.
  * @param t_curLineNum the current line of the constant.
  * @return a reference to the constant node.
  */
  ConstantNode* insertConstant(std::string t_constVal, int t_curLineNum);

  /**
  * Inserts a constant into the PKB.
  * @param t_constVal the constant to be added in string form.
  * @param t_curLineNum the current line of the constant.
  * @return a reference to the constant node.
  */
  void insertConstant(CONSTANT_TERM t_constVal);

  /**
  * Returns a plus operator.
  * @param t_left the node to the left.
  * @param t_right the node to the right.
  * @param t_curLineNum the current line number.
  * @return a reference to the plus node subtree.
  */
  PlusNode* insertPlusOp(TNode* t_left, TNode* t_right, int t_curLineNum);
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
  bool isFollows(STMT_NUM t_s1, STMT_NUM t_s2);

  /**
  * Method that checks if follows*(s1, s2) holds.
  * Checks if s2 exists within the vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  bool isFollowsStar(STMT_NUM t_s1, STMT_NUM t_s2);

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
  std::unordered_map<STMT_NUM, STMT_NUM> getAllFollows();

  /**
  * Method that returns the entire map of line numbers that satisfy the follow* relationship.
  * @return the entire map that keep tracks of the follow relationship.
  */
  std::unordered_map<STMT_NUM, LIST_OF_STMT_NUMS> getAllFollowsStar();

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
  bool hasFollowRelationship();

  /**
  * Method that checks if follows(_, s2) and follows*(_, s2) holds, where s2 is a statement number.
  * @return true if s2 exists in the allFollows map, return false if otherwise.
  */
  bool isFollowsAnything(STMT_NUM t_s2);

  /**
  * Method that checks if follows(2, _) and follows*(2, _) holds, where s2 is a statement number.
  * @return true if s2 exists in the allFollows map, return false if otherwise.
  */
  bool isFollowedByAnything(STMT_NUM t_s1);

  ///////////////////////////////////////////////////////
  //  ParentTable methods
  ///////////////////////////////////////////////////////
  ParentTable* getParentTable();
  void populateParentStarMap();
  void populateParentedByStarMap();
  bool isParent(STMT_NUM t_s1, STMT_NUM t_s2);
  bool isParentStar(STMT_NUM t_s1, STMT_NUM t_s2);
  STMT_NUM getParentOf(STMT_NUM t_s2);
  LIST_OF_STMT_NUMS getChildrenOf(STMT_NUM t_s1);
  LIST_OF_STMT_NUMS getParentStarOf(STMT_NUM t_s2);
  LIST_OF_STMT_NUMS getChildrenStarOf(STMT_NUM t_s1);
  std::unordered_map<STMT_NUM, LIST_OF_STMT_NUMS> getAllParents();
  std::unordered_map<STMT_NUM, LIST_OF_STMT_NUMS> getAllParentsStar();
  LIST_OF_STMT_NUMS getChildrenOfAnything();
  LIST_OF_STMT_NUMS getParentOfAnything();
  LIST_OF_STMT_NUMS getChildrenStarOfAnything();
  LIST_OF_STMT_NUMS getParentStarOfAnything();
  bool hasParentRelationship();
  bool hasParentStarRelationship();
  bool isChildrenOfAnything(STMT_NUM t_s2);
  bool isParentOfAnything(STMT_NUM t_s1);
  bool isChildrenOfStarAnything(STMT_NUM t_s2);
  bool isParentOfStarAnything(STMT_NUM t_s1);

  //////////////////////////////////////////////////////////
  //  statementTypeTable and typeOfStatementTable Methods
  //////////////////////////////////////////////////////////

  std::unordered_map<STMT_NUM, queryType::GType> getTypeOfStatementTable();
  bool insertTypeOfStatementTable(STMT_NUM t_lineNum, queryType::GType t_type);
  std::unordered_map<queryType::GType, LIST_OF_STMT_NUMS>  getStatementTypeTable();
  bool insertStatementTypeTable(queryType::GType t_type, STMT_NUM t_lineNum);

  ///////////////////////////////////////////////////////
  //  VarTable methods
  ///////////////////////////////////////////////////////
  VarTable* getVarTable();
  STMT_NUM insertUsesForStmt(std::string t_varName, STMT_NUM t_lineNum);
  STMT_NUM insertModifiesForStmt(std::string t_varName, STMT_NUM t_lineNum);
  bool isModifies(STMT_NUM t_lineNum, std::string t_varName);
  bool isUses(STMT_NUM t_lineNum, std::string t_varName);
  LIST_OF_VAR_NAMES getModifies(STMT_NUM t_lineNum);
  LIST_OF_VAR_NAMES getUses(STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getStmtModifies(std::string t_varName);
  LIST_OF_STMT_NUMS getStmtUses(std::string t_varName);
  std::unordered_map<std::string, LIST_OF_STMT_NUMS> getAllStmtModifies();
  std::unordered_map<std::string, LIST_OF_STMT_NUMS> getAllStmtUses();
  STMT_NUM getIndexOfVar(std::string t_varName);
  std::string getVarNameFromIndex(STMT_NUM t_index);
  bool isModifiesAnything(STMT_NUM t_lineNum);
  bool isUsesAnything(STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getStmtModifiesAnything();
  LIST_OF_STMT_NUMS getStmtUsesAnything();
  LIST_OF_VAR_NAMES getAllVariables();

  ///////////////////////////////////////////////////////
  //  AssignTable
  ///////////////////////////////////////////////////////
  AssignTable* getAssignTable();
  /*
  * Returns all assignment statements number that modifies the variable name.
  * @param t_varName the name of the variable.
  */
  std::list<STMT_NUM> getAllAssignStmtListByVar(VAR_NAME& t_varName);

  /*
  * Returns all assignment statements.
  */
  LIST_OF_STMT_NUMS getAllAssignStmtList();

  /*
  * Returns all assignment statements in a representation.
  * The representation is a variable mapped to all statement number under that variable.
  */
  std::unordered_map<std::string, std::list<STMT_NUM>> getAllVarNameWithAssignStmt();

  /*
  * Returns all assignment statements in a representation.
  * The repsentation is a statement number mapped to the variable in that statement number.
  */
  std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVarName();

  /*
  * Populates the rest of the representation in the assignment table.
  * This method is to be called in the design extractor.
  */
  void populateAssignTableAbstractions();

  ///////////////////////////////////////////////////////
  //  ProcTable
  ///////////////////////////////////////////////////////
  ProcTable* getProcTable();

  ///////////////////////////////////////////////////////
  //  ConstantTable methods
  ///////////////////////////////////////////////////////
  std::list<std::string> getAllConstants();

  ///////////////////////////////////////////////////////
  //  Pattern Matching
  ///////////////////////////////////////////////////////

  /**
  * Pattern a("x", "y") or Pattern a("x", _"y"_).
  * Gets list of statements with exact pattern match on right hand side, and a given variable name on the left hand side.
  * @param t_varName variable name to be matched.
  * @param t_pattern pattern to be matched (having whitespaces will not affect result) i.e. "x + y + h", "x"
  * @param t_isExact if it is true a("x", "y") else a("x", _"y"_). *Subject to change in later versions*.
  * @return list of statement numbers with match
  */
  std::list<STMT_NUM> getAssignStmtByVarPattern(std::string t_varName, std::string pattern, bool t_isExact); /*< Pattern a("x", "y") or Pattern a("x", _"y"_)*/

  /**
  * Pattern a(v,"y") or Pattern a(v, _"y"_).
  * Gets a statement number mapping to a variable.
  * @param t_pattern pattern to be matched (having whitespaces will not affect result) i.e. "x + y + h", "x"
  * @param t_isExact if it is true a("x", "y") else a("x", _"y"_). *Subject to change in later versions*.
  * @return list of statement numbers with match
  */
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

  ///////////////////////////////////////////////////////
  //  CallsTable methods
  ///////////////////////////////////////////////////////
  bool insertCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);
  bool isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);
  bool isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2);
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
  bool hasCallsRelationship();  //calls(_, _)
  bool isCallsAnything(PROC_NAME t_proc1);
  bool isCalledByAnything(PROC_NAME t_proc2);
  void populateCallsStarMaps();

  ///////////////////////////////////////////////////////
  //  ModifiesP methods
  ///////////////////////////////////////////////////////
  bool isModifiesP(const PROC_NAME& t_procName, const VAR_NAME& t_varName); /*< Modifies("First", "x") */
  bool isModifiesInProc(const PROC_NAME& t_procName); /*< Modifies("First", _) */
  LIST_OF_VAR_NAMES getModifiesPVarNamesWithProcIdx(const PROC_NAME& t_procName); /*< Modifies("First", x) */
  LIST_OF_PROC_NAMES getModifiesPProcNamesWithVarIdx(const VAR_NAME& t_varName); /*< Modifies(p, "x") */
  MAP_OF_PROC_TO_VAR& getModifiesPAllProcToVar(); /*< Modifies(p, x) */
  LIST_OF_PROC_NAMES& getModifiesPAllProcNames(); /*< Modifies(p, _) */

  ///////////////////////////////////////////////////////
  //  UsesP methods
  ///////////////////////////////////////////////////////
  bool isUsesP(const PROC_NAME& t_procName, const VAR_NAME& t_varName); /*< Modifies("First", "x") */
  bool isUsesInProc(const PROC_NAME& t_procName); /*< Modifies("First", _) */
  LIST_OF_VAR_NAMES getUsesPVarNamesWithProcIdx(const PROC_NAME& t_procName); /*< Modifies("First", x) */
  LIST_OF_PROC_NAMES getUsesPProcNamesWithVarIdx(const VAR_NAME& t_varName); /*< Modifies(p, "x") */
  MAP_OF_PROC_TO_VAR& getUsesPAllProcToVar(); /*< Modifies(p, x) */
  LIST_OF_PROC_NAMES& getUsesPAllProcNames(); /*< Modifies(p, _) */

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

  ASTBuilder m_builder;
};
