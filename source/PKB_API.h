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

/** API for PKB
*/

class PKB_API {
public:


  ///////////////////////////////////////////////////////
  //  FollowTable methods 
  ///////////////////////////////////////////////////////



  /**
  * Method that inserts to FollowTable the line number (s2) to the unordered map of vectors containing line number s1 as key.
  * Returns false if current s1, s2 pair already exists in the map.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return The status of the insertion.
  */
  virtual bool insertFollows(int t_s1, int t_s2) = 0;

  /**
  * Method that checks if follows(s1, s2) holds.
  * Checks if s2 exists as the first element in vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isFollows(int t_s1, int t_s2) = 0;

  /**
  * Method that checks if follows*(s1, s2) holds.
  * Checks if s2 exists within the vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isFollowsStar(int t_s1, int t_s2) = 0;

  /**
  * Method that returns the line number that follows(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param s1 an integer argument.
  * @return the line number that line s1 follows.
  */
  virtual int getFollows(int t_s1) = 0;

  /**
  * Method that returns the line number that follows(s, s2) holds, where s is a variable and s2 is a known line number.
  * @param s2 an integer argument.
  * @return the line number that is followed by line s2.
  */
  virtual int getFollowedBy(int t_s2) = 0;

  /**
  * Method that returns the list of line numbers that follows*(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param s1 an integer argument.
  * @return the vector of line numbers that line s1 follows*.
  */
  virtual std::vector<int> getFollowsStar(int t_s1) = 0;

  /**
  * Method that returns the list of line numbers that follows*(s, s2) holds, where s is a variable and s2 is a known line number.
  * Returns the vector of keys that the vector mapped to the key contains s2.
  * @param s1 an integer argument.
  * @return the vector of line numbers that are followedBy* s2.
  */
  virtual std::vector<int> getFollowedByStar(int t_s2) = 0;

  /**
  * Method that returns the entire map of line numbers that satisfy the follow* relationship.
  * @return the entire map that keep tracks of the follow relationship.
  */
  virtual std::unordered_map<int, int> getAllFollows() = 0;

  /**
  * Method that returns the entire map of line numbers that satisfy the follow* relationship.
  * @return the entire map that keep tracks of the follow relationship.
  */
  virtual std::unordered_map<int, std::vector<int>> getAllFollowsStar() = 0;

  /**
  * Method that returns the list of line numbers that is followed by another statement.
  * @return the vector of keys within the followTable.
  */
  virtual std::vector<int> getFollowedByAnything() = 0;

  /**
  * Method that returns the list of line numbers that follows a statement.
  * @return the vector of keys within the followTable.
  */
  virtual std::vector<int> getFollowsAnything() = 0;

  /**
  * Method that checks if follows(_, _) or follows*(_, _) holds.
  * @return true if the size of the followTable is more than zero, return false if otherwise.
  */
  virtual bool hasFollowRelationship() = 0;

  /**
  * Method that checks if follows(_, t_s2) and follows*(_, t_s2) holds, where t_s2 is a statement number.
  * @return true if t_s2 exists in the allFollows map, return false if otherwise.
  */
  virtual bool isFollowsAnything(int t_s2) = 0;

  /**
  * Method that checks if follows(2, _) and follows*(2, _) holds, where s2 is a statement number.
  * @return true if s2 exists in the allFollows map, return false if otherwise.
  */
  virtual bool isFollowedByAnything(int t_s1) = 0;



  ///////////////////////////////////////////////////////
  //  ParentTable methods 
  ///////////////////////////////////////////////////////



  /**
  * Method that inserts to ParentTable the line number (t_s2) to the unordered map of vectors containing line number t_s1 as key.
  * Returns false if current t_s1, t_s2 pair already exists in the map.
  * @param t_s1 an integer argument.
  * @param t_s2 an integer argument.
  * @return The status of the insertion.
  */
  virtual bool insertParent(int t_s1, int t_s2) = 0;

  /**
  * Method that populates the ParentStarMap.
  * To be called by Design Extractor after the initial parsing.
  */
  virtual void populateParentStarMap() = 0;

  /**
  * Method that populates the ParentedByStarMap.
  * To be called by Design Extractor after the initial parsing.
  */
  virtual void populateParentedByStarMap() = 0;

  /**
  * Method that checks if parent(t_s1, t_s2) holds.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isParent(int t_s1, int t_s2) = 0;

  /**
  * Method that checks if parent*(t_s1, t_s2) holds.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isParentStar(int t_s1, int t_s2) = 0;

  /**
  * Method that returns the line numuber that is the parent of t_s2.
  * For example: stmt s; parent(s, 4).
  * @param t_s2 an integer argument.
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual int getParentOf(int t_s2) = 0;

  /**
  * Method that returns the vector of line numubers that are the children of t_s1.
  * For example: stmt s; parent(4, s).
  * @param t_s1 an integer argument.
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual std::vector<int> getChildrenOf(int t_s1) = 0;

  /**
  * Method that returns the vector of line numubers that are the parent* of t_s2.
  * For example: stmt s; parent*(s, 4).
  * @param t_s2 an integer argument.
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual std::vector<int> getParentStarOf(int t_s2) = 0;

  /**
  * Method that returns the vector of line numubers that are the children* of t_s1.
  * For example: stmt s; parent*(4, s).
  * @param t_s1 an integer argument.
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual std::vector<int> getChildrenStarOf(int t_s1) = 0;

  /**
  * Method that returns the entire of parent relationship.
  * @return an unordered_map i.e. parentMap.
  */
  virtual std::unordered_map<int, std::vector<int>> getAllParents() = 0;

  /**
  * Method that returns the entire of parent* relationship.
  * @return an unordered_map i.e. parentStarMap.
  */
  virtual std::unordered_map<int, std::vector<int>> getAllParentsStar() = 0;

  /**
  * Method that returns the vector of line numubers that are the children of any line.
  * For example: stmt s2; parent(_, s2).
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual std::vector<int> getChildrenOfAnything() = 0;

  /**
  * Method that returns the vector of line numubers that are the parent of any line.
  * For example: stmt s1; parent(s1, _).
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual std::vector<int> getParentOfAnything() = 0;

  /**
  * Method that returns the vector of line numubers that are the parent of any line.
  * For example: stmt s2; parent*(_, s2).
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual std::vector<int> getChildrenStarOfAnything() = 0;

  /**
  * Method that returns the vector of line numubers that are the parent of any line.
  * For example: stmt s1; parent*(s1, _).
  * @return a vector of line numbers that satisfy the condition.
  */
  virtual std::vector<int> getParentStarOfAnything() = 0;

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
  virtual bool isChildrenOfAnything(int t_s2) = 0;

  /**
  * Method that checks if parent(t_s1, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one parent relationship with t_s1 being the parent, false if otherwise.
  */
  virtual bool isParentOfAnything(int t_s1) = 0;

  /**
  * Method that checks if parent*(_, t_s2) holds.
  * @param t_s2 an integer argument.
  * @return true if there exists at least one parent* relationship with t_s2 being the child, false if otherwise.
  */
  virtual bool isChildrenOfStarAnything(int t_s2) = 0;

  /**
  * Method that checks if parent*(t_s1, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one parent* relationship with t_s1 being the parent, false if otherwise.
  */
  virtual bool isParentOfStarAnything(int t_s1) = 0;



  //////////////////////////////////////////////////////////
  //  statementTypeTable and typeOfStatementTable Methods
  //////////////////////////////////////////////////////////

  /**
  * Method that retrieves the TypeOfStatementTable.
  * @return the unordered_map of the statement's line number mapped to the type of the statement.
  */
  virtual std::unordered_map<int, Grammar::GType> getTypeOfStatementTable() = 0;

  /**
  * Method that inserts to TypeOfStatementTable with the line number as key and type of statement as value.
  * Returns false if current key value pair already exists in the map.
  * @param s1 an integer argument.
  * @param s2 a Grammer GType argument.
  * @return The status of the insertion.
  */
  virtual bool insertTypeOfStatementTable(int t_lineNum, Grammar::GType t_type) = 0;

  /**
  * Method that retrieves the StatementTypeTable.
  * @return the unordered_map of type of the statement mapped to its line number.
  */
  virtual std::unordered_map<Grammar::GType, std::vector<int>>  getStatementTypeTable() = 0;

  /**
  * Method that inserts to StatementTypeTable with the type of statement as key and line number as value.
  * Returns false if current key value pair already exists in the map.
  * @param s1 a Grammer GType argument.
  * @param s2 an integer argument.
  * @return The status of the insertion.
  */
  virtual bool insertStatementTypeTable(Grammar::GType t_type, int t_lineNum) = 0;



  ///////////////////////////////////////////////////////
  //  VarTable methods 
  ///////////////////////////////////////////////////////

  /**
  * Method that inserts to VarTable the line number and the name of the variable being used.
  * @param t_varName a string argument.
  * @param t_lineNum an integer argument.
  * @return the index of the entry.
  */
  virtual int insertUsesForStmt(std::string t_varName, int t_lineNum) = 0;

  /**
  * Method that inserts to VarTable the line number and the name of the variable being modified.
  * @param t_varName a string argument.
  * @param t_lineNum an integer argument.
  * @return the index of the entry.
  */
  virtual int insertModifiesForStmt(std::string t_varName, int t_lineNum) = 0;

  /**
  * Method that checks if modifies(t_s1, t_varname) holds.
  * @param t_lineNum an integer argument.
  * @param s2 t_varName a string argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isModifies(int t_lineNum, std::string t_varName) = 0;

  /**
  * Method that checks if uses(t_s1, t_varname) holds.
  * @param t_lineNum an integer argument.
  * @param s2 t_varName a string argument.
  * @return true if the relationship holds, false if otherwise.
  */
  virtual bool isUses(int t_lineNum, std::string t_varName) = 0;

  /**
  * Method that returns the vector of variables that are modified in line number t_lineNum.
  * For example: modifies(2, "x").
  * @param t_lineNum an integer argument.
  * @return a vector of variables that satisfy the condition.
  */
  virtual std::vector<std::string> getModifies(int t_lineNum) = 0;

  /**
  * Method that returns the vector of variables that are used in line number t_lineNum.
  * For example: uses(2, "x").
  * @param t_lineNum an integer argument.
  * @return a vector of variables that satisfy the condition.
  */
  virtual std::vector<std::string> getUses(int t_lineNum) = 0;

  /**
  * Method that returns the vector of statement numbers that modifies variable t_varName.
  * For example: stmt s; modifies(s, "x").
  * @param t_lineNum an integer argument.
  * @return a vector of statement numbers that satisfy the condition.
  */
  virtual std::vector<int> getStmtModifies(std::string t_varName) = 0;

  /**
  * Method that returns the vector of statement numbers that uses variable t_varName.
  * For example: stmt s; uses(s, "x").
  * @param t_lineNum an integer argument.
  * @return a vector of statement numbers that satisfy the condition.
  */
  virtual std::vector<int> getStmtUses(std::string t_varName) = 0;

  /**
  * Method that returns the entire map of modifies relationship.
  * For example: stmt s, variable v; modifies(s, v).
  * @return an unordered_map that satisfy the condition.
  */
  virtual std::unordered_map<std::string, std::vector<int>> getAllStmtModifies() = 0;

  /**
  * Method that returns the entire map of uses relationship.
  * For example: stmt s, variable v; uses(s, v).
  * @return an unordered_map that satisfy the condition.
  */
  virtual std::unordered_map<std::string, std::vector<int>> getAllStmtUses() = 0;

  /**
  * Method that returns the index of the variable t_varName in VarTable.
  * @param t_varName a string argument.
  * @return the index of variable.
  */
  virtual int getIndexOfVar(std::string t_varName) = 0;

  /**
  * Method that returns the name of the variable in VarTable given its index.
  * @param t_index an integer argument.
  * @return the name of the variable.
  */
  virtual std::string getVarNameFromIndex(int t_index) = 0;

  /**
  * Method that checks if modifies(t_lineNum, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one modifies relationship with t_lineNum being the statement number, false if otherwise.
  */
  virtual bool isModifiesAnything(int t_lineNum) = 0;

  /**
  * Method that checks if uses(t_lineNum, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one uses relationship with t_lineNum being the statement number, false if otherwise.
  */
  virtual bool isUsesAnything(int t_lineNum) = 0;

  /**
  * Method that returns the vector of line numbers that involves modification of variables.
  * For example: stmt s; modifies(s, _).
  * @return a vector of statement numbers that satisfy the condition.
  */
  virtual std::vector<int> getStmtModifiesAnything() = 0;

  /**
  * Method that returns the vector of line numbers that involves use of variables.
  * For example: stmt s; uses(s, _).
  * @return a vector of statement numbers that satisfy the condition.
  */
  virtual std::vector<int> getStmtUsesAnything() = 0;

  /**
  * Method that returns the vector of variables that are stored within VarTable.
  * @return a vector of statement numbers.
  */
  virtual std::vector<std::string> getAllVariables() = 0;


  ///////////////////////////////////////////////////////
  //  ConstantTable methods 
  ///////////////////////////////////////////////////////

  /**
  * Method that inserts the constant into ConstantTable.
  * @param t_constant a string argument.
  * @return the index of t_constant in ConstantTable.
  */
  virtual int insertConstant(std::string t_constant) = 0;

  /**
  * Method that returns the list of all constants that are stored within ConstantTable.
  * @return a list of constants (strings).
  */
  virtual std::list<std::string> getAllConstants() = 0;

};