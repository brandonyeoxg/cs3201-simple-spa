#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <stdexcept>
#include <algorithm>

#include "UsesTable.h"

void UsesTable::insertUsesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum) {
  bool inserted = false;
  //if var name already exists in stmtVarMap, check if vector has lineNum. if so, return index (0).
  auto iterator = m_usesVarMap.find(t_varName);
  if (iterator != m_usesVarMap.end()) {
    LIST_OF_STMT_NUMS vector = iterator->second;
    if (std::find(vector.begin(), vector.end(), t_lineNum) == vector.end()) {
      //if cannot find in vector, it means it's a valid insertion. Enter to both maps.
      vector.push_back(t_lineNum);
      m_usesVarMap[t_varName] = vector;
      insertToUsesStmtMap(t_lineNum, t_varName);
      //insert into sets
      m_allVariablesUsed.insert(t_varName);
      m_allStmtNumsUsed.insert(t_lineNum);
      inserted = true;
    }
  }
  if (inserted == false) {
    //not found in usesVarMap, new insertion to the map.
    LIST_OF_STMT_NUMS newVector;
    newVector.push_back(t_lineNum);
    m_usesVarMap.emplace(t_varName, newVector);
    insertToUsesStmtMap(t_lineNum, t_varName);
    //insert into sets
    m_allVariablesUsed.insert(t_varName);
    m_allStmtNumsUsed.insert(t_lineNum);
  }
}

//insertion method to the 2-way map.
void UsesTable::insertToUsesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName) {
  bool inserted = false;
  //check if lineNum exists as key
  auto iterator = m_usesStmtMap.find(t_lineNum);
  if (iterator != m_usesStmtMap.end()) {
    //if have, check if varname exists in vector. if does, return false
    LIST_OF_VAR_NAMES vector = iterator->second;
    UNORDERED_SET_OF_NAMES varNamesSet(vector.begin(), vector.end());
    if (std::find(vector.begin(), vector.end(), t_varName) == vector.end()) {
      //else, valid insertion. Append varName to vector and replace in the map.
      vector.push_back(t_varName);
      varNamesSet.insert(t_varName);
      m_usesStmtMap[t_lineNum] = vector;
      m_usesStmtSet[t_lineNum] = varNamesSet;
      inserted = true;
    }
  } 
  if(inserted == false) {
    //if lineNum does not exist... create new vector and emplace
    LIST_OF_VAR_NAMES newVector;
    UNORDERED_SET_OF_NAMES newVarNamesSet;
    newVector.push_back(t_varName);
    newVarNamesSet.insert(t_varName);
    m_usesStmtMap.emplace(t_lineNum, newVector);
    m_usesStmtSet.emplace(t_lineNum, newVarNamesSet);
  }
}

bool UsesTable::isUses(STMT_NUM t_lineNum, VAR_NAME t_varName) {
  /*
  //search usesStmtMap (reason: int vs string comparison
  auto itr = m_usesStmtMap.find(t_lineNum);
  if (itr == m_usesStmtMap.end()) {
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
  */
  auto itr = m_usesStmtSet.find(t_lineNum);
  if (itr == m_usesStmtSet.end()) {
    return false;
  } else {
    //check if varName appears in vector
    UNORDERED_SET_OF_NAMES varNamesSet = itr->second;
    if (varNamesSet.find(t_varName) != varNamesSet.end()) {
      return true;
    } else {
      return false;
    }
  }
}

LIST_OF_VAR_NAMES UsesTable::getUses(STMT_NUM t_lineNum) {
  LIST_OF_VAR_NAMES vector;
  auto itr = m_usesStmtMap.find(t_lineNum);
  if (itr == m_usesStmtMap.end()) {
    return vector;  //empty, no results
  } else {
    vector = itr->second;
    return vector;
  }

}

LIST_OF_STMT_NUMS UsesTable::getStmtUses(VAR_NAME t_varName) {
  LIST_OF_STMT_NUMS vector;
  auto itr = m_usesVarMap.find(t_varName);
  if (itr == m_usesVarMap.end()) {
    return vector;
  } else {
    vector = itr->second;
    return vector;
  }
}
std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> UsesTable::getAllStmtUses() {
  return m_usesVarMap;
}

bool UsesTable::isUsesAnything(STMT_NUM t_lineNum) {
  auto itr = m_usesStmtMap.find(t_lineNum);
  if (itr == m_usesStmtMap.end()) {
    return false;
  } else {
    return true;
  }

}
LIST_OF_STMT_NUMS UsesTable::getStmtUsesAnything() {
  LIST_OF_STMT_NUMS results;
  results.assign(m_allStmtNumsUsed.begin(), m_allStmtNumsUsed.end());
  return results;
}

LIST_OF_VAR_NAMES UsesTable::getAllUsesVarNames() {
  LIST_OF_VAR_NAMES results;
  results.assign(m_allVariablesUsed.begin(), m_allVariablesUsed.end());
  return results;
}

//Constructor.
UsesTable::UsesTable() {
  std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> m_usesStmtMap;
  MAP_OF_STMT_NUM_TO_SET_OF_NAMES m_usesStmtSet;
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> m_usesVarMap;
  SET_OF_VAR_NAMES m_allVariablesUsed;
  SET_OF_STMT_NUMS m_allStmtNumsUsed;
}

std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> UsesTable::getUsesStmtMap() {
  return m_usesStmtMap;
}

std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> UsesTable::getUsesVarMap() {
  return m_usesVarMap;
}