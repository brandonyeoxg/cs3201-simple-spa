#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

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
  VarTable* insert(VarTable* table, std::string key, int lineNum);
  std::vector<int> get(std::string var);
  void setVarTable(std::unordered_map<std::string, std::vector<int>>& table);
  std::unordered_map<std::string, std::vector<int>> getVarTable();
  VarTable();

private:
  std::unordered_map<std::string, std::vector<int>> m_varTable;
};
