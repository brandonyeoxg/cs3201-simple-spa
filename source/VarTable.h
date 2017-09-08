#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "VarRelations.h"

/**
* Represents the variable table, which is a vector of unordered_maps to 
* track which variable appear on which line.
*
* @author pengcheng
* @date 8/27/2017
*
*/
class VarTable {
public:
  //old methods
  VarTable* insert(VarTable* table, std::string key, int lineNum);
  std::vector<int> get(std::string var);
  void setVarTable(std::unordered_map<std::string, std::vector<int>>& table);

  std::unordered_map<int, VarRelations> getVarTable();

  VarTable();

  int insertUsesForStmt(int index, std::string varName, int lineNum);
  int insertModifiesForStmt(int index, std::string varName, int lineNum);
  bool isModifies(int lineNum, std::string varName);
  bool isUses(int lineNum, std::string varName);
  std::vector<std::string> getModifies(int lineNum);
  std::vector<std::string> getUses(int line_num);
  std::vector<int> getStmtModifies(std::string varName);
  std::vector<int> getStmtUses(std::string varName);
  std::unordered_map<std::string, std::vector<int>> getAllStmtModifies();
  std::unordered_map<std::string, std::vector<int>> getAllStmtUses();

private:
  std::unordered_map<int, VarRelations> m_varTable;
};
