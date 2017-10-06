#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <algorithm>

#include "ParentTable.h"

bool ParentTable::insertParent(int t_s1, int t_s2) {
  if (t_s1 == 0) {
    return false;
  }
  if (m_parentMap.find(t_s2) == m_parentMap.end()) {
    //if s2 does not exist in parentMap
    m_parentMap.emplace(t_s2, t_s1);
    //if s1 does not exist in childMap,
    //push s2 into a new vector and map to s1.
    if (m_childMap.find(t_s1) == m_childMap.end()) {
      std::vector<int> lineNums;
      lineNums.push_back(t_s2);
      m_childMap.emplace(t_s1, lineNums);
    } else {
      //s1 already exists, push s2 to existing vector
      std::vector<int> lineNums = m_childMap[t_s1];
      lineNums.push_back(t_s2);
      m_childMap[t_s1] = lineNums;
    }
  } else {
    //s2 already exists in parentMap. Violates the rule that every statement can only have 1 parent.
    return false;
  }

  return true;
}

bool ParentTable::isParent(int t_s1, int t_s2) {
  //return true if s2 exists in parentMap as key and s1 is the value.
  if (m_parentMap.find(t_s2) != m_parentMap.end()) {
    if (m_parentMap[t_s2] == t_s1) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool ParentTable::isParentStar(int t_s1, int t_s2) {
  auto pItr = m_parentStarMap.find(t_s1);
  if (pItr == m_parentStarMap.end()) {
    //not found in parent*Map
    return false;
  } else {
    std::vector<int> childrenStar = pItr->second;
    if (std::find(childrenStar.begin(), childrenStar.end(), t_s2) != childrenStar.end()) {
      return true;
    } else {
      return false;
    }
  }
}

int ParentTable::getParentOf(int t_s2) {
  if (m_parentMap.find(t_s2) == m_parentMap.end()) {
    //if s2 is not present in parentMap, throw exception
    throw std::invalid_argument("key s2 does not exist in ParentTable");
  } else {
    return m_parentMap[t_s2];
  }
}

std::vector<int> ParentTable::getChildrenOf(int t_s1) {
  std::vector<int> emptyVector;
  if (m_childMap.find(t_s1) == m_childMap.end()) {
    //if s1 is not present in childMap, throw exception
    return emptyVector;
  } else {
    return m_childMap[t_s1];
  }
}

std::vector<int> ParentTable::getParentStarOf(int t_s2) {
  std::vector<int> emptyVector;
  if (m_parentedByStarMap.find(t_s2) == m_parentedByStarMap.end()) {
    return emptyVector;
  }
  auto iterator = m_parentedByStarMap.find(t_s2);
  return iterator->second;
}

std::vector<int> ParentTable::getChildrenStarOf(int t_s1) {
  std::vector<int> emptyVector;
  //if does not exist in childMap, return empty vector.
  if (m_childMap.find(t_s1) == m_childMap.end()) {
    return emptyVector;
  }

  //new implementation: use m_parentStarTable to query for s1.
  auto iterator = m_parentStarMap.find(t_s1);
  return iterator->second;
}

std::unordered_map<int, std::vector<int>> ParentTable::getAllParents() {
  return m_childMap;
}

std::vector<int> ParentTable::getChildrenOfAnything() {
  //parent(_,s2) s2 variable
  //return list of children i.e. keys of m_parentMap
  std::vector<int> result;
  for (auto it = m_parentMap.begin(); it != m_parentMap.end(); ++it) {
    result.push_back(it->first);
  }

  return result;
}

std::vector<int> ParentTable::getParentOfAnything() {
  //return list of parents i.e. keys of m_childMap
  std::vector<int> result;
  for (auto it = m_childMap.begin(); it != m_childMap.end(); ++it) {
    result.push_back(it->first);
  }

  return result;
}

std::vector<int> ParentTable::getChildrenStarOfAnything() {
  //parent*(_, s2)
  //return list of children* i.e. keys of parentedByStarMap
  std::vector<int> result;
  for (auto it = m_parentedByStarMap.begin(); it != m_parentedByStarMap.end(); ++it) {
    result.push_back(it->first);
  }

  return result;
}

std::vector<int> ParentTable::getParentStarOfAnything() {
  //parent*(s1, _)
  //return list of parent* i.e. keys of parentStarMap
  std::vector<int> result;
  for (auto it = m_parentStarMap.begin(); it != m_parentStarMap.end(); ++it) {
    result.push_back(it->first);
  }

  return result;
}

bool ParentTable::hasParentRelationship() {
  //Parent (_, _)
  if (m_parentMap.size() > 0) {
    return true;
  } else {
    return false;
  }
}

bool ParentTable::hasParentStarRelationship() {
  //Parent* (_, _)
  if (m_parentStarMap.size() > 0) {
    return true;
  } else {
    return false;
  }
}

bool ParentTable::isChildrenOfAnything(int t_s2) {
  //parent(_, 2)
  //check if 2 exists as a child (key in parentMap)
  for (auto it = m_parentMap.begin(); it != m_parentMap.end(); ++it) {
    if (t_s2 == it->first) {
      return true;
    }
  }
  return false;
}

bool ParentTable::isParentOfAnything(int t_s1) {
  //parent(2, _)
  //check if 2 exists as a parent (key in childMap)
  for (auto it = m_childMap.begin(); it != m_childMap.end(); ++it) {
    if (t_s1 == it->first) {
      return true;
    }
  }
  return false;
}

bool ParentTable::isChildrenOfStarAnything(int t_s2) {
  //parent*(_, 2)
  //check if 2 exists as a key in parentedByStarMap
  for (auto it = m_parentedByStarMap.begin(); it != m_parentedByStarMap.end(); ++it) {
    if (t_s2 == it->first) {
      return true;
    }
  }
  return false;
}

bool ParentTable::isParentOfStarAnything(int t_s1) {
  //parent*(2, _)
  //check if 2 exists as a key in parentStarMap
  for (auto it = m_parentStarMap.begin(); it != m_parentStarMap.end(); ++it) {
    if (t_s1 == it->first) {
      return true;
    }
  }
  return false;
}




//getter and setters
void ParentTable::setChildMap(std::unordered_map<int, std::vector<int>> &t_map) {
  m_childMap = t_map;
}

void ParentTable::setParentMap(std::unordered_map<int, int> &t_map) {
  m_parentMap = t_map;
}

void ParentTable::setParentStarMap(std::unordered_map<int, std::vector<int>> &t_map) {
  m_parentStarMap = t_map;
}

void ParentTable::setParentedByStarMap(std::unordered_map<int, std::vector<int>> &t_map) {
  m_parentedByStarMap = t_map;
}

std::unordered_map<int, std::vector<int>>& ParentTable::getChildMap() {
  return m_childMap;
}

std::unordered_map<int, int>& ParentTable::getParentMap() {
  return m_parentMap;
}

std::unordered_map<int, std::vector<int>>& ParentTable::getParentStarMap() {
  return m_parentStarMap;
}

std::unordered_map<int, std::vector<int>>& ParentTable::getParentedByStarMap() {
  return m_parentedByStarMap;
}

/**
* A constructor.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
ParentTable::ParentTable() {
  std::unordered_map<int, int> m_parentMap;
  std::unordered_map<int, std::vector<int>> m_childMap;
  std::unordered_map<int, std::vector<std::vector<int>>> m_parentStarMap;
  std::unordered_map<int, std::vector<int>> m_parentedByStarMap;
}
