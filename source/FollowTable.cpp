#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

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
  //if the key is not present in followTable
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
