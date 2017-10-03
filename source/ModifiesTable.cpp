#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <stdexcept>
#include <algorithm>

#include "ModifiesTable.h"

void ModifiesTable::insertModifiesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum) {
  bool inserted = false;
  //if var name already exists in stmtVarMap, check if vector has lineNum. if so, return index (0).
  auto iterator = m_modifiesVarMap.find(t_varName);
  if (iterator != m_modifiesVarMap.end()) {
    LIST_OF_STMT_NUMS vector = iterator->second;
    if (std::find(vector.begin(), vector.end(), t_lineNum) == vector.end()) {
      //if cannot find in vector, it means it's a valid insertion. Enter to both maps.
      vector.push_back(t_lineNum);
      m_modifiesVarMap[t_varName] = vector;
      insertToModifiesStmtMap(t_lineNum, t_varName);
      //insert into sets
      m_allVariablesModified.insert(t_varName);
      m_allStmtNumsModified.insert(t_lineNum);
      inserted = true;
    }
  }
  if (inserted == false) {
    //not found in modifiesVarMap, new insertion to the map.
    LIST_OF_STMT_NUMS newVector;
    newVector.push_back(t_lineNum);
    m_modifiesVarMap.emplace(t_varName, newVector);
    insertToModifiesStmtMap(t_lineNum, t_varName);
    //insert into sets
    m_allVariablesModified.insert(t_varName);
    m_allStmtNumsModified.insert(t_lineNum);
  }
}

//insertion method to the 2-way map.
void ModifiesTable::insertToModifiesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName) {
  bool inserted = false;
  //check if lineNum exists as key
  auto iterator = m_modifiesStmtMap.find(t_lineNum);
  if (iterator != m_modifiesStmtMap.end()) {
    //if have, check if varname exists in vector. if does, return false
    LIST_OF_VAR_NAMES vector = iterator->second;
    if (std::find(vector.begin(), vector.end(), t_varName) == vector.end()) {
      //else, valid insertion. Append varName to vector and replace in the map.
      vector.push_back(t_varName);
      m_modifiesStmtMap[t_lineNum] = vector;
      inserted = true;
    }
  }
  if (inserted == false) {
    //if lineNum does not exist... create new vector and emplace
    LIST_OF_VAR_NAMES newVector;
    newVector.push_back(t_varName);
    m_modifiesStmtMap.emplace(t_lineNum, newVector);
  }
}

bool ModifiesTable::isModifies(STMT_NUM t_lineNum, VAR_NAME t_varName) {
  //search modifiesStmtMap (reason: int vs string comparison
  auto itr = m_modifiesStmtMap.find(t_lineNum);
  if (itr == m_modifiesStmtMap.end()) {
    return false;
  } else {
    //check if varName appears in vector
    LIST_OF_VAR_NAMES vector = itr->second;
    if (std::find(vector.begin(), vector.end(), t_varName) != vector.end()) {
      return true;
    } else {
      return false;
    }
  }
}

LIST_OF_VAR_NAMES ModifiesTable::getModifies(STMT_NUM t_lineNum) {
  LIST_OF_VAR_NAMES vector;
  auto itr = m_modifiesStmtMap.find(t_lineNum);
  if (itr == m_modifiesStmtMap.end()) {
    return vector;  //empty, no results
  } else {
    vector = itr->second;
    return vector;
  }

}

LIST_OF_STMT_NUMS ModifiesTable::getStmtModifies(VAR_NAME t_varName) {
  LIST_OF_STMT_NUMS vector;
  auto itr = m_modifiesVarMap.find(t_varName);
  if (itr == m_modifiesVarMap.end()) {
    return vector;
  } else {
    vector = itr->second;
    return vector;
  }
}
std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> ModifiesTable::getAllStmtModifies() {
  return m_modifiesVarMap;
}

bool ModifiesTable::isModifiesAnything(STMT_NUM t_lineNum) {
  auto itr = m_modifiesStmtMap.find(t_lineNum);
  if (itr == m_modifiesStmtMap.end()) {
    return false;
  } else {
    return true;
  }

}
LIST_OF_STMT_NUMS ModifiesTable::getStmtModifiesAnything() {
  LIST_OF_STMT_NUMS results;
  results.assign(m_allStmtNumsModified.begin(), m_allStmtNumsModified.end());
  return results;
}

LIST_OF_VAR_NAMES ModifiesTable::getAllModifiesVarNames() {
  LIST_OF_VAR_NAMES results;
  results.assign(m_allVariablesModified.begin(), m_allVariablesModified.end());
  return results;
}

//Constructor.
ModifiesTable::ModifiesTable() {
  std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> m_modifiesStmtMap;
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> m_modifiesVarMap;
  SET_OF_VAR_NAMES m_allVariablesModified;
  SET_OF_STMT_NUMS m_allStmtNumsModified;
}

std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> ModifiesTable::getModifiesStmtMap() {
  return m_modifiesStmtMap;
}

std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> ModifiesTable::getModifiesVarMap() {
  return m_modifiesVarMap;
}