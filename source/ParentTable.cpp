#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "ParentTable.h"

/**
* Method that inserts the line number (s2) to the unordered map of vectors containing line number s1 as key.
* @param s1 an integer argument.
* @param s2 an integer argument.
*/
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

/**
* Method that retrieves the vector containing all line numbers that line s2 nests within.
* @param key a string argument.
* @return a vector<int> object.
*/
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

/**
* Method that retrieves the vector containing all line numbers that line s1 nests within.
* @param key a string argument.
* @return a vector<int> object.
*/
std::vector<int> ParentTable::getS2(int s1) {
  //retrieves unordered_map with key==s1
  return getParentTable().at(s1);
}

void ParentTable::setParentTable(std::unordered_map<int, std::vector<int>> table) {
  m_parentTable = table;
}

std::unordered_map<int, std::vector<int>> ParentTable::getParentTable() {
  return m_parentTable;
}

/**
* A constructor.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
ParentTable::ParentTable() {
  std::unordered_map<int, std::vector<int>> parentTable;
}
