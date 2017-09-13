#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <stdexcept>

#include "VarTable.h"

std::unordered_map<int, VarRelations> VarTable::getVarTable() {
  return m_varTable;
}

/**
 * A constructor.
 * Instantiates an unordered map (hashmap) of variables to vector of line numbers associated.
 */
VarTable::VarTable() {
  int m_index = 1;
  std::unordered_map<int, VarRelations> m_varTable;
  std::set<std::string> m_allVariables;
}

int VarTable::insertUsesForStmt(std::string t_varName, int t_lineNum) {
  int index = 0;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    index = it->first;
    VarRelations var = it->second;

    if (var.getVarName() == t_varName) {
      std::vector<int> uses = var.getUses();
      if (std::find(uses.begin(), uses.end(), t_lineNum) != uses.end()) {
        return index;
      }
      var.insertUses(t_lineNum);
      m_varTable[index] = var;
      m_allVariables.insert(t_varName);
      return index;
    }
  }
  //if dont have,
  VarRelations varRelations;
  varRelations.setVarName(t_varName);
  varRelations.insertUses(t_lineNum);
  index = m_varTable.size();
  m_varTable.emplace(m_varTable.size(), varRelations);
  m_allVariables.insert(t_varName);
  return index;
}

int VarTable::insertModifiesForStmt(std::string t_varName, int t_lineNum) {
  int index = 0;
  VarRelations var;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    index = it->first;
    var = it->second;
    if (var.getVarName() == t_varName) {
      std::vector<int> modifies = var.getModifies();
      if (std::find(modifies.begin(), modifies.end(), t_lineNum) != modifies.end()) {
        return index;
      }
      var.insertModifies(t_lineNum);
      m_varTable[index] = var;
      m_allVariables.insert(t_varName);
      return index;
    }
  }
  VarRelations varRelations;
  varRelations.setVarName(t_varName);
  varRelations.insertModifies(t_lineNum);
  index = m_varTable.size();
  m_varTable.emplace(index, varRelations);
  m_allVariables.insert(t_varName);
  return index;
}

bool VarTable::isModifies(int t_lineNum, std::string t_varName) {
  //for every index check if varName exists.
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    VarRelations var = it->second;
    if (var.getVarName() == t_varName) {
      //return true if lineNum can be found in vector modifies.
      std::vector<int> modifies = var.getModifies();
      if (std::find(modifies.begin(), modifies.end(), t_lineNum) != modifies.end()) {
        return true;
      } else {
        return false;
      }
    }
  }

  //if cannot find varName after all indices, return false
  return false;
}

bool VarTable::isUses(int t_lineNum, std::string t_varName) {
  //for every index check if varName exists.
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    VarRelations var = it->second;
    if (var.getVarName() == t_varName) {
      //return true if lineNum can be found in vector modifies.
      std::vector<int> uses = var.getUses();
      if (std::find(uses.begin(), uses.end(), t_lineNum) != uses.end()) {
        return true;
      } else {
        return false;
      }
    }
  }

  //if cannot find varName after all indices, return false
  return false;
}

std::vector<std::string> VarTable::getModifies(int t_lineNum) {
  //for every index, if lineNum present, append to vector.
  std::vector<int> modifies;
  std::vector<std::string> result;
  VarRelations var;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    var = it->second;
    modifies = var.getModifies();
    if (std::find(modifies.begin(), modifies.end(), t_lineNum) != modifies.end()) {
      result.push_back(var.getVarName());
    }
  }

  return result;
}

std::vector<std::string> VarTable::getUses(int t_lineNum) {
  //for every index, if lineNum present, append to vector.
  std::vector<int> uses;
  std::vector<std::string> result;
  VarRelations var;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    var = it->second;
    uses = var.getUses();
    if (std::find(uses.begin(), uses.end(), t_lineNum) != uses.end()) {
      result.push_back(var.getVarName());
    }
  }

  return result;
}

std::vector<int> VarTable::getStmtModifies(std::string t_varName) {
  std::vector<int> emptyResult;
  VarRelations var;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    var = it->second;
    if (t_varName == var.getVarName()) {
      return var.getModifies();
    }
  }
  return emptyResult;
}

std::vector<int> VarTable::getStmtUses(std::string t_varName) {
  std::vector<int> emptyResult;
  VarRelations var;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    var = it->second;
    if (t_varName == var.getVarName()) {
      return var.getUses();
    }
  }
  return emptyResult;
}

std::unordered_map<std::string, std::vector<int>> VarTable::getAllStmtModifies() {
  std::unordered_map<std::string, std::vector<int>> result;
  VarRelations var;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    var = it->second;
    result.emplace(var.getVarName(), var.getModifies());
  }
  return result;
}

std::unordered_map<std::string, std::vector<int>> VarTable::getAllStmtUses() {
  std::unordered_map<std::string, std::vector<int>> result;
  VarRelations var;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    var = it->second;
    result.emplace(var.getVarName(), var.getUses());
  }
  return result;
}

int VarTable::getIndexOfVar(std::string t_varName) {
  VarRelations var;
  int index;
  bool isFound = false;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    var = it->second;
    index = it->first;
    if (var.getVarName() == t_varName) {
      isFound = true;
      return index;
    }
  }
  //if not found, throw invalid_argument exception.
  if (isFound == false) {
    throw std::invalid_argument("variable name does not exist in varTable");
  }
}

std::string VarTable::getVarNameFromIndex(int t_index) {
  if (t_index > m_varTable.size()) {
    throw std::invalid_argument("var Index does not exist.");
  } else {
    VarRelations varRelation = m_varTable.find(t_index)->second;
    return varRelation.getVarName();
  }
}
bool VarTable::isModifiesAnything(int t_line_num) {
  VarRelations varRelations;
  std::vector<int> lineNums;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    varRelations = it->second;
    lineNums = varRelations.getModifies();
    if (std::find(lineNums.begin(), lineNums.end(), t_line_num) != lineNums.end()) {
      return true;
    }
  }
  return false;
}

bool VarTable::isUsesAnything(int t_line_num) {
  VarRelations varRelations;
  std::vector<int> lineNums;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    varRelations = it->second;
    lineNums = varRelations.getUses();
    if (std::find(lineNums.begin(), lineNums.end(), t_line_num) != lineNums.end()) {
      return true;
    }
  }
  return false;
}

std::vector<int> VarTable::getStmtModifiesAnything() {
  VarRelations var;
  std::set<int> resultSet;
  std::vector<int> modifies;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    var = it->second;
    modifies = var.getModifies();
    for (int i = 0; i < modifies.size(); i++) {
      resultSet.insert(modifies[i]);
    }
  }
  std::vector<int> result;
  result.assign(resultSet.begin(), resultSet.end());

  return result;
}

std::vector<int> VarTable::getStmtUsesAnything() {
  VarRelations var;
  std::set<int> resultSet;
  std::vector<int> uses;
  for (auto it = m_varTable.begin(); it != m_varTable.end(); ++it) {
    var = it->second;
    uses = var.getUses();
    for (int i = 0; i < uses.size(); i++) {
      resultSet.insert(uses[i]);
    }
  }
  std::vector<int> result;
  result.assign(resultSet.begin(), resultSet.end());

  return result;
}

std::vector<std::string> VarTable::getAllVariables() {
  std::vector<std::string> result;
  result.assign(m_allVariables.begin(), m_allVariables.end());

  return result;
}