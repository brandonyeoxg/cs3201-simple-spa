#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "FollowTable.h"

void FollowTable::setFollowTable(std::unordered_map<int, std::vector<int>> table) {
  m_followTable = table;
}

std::unordered_map<int, std::vector<int>> FollowTable::getFollowTable() {
  return m_followTable;
}

/**
* Method that inserts the line number (s2) to the unordered map of vectors containing line number s1 as key.
* @param s1 an integer argument.
* @param s2 an integer argument.
*/
bool FollowTable::insertFollows(int s1, int s2) {
  if (m_followTable.find(s1) == m_followTable.end()) {
  //if s1 is not present in followTable
    std::vector<int> lineNums;
    lineNums.push_back(s2);
    m_followTable.emplace(s1, lineNums);
  } else {
    //if not, first check if the existing vector consists s2; if it does, return false
    std::vector<int> lineNums = m_followTable[s1];
    if (std::find(lineNums.begin(), lineNums.end(), s2) != lineNums.end()) {
      return false;
    }
  }
  //else, retrieve the existing vector, append, and put back to followTable.
  //also for every existing vector, check if s1 exists. If it does, append s2.
  for (auto it = m_followTable.begin(); it != m_followTable.end(); ++it) {
    std::vector<int> vect = it->second;
    for (int i = 0; i < vect.size(); i++) {
      if (vect[i] == s1) { //if s1 present in vector
        vect.push_back(s2);
        //update followtable!
        m_followTable[it->first] = vect;
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
bool FollowTable::isFollows(int s1, int s2) {
  //if s1 doesn't exist in followtable, returns false.
  //else, check if s2 is the first element in vector of s1.
  if (m_followTable.find(s1) == m_followTable.end()) {
    //if s1 is not present in followTable
    return false;
  } else {
    std::vector<int> lineNums = m_followTable[s1];
    if (lineNums[0] == s2) {
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
bool FollowTable::isFollowsStar(int s1, int s2) {
  //in this case, since s1 is known,
  //we just retrieve the vector mapped to s1 and check if s2 exists.
  if (m_followTable.find(s1) == m_followTable.end()) {
    //if s1 is not present in followTable
    return false;
  } else {
    std::vector<int> lineNums = m_followTable[s1];
    if (std::find(lineNums.begin(), lineNums.end(), s2) != lineNums.end()) {
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
int FollowTable::getFollows(int s1) {
  //in this case, since s1 is known,
  //we just retrieve the vector mapped to s1 return the .
  if (m_followTable.find(s1) == m_followTable.end()) {
    //if s1 is not present in followTable, throw exception
    throw std::invalid_argument("key s1 does not exist in FollowTable");
  } else {
    std::vector<int> lineNums = m_followTable[s1];
    return lineNums[0];
  }
}

/**
* Method that retrieves the vector containing all line numbers that follows line s2.
* @param key a string argument.
* @return a vector<int> object.
*/
std::vector<int> FollowTable::getS1(int s2) {
  //for every vector, check if s2 exists
  //eliminate duplicates from vector (check if exists before adding), then sort(?)
  std::vector<int> result;
  std::unordered_map<int, std::vector<int>> followTable = getFollowTable();
  for (auto it = followTable.begin(); it != followTable.end(); it++) {
    std::vector<int> vect = it->second;
    if (std::find(vect.begin(), vect.end(), s2) != vect.end()) {
      int lineNum = it->first;
      result.push_back(lineNum);     
    }
  }
  return result;
}

/**
* Method that retrieves the vector containing all line numbers that line s1 follows.
* @param key a string argument.
* @return a vector<int> object.
*/
std::vector<int> FollowTable::getS2(int s1) {
  //retrieves unordered_map with key==s1
  return getFollowTable().at(s1);
}


/**
* A constructor.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
FollowTable::FollowTable() {
  std::unordered_map<int, std::vector<int>> m_followTable;
}
