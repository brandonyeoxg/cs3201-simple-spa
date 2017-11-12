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

void UsesTable::insertUsesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum, VAR_INDEX t_varIdx) {
  BOOLEAN inserted = false;
  //if var name already exists in stmtVarMap, check if vector has lineNum. if so, return index (0).
  auto iterator = m_usesVarMap.find(t_varName); //old imp.
  auto itr = m_usesVarByIdxMap.find(t_varIdx);
  if (itr != m_usesVarByIdxMap.end()) {
    LIST_OF_STMT_NUMS vector = itr->second;
    if (std::find(vector.begin(), vector.end(), t_lineNum) == vector.end()) {
      //if cannot find in vector, it means it's a valid insertion. Enter to both maps.
      vector.push_back(t_lineNum);
      m_usesVarMap[t_varName] = vector; //old imp.
      m_usesVarByIdxMap[t_varIdx] = vector;
      insertToUsesStmtMap(t_lineNum, t_varName, t_varIdx);
      //insert into sets
      m_allVariablesUsed.insert(t_varName); //old imp.
      m_allVariablesUsedByIdx.insert(t_varIdx);
      m_allStmtNumsUsed.insert(t_lineNum);
      inserted = true;
    }
  }
  if (inserted == false) {
    //not found in usesVarMap, new insertion to the map.
    LIST_OF_STMT_NUMS newVector;
    newVector.push_back(t_lineNum);
    m_usesVarMap.emplace(t_varName, newVector); //old imp.
    m_usesVarByIdxMap.emplace(t_varIdx, newVector);
    insertToUsesStmtMap(t_lineNum, t_varName, t_varIdx);
    //insert into sets
    m_allVariablesUsed.insert(t_varName); //old imp.
    m_allVariablesUsedByIdx.insert(t_varIdx);
    m_allStmtNumsUsed.insert(t_lineNum);
  }
}

//insertion method to the 2-way map.
void UsesTable::insertToUsesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName, VAR_INDEX t_varIdx) {
  BOOLEAN inserted = false;
  //check if lineNum exists as key
  auto iterator = m_usesStmtMap.find(t_lineNum);  //old imp.
  auto itr = m_usesStmtByIdxMap.find(t_lineNum);
  if (itr != m_usesStmtByIdxMap.end()) {
    //if have, check if varname exists in vector. if does, return false
    LIST_OF_VAR_NAMES vector = iterator->second;  //old imp.
    LIST_OF_VAR_INDICES varIndices = itr->second;
    UNORDERED_SET_OF_NAMES varNamesSet(vector.begin(), vector.end()); //old imp.
    UNORDERED_SET_OF_VAR_INDICES varIdxSet(varIndices.begin(), varIndices.end()); 
    if (std::find(vector.begin(), vector.end(), t_varName) == vector.end()) {
      //else, valid insertion. Append varName to vector and replace in the map.
      vector.push_back(t_varName); //old imp.
      varIndices.push_back(t_varIdx);
      varNamesSet.insert(t_varName); //old imp.
      varIdxSet.insert(t_varIdx);
      m_usesStmtMap[t_lineNum] = vector; //old imp.
      m_usesStmtByIdxMap[t_lineNum] = varIndices;
      m_usesStmtSet[t_lineNum] = varNamesSet; //old imp.
      m_usesStmtByIdxSet[t_lineNum] = varIdxSet;
      inserted = true;
    }
  } 
  if(inserted == false) {
    //if lineNum does not exist... create new vector and emplace
    LIST_OF_VAR_NAMES newVector; //old imp.
    LIST_OF_VAR_INDICES newIndices;
    UNORDERED_SET_OF_NAMES newVarNamesSet; //old imp.
    UNORDERED_SET_OF_VAR_INDICES newVarIdxSet;
    newVector.push_back(t_varName); //old imp.
    newIndices.push_back(t_varIdx);
    newVarNamesSet.insert(t_varName); //old imp.
    newVarIdxSet.insert(t_varIdx);
    m_usesStmtMap.emplace(t_lineNum, newVector);  //old imp.
    m_usesStmtByIdxMap.emplace(t_lineNum, newIndices);

    m_usesStmtSet.emplace(t_lineNum, newVarNamesSet); //old imp.
    m_usesStmtByIdxSet.emplace(t_lineNum, newVarIdxSet);
  }
}

bool UsesTable::isUses(STMT_NUM t_lineNum, VAR_NAME t_varName) {
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

LIST_OF_VAR_INDICES UsesTable::getUsesByIdx(STMT_NUM t_lineNum) {
  LIST_OF_VAR_INDICES vector;
  auto itr = m_usesStmtByIdxMap.find(t_lineNum);
  if (itr == m_usesStmtByIdxMap.end()) {
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
MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS UsesTable::getAllStmtUses() {
  return m_usesVarMap;
}

MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS UsesTable::getAllStmtUsesByIdx() {
  return m_usesVarByIdxMap;
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
  return m_allStmtNumsUsedList;
}

LIST_OF_VAR_NAMES UsesTable::getAllUsesVarNames() { //obsolete
  LIST_OF_VAR_NAMES results;
  results.assign(m_allVariablesUsed.begin(), m_allVariablesUsed.end());
  return results;
}

LIST_OF_VAR_INDICES UsesTable::getAllUsesVarNamesByIdx() {
  return m_allVariablesUsedList;
}

UsesTable::UsesTable() {}

MAP_OF_STMT_NUM_TO_LIST_OF_VAR_NAMES UsesTable::getUsesStmtMap() {
  return m_usesStmtMap;
}

MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS UsesTable::getUsesVarMap() {
  return m_usesVarMap;
}

void UsesTable::populateUsesAnythingRelationships() {
  m_allStmtNumsUsedList.assign(m_allStmtNumsUsed.begin(), m_allStmtNumsUsed.end());
  m_allVariablesUsedList.assign(m_allVariablesUsedByIdx.begin(), m_allVariablesUsedByIdx.end());
}