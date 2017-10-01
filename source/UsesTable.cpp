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
      inserted = true;
    }
  }
  if (inserted == false) {
    //not found in usesVarMap, new insertion to the map.
    LIST_OF_STMT_NUMS newVector;
    newVector.push_back(t_lineNum);
    m_usesVarMap.emplace(t_varName, newVector);
    insertToUsesStmtMap(t_lineNum, t_varName);
  }
}

void UsesTable::insertToUsesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName) {
  bool inserted = false;
  //check if lineNum exists as key
  auto iterator = m_usesStmtMap.find(t_lineNum);
  if (iterator != m_usesStmtMap.end()) {
    //if have, check if varname exists in vector. if does, return false
    LIST_OF_VAR_NAMES vector = iterator->second;
    if (std::find(vector.begin(), vector.end(), t_varName) == vector.end()) {
      //else, valid insertion. Append varName to vector and replace in the map.
      vector.push_back(t_varName);
      m_usesStmtMap[t_lineNum] = vector;
      inserted = true;
    }
  } 
  if(inserted == false) {
    //if lineNum does not exist... create new vector and emplace
    LIST_OF_VAR_NAMES newVector;
    newVector.push_back(t_varName);
    m_usesStmtMap.emplace(t_lineNum, newVector);
  }
}

bool isUses(STMT_NUM t_lineNum, VAR_NAME t_varName);
LIST_OF_VAR_NAMES getUses(STMT_NUM t_line_num);
LIST_OF_STMT_NUMS getStmtUses(VAR_NAME t_varName);
std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> getAllStmtUses();
bool isUsesAnything(STMT_NUM t_line_num);  //uses(2, _)
LIST_OF_STMT_NUMS getStmtUsesAnything(); //uses(s, _)

UsesTable::UsesTable() {
  std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> m_usesStmtMap;
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> m_usesVarMap;
}

std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> UsesTable::getUsesStmtMap() {
  return m_usesStmtMap;
}

std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> UsesTable::getUsesVarMap() {
  return m_usesVarMap;
}