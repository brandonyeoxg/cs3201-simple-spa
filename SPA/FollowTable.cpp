#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

#include "FollowTable.h"

void FollowTable::setFollowTable(unordered_map<int, vector<int>> table) {
  m_followTable = table;
}

unordered_map<int, vector<int>> FollowTable::getFollowTable() {
  return m_followTable;
}

/**
* Method that inserts the line number (s2) to the unordered map of vectors containing line number s1 as key.
* @param s1 an integer argument.
* @param s2 an integer argument.
*/
void FollowTable::insert(int s1, int s2) {
  unordered_map<int, vector<int>> followTable = getFollowTable();
  if (followTable.find(s1) == followTable.end()) {
    //if the key is not present in varTable
    vector<int> lineNums;
    lineNums.push_back(s2);
    followTable.emplace(s1, lineNums);
  } else {
    //if not, retrieve the existing vector, append, and put back to followTable.
    //for every existing vector, check if s1 exists. If it does, append s2
    for (auto it = followTable.begin(); it != followTable.end(); it++) {
      vector<int> vect = it->second;  //test?
      if (std::find(vect.begin(), vect.end(), s1) != vect.end()) {
        //if s1 present in vector
        vect.push_back(s2);
      }
    }
  }
}

/**
* Method that retrieves the vector containing all line numbers that follows line s2.
* @param key a string argument.
* @return a vector<int> object.
*/
vector<int> FollowTable::getS1(int s2) {
  //for every vector, check if s2 exists
  //eliminate duplicates from vector (check if exists before adding), then sort(?)
  vector<int> result;
  unordered_map<int, vector<int>> followTable = getFollowTable();
  for (auto it = followTable.begin(); it != followTable.end(); it++) {
    vector<int> vect = it->second;
    if (std::find(vect.begin(), vect.end(), s2) != vect.end()) {
      int lineNum = it->first;
      if (std::find(result.begin(), result.end(), lineNum) == result.end()) {
        //only append to result vector if lineNum is not present (no duplicates).
        result.push_back(lineNum);
      }     
    }
  }
  return vector<int>();
}

/**
* Method that retrieves the vector containing all line numbers that line s1 follows.
* @param key a string argument.
* @return a vector<int> object.
*/
vector<int> FollowTable::getS2(int s1) {
  //retrieves unordered_map with key==s1
  return getFollowTable().at(s1);
}


/**
* A constructor.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
FollowTable::FollowTable() {
  unordered_map<int, vector<int>> m_followTable;
}
