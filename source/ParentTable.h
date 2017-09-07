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
  //ParentTable* insert(ParentTable* table, int s1, int s2);
  //std::vector<int> getS1(int s2);
  //std::vector<int> getS2(int s1);

  void setChildMap(std::unordered_map<int, std::vector<int>> &map);
  void setParentMap(std::unordered_map<int, int> &map);
  std::unordered_map<int, std::vector<int>> getChildMap();
  std::unordered_map<int, int> getParentMap();
  ParentTable();

  bool insertParent(int s1, int s2);
  bool isParent(int s1, int s2);
  bool isParentStar(int s1, int s2);
  int getParentOf(int s2);
  std::vector<int> getChildrenOf(int s1);
  std::vector<int> getParentStarOf(int s2);
protected:
  std::unordered_map<int, int> m_parentMap; //every statement can only have 1 parent.
  std::unordered_map<int, std::vector<int>> m_childMap;

};
