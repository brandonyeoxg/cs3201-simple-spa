#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

/**
* Represents the parent relationship table, which is a vector of unordered_maps to
* track which line is nested within which lines.
*
* @author pengcheng
* @date 8/27/2017
*
*/
class ParentTable {
public:
  ParentTable* insert(ParentTable* table, int s1, int s2);
  std::vector<int> getS1(int s2);
  std::vector<int> getS2(int s1);
  void setParentTable(std::unordered_map<int, std::vector<int>> table);
  std::unordered_map<int, std::vector<int>> getParentTable();
  ParentTable();

protected:
  std::unordered_map<int, std::vector<int>> m_parentTable;
};
