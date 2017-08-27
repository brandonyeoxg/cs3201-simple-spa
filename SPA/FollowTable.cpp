#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

#include "FollowTable.h"

void FollowTable::insert(int s1, int s2) {
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

vector<int> FollowTable::getS1(int s2) {
  //for every vector, check if s2 exists
  //eliminate duplicates from vector (check if exists before adding), then sort(?)
  vector<int> result;
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

vector<int> FollowTable::getS2(int s1) {
  //retrieves unordered_map with key==s1
  return followTable.at(s1);
}

FollowTable::FollowTable() {
  unordered_map<int, vector<int>> followTable;
}
