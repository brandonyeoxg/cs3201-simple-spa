#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "VarTable.h"

/**
 * Method that inserts the line number to the unordered map of vectors containing the variable as key.
 * @param key a string argument.
 * @param lineNum an integer argument.
 */
VarTable* VarTable::insert(VarTable* table, std::string key, int lineNum) {
  std::unordered_map<std::string, std::vector<int>> varTable = table->getVarTable();
  if (varTable.find(key) == varTable.end()) {
    //if the key is not present in varTable
    std::vector<int> lineNums;
    lineNums.push_back(lineNum);
    varTable.emplace(key, lineNums);
  } else {
    //if not, retrieve the existing vector, append, and put back to varTable.
    std::vector<int> vect = varTable[key];
    vect.push_back(lineNum);
    varTable[key] = vect;
  } 
  table->setVarTable(varTable);
  return table;

}

/**
 * Method that retrieves the vector containing all line numbers that contain the variable var.
 * @param key a string argument.
 * @return a vector<int> object.
 */
std::vector<int> VarTable::get(std::string key) {
  //return the vector containing all line numbers that the variable var.
  //if not present in varTable, return an empty vector.
  std::vector<int> emptyVector;
  std::unordered_map<std::string, std::vector<int>> varTable = getVarTable();
  if (varTable.find(key) == varTable.end()) {
    return emptyVector;
  }
  std::vector<int> vect = varTable.at(key);

  return vect;
}

void VarTable::setVarTable(std::unordered_map<std::string, std::vector<int>>& table) {
  m_varTable = table;
}

std::unordered_map<std::string, std::vector<int>> VarTable::getVarTable() {
  return m_varTable;
}

/**
 * A constructor.
 * Instantiates an unordered map (hashmap) of variables to vector of line numbers associated.
 */
VarTable::VarTable() {
  std::unordered_map<std::string, std::vector<int>> m_varTable;
}




