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
  bool insertFollows(int s1, int s2);

  /**
  * Method that checks if follows(s1, s2) holds.
  * Checks if s2 exists as the first element in vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  bool isFollows(int s1, int s2);

  /**
  * Method that checks if follows*(s1, s2) holds.
  * Checks if s2 exists within the vector mapped to key s1.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return true if the relationship holds, false if otherwise.
  */
  bool isFollowsStar(int s1, int s2);

  /**
  * Method that returns the line number that follows(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param s1 an integer argument.
  * @return the line number that line s1 follows.
  */
  int getFollows(int s1);

  /**
  * Method that returns the line number that follows(s, s2) holds, where s is a variable and s2 is a known line number.
  * @param s2 an integer argument.
  * @return the line number that is followed by line s2.
  */
  int getFollowedBy(int s2);

  /**
  * Method that returns the list of line numbers that follows*(s1, s) holds, where s is a variable and s1 is a known line number.
  * Returns the first element in the vector mapped to key s1.
  * @param s1 an integer argument.
  * @return the vector of line numbers that line s1 follows*.
  */
  std::vector<int> getFollowsStar(int s1);

  /**
  * Method that returns the list of line numbers that follows*(s, s2) holds, where s is a variable and s2 is a known line number.
  * Returns the vector of keys that the vector mapped to the key contains s2.
  * @param s1 an integer argument.
  * @return the vector of line numbers that are followedBy* s2.
  */
  std::vector<int> getFollowedByStar(int s2);

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

  std::vector<int> getFollowedByAnything(); //Follows(s1, _)
  std::vector<int> getFollowsAnything();  //Follows (_, s2)
  std::vector<int> getFollowedByStarAnything(); //Follows*(s1, _)
  std::vector<int> getFollowsStarAnything();  //Follows* (_, s2)
  bool hasFollowRelationship(); //Follows (_, _)
  bool hasFollowStarRelationship(); //Follows* (_, _)
  bool isFollowsAnything(int t_s2); //follows(_, 2)
  bool isFollowedByAnything(int t_s1);  //follows(2, _)
  bool isFollowsStarAnything(int t_s2); //follows*(_, 2)
  bool isFollowedByStarAnything(int t_s1);  //follows*(2, _)













  /**
  * A setter method for followTable.
  * Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
  */
  void setFollowTable(std::unordered_map<int, std::vector<int>> &table);

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