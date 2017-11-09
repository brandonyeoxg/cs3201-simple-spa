#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <algorithm>

#include "ParentTable.h"

BOOLEAN ParentTable::insertParent(int t_s1, int t_s2) {
  if (t_s1 == 0) {
    return false;
  }
  if (m_parentMap.find(t_s2) == m_parentMap.end()) {
    //if s2 does not exist in parentMap
    m_parentMap.emplace(t_s2, t_s1);
    //if s1 does not exist in childMap,
    //push s2 into a new vector and map to s1.
    if (m_childMap.find(t_s1) == m_childMap.end()) {
      LIST_OF_STMT_NUMS lineNums;
      lineNums.push_back(t_s2);
      m_childMap.emplace(t_s1, lineNums);
    } else {
      //s1 already exists, push s2 to existing vector
      LIST_OF_STMT_NUMS lineNums = m_childMap[t_s1];
      lineNums.push_back(t_s2);
      m_childMap[t_s1] = lineNums;
    }
  } else {
    //s2 already exists in parentMap. Violates the rule that every statement can only have 1 parent.
    return false;
  }

  return true;
}

BOOLEAN ParentTable::isParent(int t_s1, int t_s2) {
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

BOOLEAN ParentTable::isParentStar(int t_s1, int t_s2) {
  TOTAL_NUMBER_OF_STMTS total = m_parentMatrix.size();
  if (t_s1 >= total || t_s2 >= total) {
    return false;
  } else {
    return m_parentMatrix.at(t_s1).at(t_s2);
  }
}

STMT_NUM ParentTable::getParentOf(int t_s2) {
  if (m_parentMap.find(t_s2) == m_parentMap.end()) {
    //if s2 is not present in parentMap, throw exception
    throw InvalidArgumentException("key s2 does not exist in ParentTable");
  } else {
    return m_parentMap[t_s2];
  }
}

LIST_OF_STMT_NUMS ParentTable::getChildrenOf(int t_s1) {
  LIST_OF_STMT_NUMS emptyVector;
  if (m_childMap.find(t_s1) == m_childMap.end()) {
    //if s1 is not present in childMap, throw exception
    return emptyVector;
  } else {
    return m_childMap[t_s1];
  }
}

LIST_OF_STMT_NUMS ParentTable::getParentStarOf(int t_s2) {
  LIST_OF_STMT_NUMS emptyVector;
  if (m_parentedByStarMap.find(t_s2) == m_parentedByStarMap.end()) {
    return emptyVector;
  }
  auto iterator = m_parentedByStarMap.find(t_s2);
  return iterator->second;
}

LIST_OF_STMT_NUMS ParentTable::getChildrenStarOf(int t_s1) {
  LIST_OF_STMT_NUMS emptyVector;
  //if does not exist in childMap, return empty vector.
  if (m_childMap.find(t_s1) == m_childMap.end()) {
    return emptyVector;
  }

  //new implementation: use m_parentStarTable to query for s1.
  auto iterator = m_parentStarMap.find(t_s1);
  return iterator->second;
}

MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS ParentTable::getAllParents() {
  return m_childMap;
}

LIST_OF_STMT_NUMS ParentTable::getChildrenOfAnything() {
  //parent(_,s2) s2 variable
  //return list of children i.e. keys of m_parentMap
  LIST_OF_STMT_NUMS result;
  for (auto it = m_parentMap.begin(); it != m_parentMap.end(); ++it) {
    result.push_back(it->first);
  }

  return result;
}

LIST_OF_STMT_NUMS ParentTable::getParentOfAnything() {
  //return list of parents i.e. keys of m_childMap
  LIST_OF_STMT_NUMS result;
  for (auto it = m_childMap.begin(); it != m_childMap.end(); ++it) {
    result.push_back(it->first);
  }

  return result;
}

LIST_OF_STMT_NUMS ParentTable::getChildrenStarOfAnything() {
  //parent*(_, s2)
  //return list of children* i.e. keys of parentedByStarMap
  LIST_OF_STMT_NUMS result;
  for (auto it = m_parentedByStarMap.begin(); it != m_parentedByStarMap.end(); ++it) {
    result.push_back(it->first);
  }

  return result;
}

LIST_OF_STMT_NUMS ParentTable::getParentStarOfAnything() {
  //parent*(s1, _)
  //return list of parent* i.e. keys of parentStarMap
  std::vector<int> result;
  for (auto it = m_parentStarMap.begin(); it != m_parentStarMap.end(); ++it) {
    result.push_back(it->first);
  }

  return result;
}

BOOLEAN ParentTable::hasParentRelationship() {
  //Parent (_, _)
  if (m_parentMap.size() > 0) {
    return true;
  } else {
    return false;
  }
}

BOOLEAN ParentTable::hasParentStarRelationship() {
  //Parent* (_, _)
  if (m_parentStarMap.size() > 0) {
    return true;
  } else {
    return false;
  }
}

BOOLEAN ParentTable::isChildrenOfAnything(int t_s2) {
  //parent(_, 2)
  //check if 2 exists as a child (key in parentMap)
  for (auto it = m_parentMap.begin(); it != m_parentMap.end(); ++it) {
    if (t_s2 == it->first) {
      return true;
    }
  }
  return false;
}

BOOLEAN ParentTable::isParentOfAnything(int t_s1) {
  //parent(2, _)
  //check if 2 exists as a parent (key in childMap)
  for (auto it = m_childMap.begin(); it != m_childMap.end(); ++it) {
    if (t_s1 == it->first) {
      return true;
    }
  }
  return false;
}

BOOLEAN ParentTable::isChildrenOfStarAnything(int t_s2) {
  //parent*(_, 2)
  //check if 2 exists as a key in parentedByStarMap
  for (auto it = m_parentedByStarMap.begin(); it != m_parentedByStarMap.end(); ++it) {
    if (t_s2 == it->first) {
      return true;
    }
  }
  return false;
}

BOOLEAN ParentTable::isParentOfStarAnything(int t_s1) {
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
void ParentTable::setChildMap(MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS &t_map) {
  m_childMap = t_map;
}

void ParentTable::setParentMap(MAP_OF_STMT_NUMS &t_map) {
  m_parentMap = t_map;
}

MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS& ParentTable::getChildMap() {
  return m_childMap;
}

MAP_OF_STMT_NUMS& ParentTable::getParentMap() {
  return m_parentMap;
}

MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS& ParentTable::getParentStarMap() {
  return m_parentStarMap;
}

MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS& ParentTable::getParentedByStarMap() {
  return m_parentedByStarMap;
}

void ParentTable::populateParentMatrix(TOTAL_NUMBER_OF_STMTS total) {
  m_parentMatrix = BOOLEAN_MATRIX(total + 1, std::vector<bool>(total + 1, false));
  for (auto it = m_parentStarMap.begin(); it != m_parentStarMap.end(); ++it) {
    STMT_NUM s1 = it->first;
    LIST_OF_STMT_NUMS numList = it->second;
    for (int i : numList) {
      m_parentMatrix.at(s1).at(i) = true;
    }
  }
}

//include here for unit testing purposes.
void ParentTable::populateParentStarMap() {
  for (auto it = m_childMap.begin(); it != m_childMap.end(); ++it) {
    int parent = it->first;
    LIST_OF_STMT_NUMS children = it->second;
    m_parentStarMap.emplace(parent, children);
    std::vector<int> childrenStar = children;
    for (int i = 0; i < childrenStar.size(); i++) {
      //for every child, if it can be found in the map, append all from it's mapped vector to children
      auto iterator = m_childMap.find(childrenStar[i]);
      if (iterator != m_childMap.end()) {
        LIST_OF_STMT_NUMS toBeAppended = iterator->second;
        childrenStar.reserve(childrenStar.size() + toBeAppended.size());
        childrenStar.insert(childrenStar.end(), toBeAppended.begin(), toBeAppended.end());
      }
    }
    m_parentStarMap[parent] = childrenStar;
  }
}
/**
* A constructor.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
ParentTable::ParentTable() {
}
