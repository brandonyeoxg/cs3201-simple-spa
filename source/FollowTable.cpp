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

void FollowTable::setFollowTable(std::unordered_map<int, std::vector<int>> &t_table) {
  m_followMap = t_table;
}

std::unordered_map<int, std::vector<int>> FollowTable::getFollowTable() {
  return m_followMap;
}

bool FollowTable::insertFollows(int t_s1, int t_s2) {
  if (t_s1 == 0) {
    return false;
  }

  //insertion to followMap
  if (m_followMap.find(t_s1) == m_followMap.end()) {
  //if s1 is not present in followMap
    std::vector<int> lineNums;
    lineNums.push_back(t_s2);
    m_followMap.emplace(t_s1, lineNums);
    m_allFollows.insert(t_s2);
  } else {
    //if not, first check if the existing vector consists s2; if it does, return false
    std::vector<int> lineNums = m_followMap[t_s1];
    if (std::find(lineNums.begin(), lineNums.end(), t_s2) != lineNums.end()) {
      return false;
    }
  }
  //else, retrieve the existing vector, append, and put back to followMap.
  //also for every existing vector, check if s1 exists. If it does, append s2.
  for (auto it = m_followMap.begin(); it != m_followMap.end(); ++it) {
    std::vector<int> vect = it->second;
    for (int i = 0; i < vect.size(); i++) {
      if (vect[i] == t_s1) { //if s1 present in vector
        vect.push_back(t_s2);
        //update followMap
        m_followMap[it->first] = vect;
        m_allFollows.insert(t_s2);
      }
    }
  }

  //insertion to followedByMap
  //firstly, check if s2 exists as key in followedByMap.
  if (m_followedByMap.find(t_s2) == m_followedByMap.end()) {
    //if it doesn't, simply emplace s1 as key.
    std::vector<int> lineNums;
    lineNums.push_back(t_s1);
    auto iterator = m_followedByMap.find(t_s1);
    if (iterator != m_followedByMap.end()) {
      std::vector<int> lineNumsFollowedByS1 = iterator->second;
      lineNums.insert(lineNums.end(), lineNumsFollowedByS1.begin(), lineNumsFollowedByS1.end());
    }
    m_followedByMap.emplace(t_s2, lineNums);
  }
  return true;
}

bool FollowTable::isFollows(int t_s1, int t_s2) {
  //if s1 doesn't exist in followtable, returns false.
  //else, check if s2 is the first element in vector of s1.
  if (m_followMap.find(t_s1) == m_followMap.end()) {
    //if s1 is not present in followMap
    return false;
  } else {
    std::vector<int> lineNums = m_followMap[t_s1];
    if (lineNums[0] == t_s2) {
      //is the first element in vector
      return true;
    } else {
      return false;
    }
  }
}

bool FollowTable::isFollowsStar(int t_s1, int t_s2) {
  //in this case, since s1 is known,
  //we just retrieve the vector mapped to s1 and check if s2 exists.
  if (m_followMap.find(t_s1) == m_followMap.end()) {
    //if s1 is not present in followMap
    return false;
  } else {
    std::vector<int> lineNums = m_followMap[t_s1];
    if (std::find(lineNums.begin(), lineNums.end(), t_s2) != lineNums.end()) {
      //can be found in the vector
      return true;
    } else {
      return false;
    }
  }
}

int FollowTable::getFollows(int t_s1) {
  //in this case, since s1 is known,
  //we just retrieve the vector mapped to s1 return the .
  if (m_followMap.find(t_s1) == m_followMap.end()) {
    //if s1 is not present in followMap, throw exception
    throw std::invalid_argument("key s1 does not exist in FollowTable");
  } else {
    std::vector<int> lineNums = m_followMap[t_s1];
    return lineNums[0];
  }
}

int FollowTable::getFollowedBy(int t_s2) {
  auto iterator = m_followedByMap.find(t_s2);
  if (iterator == m_followedByMap.end()) {
    throw std::invalid_argument("s2 does not exist in FollowTable");
  } else {
    std::vector<int> lineNumsFollowedBy = iterator->second;
    return lineNumsFollowedBy[0];
  }

}

std::vector<int> FollowTable::getFollowsStar(int t_s1) {
  std::vector<int> lineNums;
  if (m_followMap.find(t_s1) != m_followMap.end()) {
    lineNums = m_followMap[t_s1];
  }
  return lineNums;
}

std::vector<int> FollowTable::getFollowedByStar(int t_s2) {
  std::vector<int> result;  //if no results found, return empty vector.
  auto iterator = m_followedByMap.find(t_s2);
  if (iterator != m_followedByMap.end()) {
    result = iterator->second;
  }
  std::sort(result.begin(), result.end());
  return result;
}

std::unordered_map<int, int> FollowTable::getAllFollows() {
  std::unordered_map<int, int> allFollows;
  for (auto it = m_followMap.begin(); it != m_followMap.end(); ++it) {
    int lineNum = it->first;
    std::vector<int> vect = it->second;
    allFollows.emplace(lineNum, vect[0]);
  }

  return allFollows;
}

std::unordered_map<int, std::vector<int>> FollowTable::getAllFollowsStar() {
  return m_followMap;
}

std::vector<int> FollowTable::getFollowedByAnything() {
  std::vector<int> keys;
  for (auto it = m_followMap.begin(); it != m_followMap.end(); ++it) {
    int lineNum = it->first;
    keys.push_back(lineNum);
  }
  return keys;
}

std::vector<int> FollowTable::getFollowsAnything() {
  std::vector<int> values;
  //copy the m_allFollows set to values vector.
  values.assign(m_allFollows.begin(), m_allFollows.end());
  return values;
}

bool FollowTable::hasFollowRelationship() {
  if (m_followMap.size() > 0) {
    return true;
  } else {
    return false;
  }
}

bool FollowTable::isFollowsAnything(int t_s2) {
  return (m_allFollows.find(t_s2) != m_allFollows.end());
}

bool FollowTable::isFollowedByAnything(int t_s1) {
  if (m_followMap.find(t_s1) != m_followMap.end()) {
    return true;
  } else {
    return false;
  }
}

/**
* A constructor.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
FollowTable::FollowTable() {
  std::unordered_map<int, std::vector<int>> m_followMap;
  std::unordered_map<int, std::vector<int>> m_followedByMap;
  std::set<int> m_allFollows;
}
