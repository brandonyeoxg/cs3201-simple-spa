#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "VarTable.h"
/*

 * Method that inserts the line number to the unordered map of vectors containing the variable as key.
 * @param key a string argument.
 * @param lineNum an integer argument.
 
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


 * Method that retrieves the vector containing all line numbers that contain the variable var.
 * @param key a string argument.
 * @return a vector<int> object.
 
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

*/

/**
 * A constructor.
 * Instantiates an unordered map (hashmap) of variables to vector of line numbers associated.
 */
VarTable::VarTable() {
  int m_index = 1;
  std::unordered_map<int, VarRelations> m_varTable;
}

int VarTable::insertUsesForStmt(int index, std::string varName, int lineNum) {
  //for every key, search varRelation to check if varName exists.
  //if have, append lineNum to uses vector.
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    int index = it->first;
    VarRelations var = it->second;
    if (var.getVarName() == varName) {
      //check if lineNum exists in vector uses. If it is, throw invalid_argument exception.
      std::vector<int> uses = var.getUses();
      if (std::find(uses.begin(), uses.end(), lineNum) != uses.end()) {
        throw std::invalid_argument("lineNum for variable already exists in varTable");
      } else {
        var.insertUses(lineNum);
        return index;
      }
    }
  }
  //if dont have,
  VarRelations varRelations;
  varRelations.setVarName(varName);
  varRelations.insertUses(lineNum);
  m_varTable.emplace(index, varRelations);
  return index;
}

int VarTable::insertModifiesForStmt(int index, std::string varName, int lineNum) {
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    int index = it->first;
    VarRelations var = it->second;
    if (var.getVarName() == varName) {
      std::vector<int> modifies = var.getModifies();
      if (std::find(modifies.begin(), modifies.end(), lineNum) != modifies.end()) {
        throw std::invalid_argument("lineNum for variable already exists in varTable");
      } else {
        var.insertModifies(lineNum);
        return index;
      }
    }
  }

  VarRelations varRelations;
  varRelations.setVarName(varName);
  varRelations.insertModifies(lineNum);
  m_varTable.emplace(index, varRelations);
  return index;
}


std::unordered_map<int, VarRelations> VarTable::getVarTable() {
  return m_varTable;
}



