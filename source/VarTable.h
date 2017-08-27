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
  void insert(string key, int lineNum);
  vector<int> get(string var);
  VarTable();

protected:
  unordered_map<string, vector<int>> varTable;
};
