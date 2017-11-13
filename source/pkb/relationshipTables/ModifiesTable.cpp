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

void ModifiesTable::insertModifiesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum, VAR_INDEX t_varIdx) {
  BOOLEAN inserted = false;
  //if var name already exists in stmtVarMap, check if vector has lineNum. if so, return index (0).
  auto iterator = m_modifiesVarMap.find(t_varName);
  auto itr = m_modifiesVarByIdxMap.find(t_varIdx);
  if (iterator != m_modifiesVarMap.end()) {
    LIST_OF_STMT_NUMS vector = iterator->second;
    if (std::find(vector.begin(), vector.end(), t_lineNum) == vector.end()) {
      //if cannot find in vector, it means it's a valid insertion. Enter to both maps.
      vector.push_back(t_lineNum);
      m_modifiesVarMap[t_varName] = vector;
      m_modifiesVarByIdxMap[t_varIdx] = vector;
      insertToModifiesStmtMap(t_lineNum, t_varName, t_varIdx);
      //insert into sets
      m_allStmtNumsModified.insert(t_lineNum);
      inserted = true;
    }
  }
  if (inserted == false) {
    //not found in modifiesVarMap, new insertion to the map.
    LIST_OF_STMT_NUMS newVector;
    newVector.push_back(t_lineNum);
    m_modifiesVarMap.emplace(t_varName, newVector);
    m_modifiesVarByIdxMap.emplace(t_varIdx, newVector);
    insertToModifiesStmtMap(t_lineNum, t_varName, t_varIdx);
    //insert into sets
    m_allStmtNumsModified.insert(t_lineNum);
  }
}

//insertion method to the 2-way map.
void ModifiesTable::insertToModifiesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName, VAR_INDEX t_varIdx) {
  BOOLEAN inserted = false;
  //check if lineNum exists as key
  auto iterator = m_modifiesStmtMap.find(t_lineNum);
  auto itr = m_modifiesStmtByIdxMap.find(t_lineNum);
  if (iterator != m_modifiesStmtMap.end()) {
    //if have, check if varname exists in vector. if does, return false
    LIST_OF_VAR_NAMES vector = iterator->second;
    LIST_OF_VAR_INDICES varIndices = itr->second;
    UNORDERED_SET_OF_NAMES varNamesSet(vector.begin(), vector.end());
    UNORDERED_SET_OF_VAR_INDICES varIdxSet(varIndices.begin(), varIndices.end());
    if (std::find(vector.begin(), vector.end(), t_varName) == vector.end()) {
      //else, valid insertion. Append varName to vector and replace in the map.
      vector.push_back(t_varName);
      varIndices.push_back(t_varIdx);
      varNamesSet.insert(t_varName);
      varIdxSet.insert(t_varIdx);
      m_modifiesStmtMap[t_lineNum] = vector;
      m_modifiesStmtByIdxMap[t_lineNum] = varIndices;
      m_modifiesStmtSet[t_lineNum] = varNamesSet;
      m_modifiesStmtByIdxSet[t_lineNum] = varIdxSet;
      inserted = true;
    }
  }
  if (inserted == false) {
    //if lineNum does not exist... create new vector and emplace
    LIST_OF_VAR_NAMES newVector;
    LIST_OF_VAR_INDICES newIndices;
    UNORDERED_SET_OF_NAMES newVarNamesSet;
    UNORDERED_SET_OF_VAR_INDICES newVarIdxSet;
    newVector.push_back(t_varName);
    newIndices.push_back(t_varIdx);
    newVarNamesSet.insert(t_varName);
    newVarIdxSet.insert(t_varIdx);
    m_modifiesStmtMap.emplace(t_lineNum, newVector);
    m_modifiesStmtByIdxMap.emplace(t_lineNum, newIndices);
    m_modifiesStmtSet.emplace(t_lineNum, newVarNamesSet);
    m_modifiesStmtByIdxSet.emplace(t_lineNum, newVarIdxSet);
  }
}

bool ModifiesTable::isModifies(STMT_NUM t_lineNum, VAR_NAME t_varName) {
  auto itr = m_modifiesStmtSet.find(t_lineNum);
  if (itr == m_modifiesStmtSet.end()) {
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

LIST_OF_VAR_INDICES ModifiesTable::getModifiesByIdx(STMT_NUM t_lineNum) {
  LIST_OF_VAR_INDICES vector;
  auto itr = m_modifiesStmtByIdxMap.find(t_lineNum);
  if (itr == m_modifiesStmtByIdxMap.end()) {
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

MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS ModifiesTable::getAllStmtModifiesByIdx() {
  return m_modifiesVarByIdxMap;
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
  return m_allStmtNumsModifiedList;
}

//Constructor.
ModifiesTable::ModifiesTable() {
}

MAP_OF_STMT_NUM_TO_LIST_OF_VAR_NAMES ModifiesTable::getModifiesStmtMap() {
  return m_modifiesStmtMap;
}

MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS ModifiesTable::getModifiesVarMap() {
  return m_modifiesVarMap;
}

void ModifiesTable::populateModifiesAnythingRelationships() {
  m_allStmtNumsModifiedList.assign(m_allStmtNumsModified.begin(), m_allStmtNumsModified.end());
}