#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

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
  VarTable* insert(VarTable* table, string key, int lineNum);
  vector<int> get(string var);
  void setVarTable(unordered_map<std::string, vector<int>>& table);
  unordered_map<string, vector<int>> getVarTable();
  VarTable();

private:
  unordered_map<string, vector<int>> m_varTable;
};
