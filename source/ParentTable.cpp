#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "ParentTable.h"

/**
* Method that inserts the line number (s2) to the unordered map of vectors containing line number s1 as key.
* @param s1 an integer argument.
* @param s2 an integer argument.

ParentTable* ParentTable::insert(ParentTable* table, int s1, int s2) {
  std::unordered_map<int, std::vector<int>> parentTable = table->getParentTable();
  if (parentTable.find(s1) == parentTable.end()) {
    //if the key is not present in varTable
    std::vector<int> lineNums;
    lineNums.push_back(s2);
    parentTable.emplace(s1, lineNums);
  }
  
    //if not, retrieve the existing vector, append, and put back to followTable.
    //for every existing vector, check if s1 exists. If it does, append s2
  for (auto it = parentTable.begin(); it != parentTable.end(); it++) {
    std::vector<int> vect = it->second;  //test?
    for (int i = 0; i < vect.size(); i++) {
      if (vect[i] == s1) { //if s1 present in vector
        vect.push_back(s2);
        //update followtable!
        parentTable[it->first] = vect;
      }
    }
  }
  
  table->setParentTable(parentTable);
  return table;
}
*/
/**
* Method that retrieves the vector containing all line numbers that line s2 nests within.
* @param key a string argument.
* @return a vector<int> object.

std::vector<int> ParentTable::getS1(int s2) {
  //for every vector, check if s2 exists
  //eliminate duplicates from vector (check if exists before adding), then sort(?)
  std::vector<int> result;
  std::unordered_map<int, std::vector<int>> parentTable = getParentTable();
  for (auto it = parentTable.begin(); it != parentTable.end(); it++) {
    std::vector<int> vect = it->second;
    if (std::find(vect.begin(), vect.end(), s2) != vect.end()) {
      int lineNum = it->first;
      if (std::find(result.begin(), result.end(), lineNum) == result.end()) {
        //only append to result vector if lineNum is not present (no duplicates).
        result.push_back(lineNum);
      }
    }
  }
  return result;
}
*/

/**
* Method that retrieves the vector containing all line numbers that line s1 nests within.
* @param key a string argument.
* @return a vector<int> object.

std::vector<int> ParentTable::getS2(int s1) {
  //retrieves unordered_map with key==s1
  return getParentTable().at(s1);
}
*/

bool ParentTable::insertParent(int s1, int s2) {
  if (m_parentMap.find(s2) == m_parentMap.end()) {
    //if s2 does not exist in parentMap
    m_parentMap.emplace(s2, s1);
    //if s1 does not exist in childMap,
    //push s2 into a new vector and map to s1.
    if (m_childMap.find(s1) == m_childMap.end()) {
      std::vector<int> lineNums;
      lineNums.push_back(s2);
      m_childMap.emplace(s1, lineNums);
    } else {
      //s1 already exists, push s2 to existing vector
      std::vector<int> lineNums = m_childMap[s1];
      lineNums.push_back(s2);
      m_childMap[s1] = lineNums;
    }
  } else {
    //s2 already exists in parentMap. Violates the rule that every statement can only have 1 parent.
    return false;
  }

  return true;
}

bool ParentTable::isParent(int s1, int s2) {
  //return true if s2 exists in parentMap as key and s1 is the value.
  if (m_parentMap.find(s2) != m_parentMap.end()) {
    if (m_parentMap[s2] == s1) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool ParentTable::isParentStar(int s1, int s2) {
  int counter = 0;
  int intermediate;
  //if s2 does not exist, return false.
  if (m_parentMap.find(s2) == m_parentMap.end()) {
    return false;
  } else {  //if s2 exists, check its mapped value. if it's s1, return true. 
    intermediate = m_parentMap[s2];
    while (counter <= m_parentMap.size()) {
      //if the mapped value is not s1, check that number's mapped value.
      if (intermediate == s1) {
        return true;
      } else {
        intermediate = m_parentMap[intermediate];
        counter++;
      }
    }
    return false;
  }
}

int ParentTable::getParentOf(int s2) {
  if (m_parentMap.find(s2) == m_parentMap.end()) {
    //if s2 is not present in parentMap, throw exception
    throw std::invalid_argument("key s2 does not exist in ParentTable");
  } else {
    return m_parentMap[s2];
  }
}

std::vector<int> ParentTable::getChildrenOf(int s1) {
  if (m_childMap.find(s1) == m_childMap.end()) {
    //if s1 is not present in childMap, throw exception
    throw std::invalid_argument("key s1 does not exist in ParentTable");
  } else {
    return m_childMap[s1];
  }
}

std::vector<int> ParentTable::getParentStarOf(int s2) {
  int counter = 0;
  int intermediate;
  std::vector<int> result;
  //if s2 does not exist, throw invalid_argument exception.
  if (m_parentMap.find(s2) == m_parentMap.end()) {
    throw std::invalid_argument("key s2 does not exist in ParentTable");
  } else {  //if s2 exists, check its mapped value. if it's s1, return true. 
    intermediate = m_parentMap[s2];
    result.push_back(intermediate);
    while (counter <= m_parentMap.size()) {
      //if the mapped value is not s1, check that number's mapped value.
      if (m_parentMap.find(intermediate) == m_parentMap.end()) {
        break;
      } else {
        intermediate = m_parentMap[intermediate];
        result.push_back(intermediate);
        counter++;
      }
    }
    std::sort(result.begin(), result.end());
    return result;
  }
}

void ParentTable::setChildMap(std::unordered_map<int, std::vector<int>> &map) {
  m_childMap = map;
}

void ParentTable::setParentMap(std::unordered_map<int, int> &map) {
  m_parentMap = map;
}

std::unordered_map<int, std::vector<int>> ParentTable::getChildMap() {
  return m_childMap;
}

std::unordered_map<int, int> ParentTable::getParentMap() {
  return m_parentMap;
}

/**
* A constructor.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
ParentTable::ParentTable() {
  std::unordered_map<int, int> m_parentMap;
  std::unordered_map<int, std::vector<int>> m_childMap;
}
