#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <stdexcept>
#include <algorithm>

#include "FollowTable.h"

/**
* A setter method for followTable.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
void FollowTable::setFollowTable(std::unordered_map<int, std::vector<int>> &t_table) {
  m_followTable = t_table;
}

/**
* A getter method for followTable.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
std::unordered_map<int, std::vector<int>> FollowTable::getFollowTable() {
  return m_followTable;
}

/**
* Method that inserts the line number (s2) to the unordered map of vectors containing line number s1 as key.
* @param s1 an integer argument.
* @param s2 an integer argument.
*/
bool FollowTable::insertFollows(int t_s1, int t_s2) {
  if (t_s1 == 0) {
    return false;
  }
  if (m_followTable.find(t_s1) == m_followTable.end()) {
  //if s1 is not present in followTable
    std::vector<int> lineNums;
    lineNums.push_back(t_s2);
    m_followTable.emplace(t_s1, lineNums);
    m_allFollows.insert(t_s2);
  } else {
    //if not, first check if the existing vector consists s2; if it does, return false
    std::vector<int> lineNums = m_followTable[t_s1];
    if (std::find(lineNums.begin(), lineNums.end(), t_s2) != lineNums.end()) {
      return false;
    }
  }
  //else, retrieve the existing vector, append, and put back to followTable.
  //also for every existing vector, check if s1 exists. If it does, append s2.
  for (auto it = m_followTable.begin(); it != m_followTable.end(); ++it) {
    std::vector<int> vect = it->second;
    for (int i = 0; i < vect.size(); i++) {
      if (vect[i] == t_s1) { //if s1 present in vector
        vect.push_back(t_s2);
        //update followtable!
        m_followTable[it->first] = vect;
        m_allFollows.insert(t_s2);
      }
    }
  }
  return true;
}

/**
* Method that checks if follows(s1, s2) holds.
* @param s1 an integer argument.
* @param s2 an integer argument.
* @return true if the relationship holds, false if otherwise.
*/
bool FollowTable::isFollows(int t_s1, int t_s2) {
  //if s1 doesn't exist in followtable, returns false.
  //else, check if s2 is the first element in vector of s1.
  if (m_followTable.find(t_s1) == m_followTable.end()) {
    //if s1 is not present in followTable
    return false;
  } else {
    std::vector<int> lineNums = m_followTable[t_s1];
    if (lineNums[0] == t_s2) {
      //is the first element in vector
      return true;
    } else {
      return false;
    }
  }
}

/**
* Method that checks if follows*(s1, s2) holds.
* @param s1 an integer argument.
* @param s2 an integer argument.
* @return true if the relationship holds, false if otherwise.
*/
bool FollowTable::isFollowsStar(int t_s1, int t_s2) {
  //in this case, since s1 is known,
  //we just retrieve the vector mapped to s1 and check if s2 exists.
  if (m_followTable.find(t_s1) == m_followTable.end()) {
    //if s1 is not present in followTable
    return false;
  } else {
    std::vector<int> lineNums = m_followTable[t_s1];
    if (std::find(lineNums.begin(), lineNums.end(), t_s2) != lineNums.end()) {
      //can be found in the vector
      return true;
    } else {
      return false;
    }
  }
}

/**
* Method that returns the line number that follows(s1, s) holds, where s is a variable and s1 is a known line number.
* Returns the first element in the vector mapped to key s1.
* @param s1 an integer argument.
* @param s2 an integer argument.
* @return the line number that line s1 follows.
*/
int FollowTable::getFollows(int t_s1) {
  //in this case, since s1 is known,
  //we just retrieve the vector mapped to s1 return the .
  if (m_followTable.find(t_s1) == m_followTable.end()) {
    //if s1 is not present in followTable, throw exception
    throw std::invalid_argument("key s1 does not exist in FollowTable");
  } else {
    std::vector<int> lineNums = m_followTable[t_s1];
    return lineNums[0];
  }
}

/**
* Method that returns the line number that follows(s, s2) holds, where s is a variable and s1 is a known line number.
* Check every vector in the map until s2 appears, then return the statement number that is positioned right before s2.
* @param s1 an integer argument.
* @param s2 an integer argument.
* @return the line number that line s1 follows.
*/
int FollowTable::getFollowedBy(int t_s2) {
  for (auto it = m_followTable.begin(); it != m_followTable.end(); ++it) {
    std::vector<int> vect = it->second;
    for (int i = 0; i < vect.size(); i++) {
      if (vect[i] == t_s2) { //if s2 present in vector
        if (i == 0) { //if s2 is the only element, it means s2 is followed by line number (the key).
          return it->first;
        } else {
          return vect[i - 1];
        }
      }
    }
  }
  //if nothing is returned after the for loop terminates, throw invalid_argument exception (i.e. s2 does not exist in followTable).
  throw std::invalid_argument("s2 does not exist in FollowTable");
}

/**
* Method that returns the list of line numbers that follows(s1, s) holds, where s is a variable and s1 is a known line number.
* Returns all the line numbers in the vector mapped to key s1.
* @param s1 an integer argument.
* @return the line number that line s1 follows.
*/
std::vector<int> FollowTable::getFollowsStar(int t_s1) {
  std::vector<int> lineNums;
  if (m_followTable.find(t_s1) != m_followTable.end()) {
    lineNums = m_followTable[t_s1];
  }
  return lineNums;
}

/**
* Method that returns the list of line numbers that follows*(s, s2) holds, where s is a variable and s2 is a known line number.
* Returns the vector of keys that the vector mapped to the key contains s2.
* @param s1 an integer argument.
* @return the vector of line numbers that are followedBy* s2.
*/
std::vector<int> FollowTable::getFollowedByStar(int t_s2) {
  std::vector<int> result;
  for (auto it = m_followTable.begin(); it != m_followTable.end(); it++) {
    std::vector<int> vect = it->second;
    if (std::find(vect.begin(), vect.end(), t_s2) != vect.end()) {
      int lineNum = it->first;
      result.push_back(lineNum);
    }
  }
  std::sort(result.begin(), result.end());  //sort according to ascending order.
  return result;
}

/**
* Method that returns the entire map of line numbers that follows(s1, s2) holds, where s1 and s2 are both variables.
* @return the entire map that keep tracks of the follow relationship.
*/
std::unordered_map<int, int> FollowTable::getAllFollows() {
  std::unordered_map<int, int> allFollows;
  for (auto it = m_followTable.begin(); it != m_followTable.end(); ++it) {
    int lineNum = it->first;
    std::vector<int> vect = it->second;
    allFollows.emplace(lineNum, vect[0]);
  }

  return allFollows;
}

/**
* Method that returns the entire map of line numbers that follows*(s1, s2) holds, where s1 and s2 are both variables.
* @return the entire map that keep tracks of the follow relationship.
*/
std::unordered_map<int, std::vector<int>> FollowTable::getAllFollowsStar() {
  return m_followTable;
}

/**
* Method that returns the list of line numbers that follows(s1, _) and follows*(s1, _) holds, where s1 is a variable.
* @return the vector of keys within the followTable.
*/
std::vector<int> FollowTable::getFollowedByAnything() {
  std::vector<int> keys;
  for (auto it = m_followTable.begin(); it != m_followTable.end(); ++it) {
    int lineNum = it->first;
    keys.push_back(lineNum);
  }
  return keys;
}

/**
* Method that returns the list of line numbers that follows(_, s2) holds, where s2 is a variable.
* @return the vector of unique values within the followTable.
*/
std::vector<int> FollowTable::getFollowsAnything() {
  std::vector<int> values;
  //copy the m_allFollows set to values vector.
  values.assign(m_allFollows.begin(), m_allFollows.end());
  return values;
}

/**
* Method that checks if follows(_, _) or follows*(_, _) holds, where s2 is a variable.
* @return true if the size of the followTable is more than zero, return false if otherwise.
*/
bool FollowTable::hasFollowRelationship() {
  bool isFollowTableEmpty;
  if (m_followTable.size() > 0) {
    return true;
  } else {
    return false;
  }
}

/**
* Method that checks if follows(_, s2) and follows*(_, s2) holds, where s2 is a statement number.
* @return true if s2 exists in the allFollows map, return false if otherwise.
*/
bool FollowTable::isFollowsAnything(int t_s2) {
  return (m_allFollows.find(t_s2) != m_allFollows.end());
}

/**
* Method that checks if follows(s1, _) and follows*(s1, _) holds, where s1 is a statement number.
* @return true if s1 exists as a key in followTable, return false if otherwise.
*/
bool FollowTable::isFollowedByAnything(int t_s1) {
  for (auto it = m_followTable.begin(); it != m_followTable.end(); ++it) {
    if (t_s1 == it->first) {
      return true;
    }
  }
  return false;
}

/**
* A constructor.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
FollowTable::FollowTable() {
  std::unordered_map<int, std::vector<int>> m_followTable;
  std::set<int> m_allFollows;
}
