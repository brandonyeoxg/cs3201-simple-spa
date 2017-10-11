#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

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
  * Method that returns the entire map of line numbers that follows(s1, s2) holds, where s1 and s2 are both variables.
  * @return the entire map that keep tracks of the follow relationship.
  */
  std::unordered_map<int, int> getAllFollows();

  /**
  * Method that returns the entire map of line numbers that follows*(s1, s2) holds, where s1 and s2 are both variables.
  * @return the entire map that keep tracks of the follow relationship.
  */
  std::unordered_map<int, std::vector<int>> getAllFollowsStar();

  /**
  * Method that returns the list of line numbers that follows(s1, _) and follows*(s1, _) holds, where s1 is a variable.
  * @return the vector of keys within the followTable.
  */
  std::vector<int> getFollowedByAnything();

  /**
  * Method that returns the list of line numbers that follows (_, s2) and Follows* (_, s2) holds, where s1 is a variable.
  * @return the vector of keys within the followTable.
  */
  std::vector<int> getFollowsAnything();

  /**
  * Method that checks if follows(_, _) or follows*(_, _) holds, where s2 is a variable.
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

  ///////////////////////////////////////////////////////
  //  Getter and Setter methods 
  ///////////////////////////////////////////////////////

  /**
  * A setter method for followTable.
  * Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
  */
  void setFollowTable(std::unordered_map<int, std::vector<int>> &t_table);

  /**
  * A getter method for followTable.
  * Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
  */
  std::unordered_map<int, std::vector<int>> getFollowTable();

  /**
  * A constructor.
  * Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
  */
  FollowTable();

private:
  std::unordered_map<int, std::vector<int>> m_followTable;
  std::set<int> m_allFollows;
};