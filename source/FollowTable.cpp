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

void FollowTable::setFollowTable(MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS &t_table) {
  m_followMap = t_table;
}

MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS FollowTable::getFollowTable() {
  return m_followMap;
}

BOOLEAN FollowTable::insertFollows(STMT_NUM t_s1, STMT_NUM t_s2) {
  if (t_s1 == 0) {
    return false;
  }

  //insertion to followMap
  if (m_followMap.find(t_s1) == m_followMap.end()) {
  //if s1 is not present in followMap
    LIST_OF_STMT_NUMS lineNums;
    lineNums.push_back(t_s2);
    m_followMap.emplace(t_s1, lineNums);
    m_allFollows.insert(t_s2);
  } else {
    //if not, first check if the existing vector consists s2; if it does, return false
    LIST_OF_STMT_NUMS lineNums = m_followMap[t_s1];
    if (std::find(lineNums.begin(), lineNums.end(), t_s2) != lineNums.end()) {
      return false;
    }
  }
  //else, retrieve the existing vector, append, and put back to followMap.
  //also for every existing vector, check if s1 exists. If it does, append s2.
  for (auto it = m_followMap.begin(); it != m_followMap.end(); ++it) {
    LIST_OF_STMT_NUMS vect = it->second;
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
    LIST_OF_STMT_NUMS lineNums;
    lineNums.push_back(t_s1);
    auto iterator = m_followedByMap.find(t_s1);
    if (iterator != m_followedByMap.end()) {
      LIST_OF_STMT_NUMS lineNumsFollowedByS1 = iterator->second;
      lineNums.insert(lineNums.end(), lineNumsFollowedByS1.begin(), lineNumsFollowedByS1.end());
    }
    m_followedByMap.emplace(t_s2, lineNums);
  }
  return true;
}

BOOLEAN FollowTable::isFollows(STMT_NUM t_s1, STMT_NUM t_s2) {
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

BOOLEAN FollowTable::isFollowsStar(STMT_NUM t_s1, STMT_NUM t_s2) {
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

int FollowTable::getFollows(STMT_NUM t_s1) {
  //in this case, since s1 is known,
  //we just retrieve the vector mapped to s1 return the .
  if (m_followMap.find(t_s1) == m_followMap.end()) {
    //if s1 is not present in followMap, throw exception
    throw std::invalid_argument("key s1 does not exist in FollowTable");
  } else {
    LIST_OF_STMT_NUMS lineNums = m_followMap[t_s1];
    return lineNums[0];
  }
}

STMT_NUM FollowTable::getFollowedBy(STMT_NUM t_s2) {
  auto iterator = m_followedByMap.find(t_s2);
  if (iterator == m_followedByMap.end()) {
    throw std::invalid_argument("s2 does not exist in FollowTable");
  } else {
    std::vector<int> lineNumsFollowedBy = iterator->second;
    return lineNumsFollowedBy[0];
  }

}

LIST_OF_STMT_NUMS FollowTable::getFollowsStar(STMT_NUM t_s1) {
  LIST_OF_STMT_NUMS lineNums;
  if (m_followMap.find(t_s1) != m_followMap.end()) {
    lineNums = m_followMap[t_s1];
  }
  return lineNums;
}

LIST_OF_STMT_NUMS FollowTable::getFollowedByStar(STMT_NUM t_s2) {
  LIST_OF_STMT_NUMS result;  //if no results found, return empty vector.
  auto iterator = m_followedByMap.find(t_s2);
  if (iterator != m_followedByMap.end()) {
    result = iterator->second;
  }
  std::sort(result.begin(), result.end());
  return result;
}

MAP_OF_STMT_NUMS FollowTable::getAllFollows() {
  MAP_OF_STMT_NUMS allFollows;
  for (auto it = m_followMap.begin(); it != m_followMap.end(); ++it) {
    STMT_NUM lineNum = it->first;
    LIST_OF_STMT_NUMS vect = it->second;
    allFollows.emplace(lineNum, vect[0]);
  }

  return allFollows;
}

MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS FollowTable::getAllFollowsStar() {
  return m_followMap;
}

LIST_OF_STMT_NUMS FollowTable::getFollowedByAnything() {
  LIST_OF_STMT_NUMS keys;
  for (auto it = m_followMap.begin(); it != m_followMap.end(); ++it) {
    int lineNum = it->first;
    keys.push_back(lineNum);
  }
  return keys;
}

LIST_OF_STMT_NUMS FollowTable::getFollowsAnything() {
  LIST_OF_STMT_NUMS values;
  //copy the m_allFollows set to values vector.
  values.assign(m_allFollows.begin(), m_allFollows.end());
  return values;
}

BOOLEAN FollowTable::hasFollowRelationship() {
  if (m_followMap.size() > 0) {
    return true;
  } else {
    return false;
  }
}

BOOLEAN FollowTable::isFollowsAnything(STMT_NUM t_s2) {
  return (m_allFollows.find(t_s2) != m_allFollows.end());
}

BOOLEAN FollowTable::isFollowedByAnything(STMT_NUM t_s1) {
  if (m_followMap.find(t_s1) != m_followMap.end()) {
    return true;
  } else {
    return false;
  }
}

/**
* A constructor.
* Instantiates unordered maps (hashmap) of line numbers to vector of line numbers associated.
*/
FollowTable::FollowTable() {
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_followMap;
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_followedByMap;
  std::set<int> m_allFollows;
}
