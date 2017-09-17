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
#include "PKB_API.h"



class TNode;

class PKB: public PKB_API {
  
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
  //  PKB building methods
  ///////////////////////////////////////////////////////

  /**
  * Inserts a procedure into the PKB. AST is built from this procedure node.
  * @param t_procName name of the procedure.
  * @return a reference to the StmtListNode created from inserting the procedure.
  */
  StmtListNode* insertProcedure(std::string& t_procName);

  /**
  * Inserts a follows relation in the PKB.
  * @param t_node reference to the StmtLst that this statement belongs to.
  * @param t_curLineNum the current line number.
  * @return true if the table is successfully added.
  */
  bool insertFollowsRelation(TNode* t_node, int t_curLineNum);

  /**
  * Inserts a variable that has been modified.
  * @param t_varName name of the variable being modified.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  VariableNode* insertModifiedVariable(std::string t_varName, int t_curLineNum,
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
  * Inserts an assignment statement into the PKB
  * @param t_parentNode reference to the parent node that the assignment statement belongs to.
  * @param t_varNode reference to the variable node that is at this assignment statement.
  * @param t_exprNode reference to the expr node of the assignment statement.
  * @param t_curLineNum the current line that this assignment is at.
  */
  void insertAssignStmt(TNode* t_parentNode, VariableNode* t_varNode, TNode* t_exprNode, int t_curLineNum);
  
  /**
  * Inserts a while statement into the PKB.
  * @param t_parentNode reference to the parent node that this while loop belongs to.
  * @param t_varaibleNode reference to the variable node that this while loop contains.
  * @param t_curLineNum the current line number that this while statement is at.
  * @return a reference of the while node.
  */
  StmtListNode* insertWhileStmt(TNode* t_parentNode, VariableNode* t_varNode, int t_curLineNum);
  
  /**
  * Inserts a constant into the PKB.
  * @param t_constVal the constant to be added in string form.
  * @param t_curLineNum the current line of the constant.
  * @return a reference to the constant node.
  */
  ConstantNode* insertConstant(std::string t_constVal, int t_curLineNum);
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

  /*
  * Inserts the procedure into the AST.
  * @param t_node reference to the procedure node to be added into the AST
  * @return the procedure index in the procedure table.
  */
  PROC_INDEX insertProcToAST(ProcedureNode* t_node);

  /*
  * Returns the procedure node from the procedure table.
  * @param t_index the index of the procedure in the procedure index.
  * @return the reference of the procedure node.
  */
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

  /*
  * Inserts an assign statement into the table.
  * @param t_index the index of the variable.
  * @param t_node reference to an assign node in the AST.
  * @return the index to the assign table.
  */
  VAR_INDEX insertAssignRelation(const VAR_INDEX& t_index, AssignNode* t_node);
  
  /*
  * Returns all assignment statements number that modifies the variable name.
  * @param t_varName the name of the variable.
  */
  std::list<STMT_NUM> getAllAssignStmtListByVar(VAR_NAME& t_varName);

  /*
  * Returns all assignment statements.
  */
  std::list<STMT_NUM> getAllAssignStmtList();

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

  static const int PROC_LINE_NUM = 0;
};