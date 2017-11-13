#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../../GlobalTypeDef.h"
#include "../../exception/InvalidArgumentException.h"

/**
* Represents the follows relations made in the program.
* The relations are stored in different representation for fast retrieval from PQL through the PKB.
*
* @author pengcheng
*/
class ParentTable {
public:
  void setChildMap(MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS &t_map);
  void setParentMap(MAP_OF_STMT_NUMS&t_map);
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS& getChildMap();
  MAP_OF_STMT_NUMS& getParentMap();

  /**
  * Method that returns the entire map of line numbers that satisfy the parent* relationship.
  * @return the unordered map that keep tracks of the parent* relationship.
  */
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS& getParentStarMap();


  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS& getParentedByStarMap();

  ParentTable();
  ~ParentTable() {};


  BOOLEAN insertParent(int t_s1, int t_s2);

  /**
  * Method that checks if parent(s1, s2) holds.
  * Checks if s2 exists as value mapped to key s1 in parentMap.
  * @param t_s1 an integer argument.
  * @param t_s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  BOOLEAN isParent(int t_s1, int t_s2);

  /**
  * Method that checks if parent*(s1, s2) holds.
  * Checks if s2 exists in vector mapped to key s1 in parentStarMap.
  * @param t_s1 an integer argument.
  * @param t_s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  BOOLEAN isParentStar(int t_s1, int t_s2);

  /**
  * Method that returns the statement number that is the parent of s2.
  * Checks if key s2 exists in parentMap.
  * @throw invalid_arguement exception if key does not exist.
  * @param t_s2 an integer argument.
  * @return a statement number.
  */
  STMT_NUM getParentOf(int t_s2);

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent(t_s1, x).
  * Returns empty vector if no relationship holds for all statement numbers.
  * @param t_s1 an integer argument.
  * @return a vector of statement numbers.
  */
  LIST_OF_STMT_NUMS getChildrenOf(int t_s1);

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent*(x, t_s2).
  * Returns empty vector if no relationship holds for all statement numbers.
  * @param t_s2 an integer argument.
  * @return a vector of statement numbers.
  */
  LIST_OF_STMT_NUMS getParentStarOf(int t_s2);

  /**
  * Method that returns a vector of statement numbers, x that satisfy the relationship parent*(t_s1, x).
  * Returns empty vector if no relationship holds for all statement numbers.
  * @param t_s1 an integer argument.
  * @return a vector of statement numbers.
  */
  LIST_OF_STMT_NUMS getChildrenStarOf(int t_s1);

  /**
  * Method that returns the entire map of line numbers that satisfy the parent relationship.
  * @return the unordered map that keep tracks of the parent relationship.
  */
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllParents();

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
  BOOLEAN isChildrenOfAnything(int t_s2);

  /**
  * Method that checks if parent(t_s1, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one parent relationship with t_s1 being the parent, false if otherwise.
  */
  BOOLEAN isParentOfAnything(int t_s1);

  /**
  * Method that checks if parent*(_, t_s2) holds.
  * @param t_s2 an integer argument.
  * @return true if there exists at least one parent* relationship with t_s2 being the child, false if otherwise.
  */
  BOOLEAN isChildrenOfStarAnything(int t_s2);

  /**
  * Method that checks if parent*(t_s1, _) holds.
  * @param t_s1 an integer argument.
  * @return true if there exists at least one parent* relationship with t_s1 being the parent, false if otherwise.
  */
  BOOLEAN isParentOfStarAnything(int t_s1);

  //Methods to be called by the DesignExtractor
  void populateParentMatrix(TOTAL_NUMBER_OF_STMTS total);
  void populateParentStarMap();
  void populateParentedByStarMap();
  void populateParentAnythingRelationships();

private:
  MAP_OF_STMT_NUMS m_parentMap; //every statement can only have 1 parent.
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_childMap;
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_parentStarMap;
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_parentedByStarMap;
  BOOLEAN_MATRIX m_parentMatrix;

  LIST_OF_STMT_NUMS m_allParents;
  LIST_OF_STMT_NUMS m_allChildren;
  LIST_OF_STMT_NUMS m_allParentStar;
  LIST_OF_STMT_NUMS m_allChildrenStar;
};
