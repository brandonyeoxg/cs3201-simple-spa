#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

#include "VarTable.h"

/**
 * Method that inserts the line number to the unordered map of vectors containing the variable as key.
 * @param key a string argument.
 * @param lineNum an integer argument.
 */
void VarTable::insert(string key, int lineNum) {
  unordered_map<string, vector<int>> varTable = getVarTable();
  if (varTable.find(key) == varTable.end()) {
    //if the key is not present in varTable
    vector<int> lineNums;
    lineNums.push_back(lineNum);
    varTable.emplace(key, lineNums);
  } else {
    //if not, retrieve the existing vector, append, and put back to varTable.
    vector<int> vect = varTable.at(key);
    vect.push_back(lineNum);
    varTable[key] = vect;
  }

}

/**
 * Method that retrieves the vector containing all line numbers that contain the variable var.
 * @param key a string argument.
 * @return a vector<int> object.
 */
vector<int> VarTable::get(string key) {
  //return the vector containing all line numbers that the variable var.
  //if not present in varTable, return an empty vector.
  vector<int> emptyVector;
  unordered_map<string, vector<int>> varTable = getVarTable();
  if (varTable.find(key) == varTable.end()) {
    return emptyVector;
  }
  vector<int> vect = varTable.at(key);

  return vect;
}

void VarTable::setVarTable(unordered_map<string, vector<int>> table) {
  m_varTable = table;
}

unordered_map<string, vector<int>> VarTable::getVarTable() {
  return m_varTable;
}

/**
 * A constructor.
 * Instantiates an unordered map (hashmap) of variables to vector of line numbers associated.
 */
VarTable::VarTable() {
  unordered_map<string, vector<int>> m_varTable;
}




