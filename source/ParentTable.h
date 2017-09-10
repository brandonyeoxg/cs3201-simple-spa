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
  void setParentStarMap(std::unordered_map<int, std::list<std::list<int>>> &map);
  void setParentedByStarMap(std::unordered_map<int, std::list<int>> &map);
  std::unordered_map<int, std::vector<int>> getChildMap();
  std::unordered_map<int, int> getParentMap();
  std::unordered_map<int, std::list<std::list<int>>> getParentStarMap();
  std::unordered_map<int, std::list<int>> getParentedByStarMap();

  ParentTable();

  bool insertParent(int s1, int s2);
  bool isParent(int s1, int s2);
  bool isParentStar(int s1, int s2);
  int getParentOf(int s2);
  std::vector<int> getChildrenOf(int s1);
  std::vector<int> getParentStarOf(int s2);
  std::vector<int> getChildrenStarOf(int s1);
  std::unordered_map<int, std::vector<int>> getAllParents();
  void populateParentStarMap(std::unordered_map<int, int>::iterator t_mapItr);
  void populateParentedByStarMap(std::unordered_map<int, int>::iterator t_mapItr);

  
private:
  std::unordered_map<int, int> m_parentMap; //every statement can only have 1 parent.
  std::unordered_map<int, std::vector<int>> m_childMap;
  std::unordered_map<int, std::list<std::list<int>>> m_parentStarMap;
  std::unordered_map<int, std::list<int>> m_parentedByStarMap;
};
