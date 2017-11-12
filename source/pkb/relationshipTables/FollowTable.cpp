#pragma once

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
    LIST_OF_STMT_NUMS lineNums = m_followMap[t_s1];
    if (lineNums[0] == t_s2) {
      //is the first element in vector
      return true;
    } else {
      return false;
    }
  }
}

BOOLEAN FollowTable::isFollowsStar(STMT_NUM t_s1, STMT_NUM t_s2) {
  TOTAL_NUMBER_OF_STMTS total = m_followsMatrix.size();
  if (t_s1 >= total || t_s2 >= total) {
    return false;
  } else {
    return m_followsMatrix.at(t_s1).at(t_s2);
  }
}

STMT_NUM FollowTable::getFollows(STMT_NUM t_s1) {
  //in this case, since s1 is known,
  //we just retrieve the vector mapped to s1 return the .
  if (m_followMap.find(t_s1) == m_followMap.end()) {
    //if s1 is not present in followMap, throw exception
    throw InvalidArgumentException("key s1 does not exist in FollowTable");
  } else {
    LIST_OF_STMT_NUMS lineNums = m_followMap[t_s1];
    return lineNums[0];
  }
}

STMT_NUM FollowTable::getFollowedBy(STMT_NUM t_s2) {
  auto iterator = m_followedByMap.find(t_s2);
  if (iterator == m_followedByMap.end()) {
    throw InvalidArgumentException("s2 does not exist in FollowTable");
  } else {
    LIST_OF_STMT_NUMS lineNumsFollowedBy = iterator->second;
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
  //std::sort(result.begin(), result.end());
  return result;
}

MAP_OF_STMT_NUMS FollowTable::getAllFollows() {
  return m_allFollowsMap;
}

MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS FollowTable::getAllFollowsStar() {
  return m_followMap;
}

LIST_OF_STMT_NUMS FollowTable::getFollowedByAnything() {
  return m_allFollowedByList;
}

LIST_OF_STMT_NUMS FollowTable::getFollowsAnything() {
  return m_allFollowsList;
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

void FollowTable::populateFollowsMatrix(TOTAL_NUMBER_OF_STMTS total) {
  m_followsMatrix = BOOLEAN_MATRIX(total+1, std::vector<bool>(total+1, false));
  //for every key in m_followMap
  for (auto it = m_followMap.begin(); it != m_followMap.end(); ++it) {
    STMT_NUM s1 = it->first;
    LIST_OF_STMT_NUMS numList = it->second;
    for (int i: numList) {
      m_followsMatrix.at(s1).at(i) = true;
    }
  }
}

void FollowTable::populateAllFollowsMap() {
  for (auto it = m_followMap.begin(); it != m_followMap.end(); ++it) {
    STMT_NUM lineNum = it->first;
    LIST_OF_STMT_NUMS vect = it->second;
    m_allFollowsMap.emplace(lineNum, vect[0]);
  }
}

void FollowTable::populateFollowsAnythingRelationships() {
  for (auto it = m_followMap.begin(); it != m_followMap.end(); ++it) {
    STMT_NUM lineNum = it->first;
    m_allFollowedByList.push_back(lineNum);
  }

  m_allFollowsList.assign(m_allFollows.begin(), m_allFollows.end());
}

FollowTable::FollowTable() {}