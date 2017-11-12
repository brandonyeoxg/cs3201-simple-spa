#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <stdexcept>
#include "../../GlobalTypeDef.h"
#include "../../exception/InvalidArgumentException.h"

/** 
* Represents the follow relationship table. The table consists an unordered_map of line numbers mapped to vectors of line numbers that
track which line is followed by which lines.
* @author pengcheng
* @date 8/27/2017
*
*/
class FollowTable {
public:

  /**
  * Method that inserts the line number (s2) to the unordered map of vectors containing line number s1 as key.
  * Returns false if current s1, s2 pair already exists in the map.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return The status of the insertion.
  */
  BOOLEAN insertFollows(STMT_NUM t_s1, STMT_NUM t_s2);

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
  * Checks if (s1, s2) is true in the 2D matrix.
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
  * Method that returns the entire map of line numbers that follows(s1, s2) holds, where s1 and s2 are both variables.
  * @return the entire map that keep tracks of the follow relationship.
  */
  MAP_OF_STMT_NUMS getAllFollows();

  /**
  * Method that returns the entire map of line numbers that follows*(s1, s2) holds, where s1 and s2 are both variables.
  * @return the entire map that keep tracks of the follow relationship.
  */
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllFollowsStar();

  /**
  * Method that returns the list of line numbers that follows(s1, _) and follows*(s1, _) holds, where s1 is a variable.
  * @return the vector of keys within the followTable.
  */
  LIST_OF_STMT_NUMS getFollowedByAnything();

  /**
  * Method that returns the list of line numbers that follows (_, s2) and Follows* (_, s2) holds, where s1 is a variable.
  * @return the vector of keys within the followTable.
  */
  LIST_OF_STMT_NUMS getFollowsAnything();

  /**
  * Method that checks if follows(_, _) or follows*(_, _) holds, where s2 is a variable.
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
  * @return true if s2 exists in the allFollows map, return false if otherwise.
  */
  BOOLEAN isFollowedByAnything(STMT_NUM t_s1);

  ///////////////////////////////////////////////////////
  //  Getter and Setter methods 
  ///////////////////////////////////////////////////////

  /**
  * A setter method for followTable.
  * Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
  */
  void setFollowTable(MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS &t_table);

  /**
  * A getter method for followTable.
  * Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
  */
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getFollowTable();

  /**
  * Method that is called by the DesignExtractor to populate the 2D follows* matrix.
  */
  void populateFollowsMatrix(TOTAL_NUMBER_OF_STMTS total);

  /**
  * Method that is called by the DesignExtractor to populate the allFollowsMap.
  */
  void populateAllFollowsMap();

  /**
  * Method that is called by the DesignExtractor to populate the allFollowsList and allFollowedByList.
  * Such that results of getFollowsAnything() and getFollowedByAnything() will be O(1) upon retrieval.
  */
  void populateFollowsAnythingRelationships();

  FollowTable();
  ~FollowTable() {};

private:

  //duality hashmaps
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_followMap;
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_followedByMap;

  MAP_OF_STMT_NUMS m_allFollowsMap;
  BOOLEAN_MATRIX m_followsMatrix; //2D representation of the follows* relationship.
  SET_OF_STMT_NUMS m_allFollows;  //all stmt# that follows (in a set)
  LIST_OF_STMT_NUMS m_allFollowsList; //all stmt# that follows (in a vector)
  LIST_OF_STMT_NUMS m_allFollowedByList;  //all stmt# that followed by a stmt (in a vector)
};