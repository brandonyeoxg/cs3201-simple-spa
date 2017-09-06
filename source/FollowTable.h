#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

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
  * @param s2 an integer argument.
  * @return the line number that line s1 follows.
  */
  int getFollows(int s1);
  std::vector<int> getS1(int s2);
  std::vector<int> getS2(int s1);
  void setFollowTable(std::unordered_map<int, std::vector<int>> table);
  std::unordered_map<int, std::vector<int>> getFollowTable();
  FollowTable();

private:
  std::unordered_map<int, std::vector<int>> m_followTable;
};