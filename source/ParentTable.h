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
  void setChildMap(std::unordered_map<int, std::vector<int>> &t_map);
  void setParentMap(std::unordered_map<int, int> &t_map);
  void setParentStarMap(std::unordered_map<int, std::vector<int>> &t_map);
  void setParentedByStarMap(std::unordered_map<int, std::vector<int>> &t_map);
  std::unordered_map<int, std::vector<int>> getChildMap();
  std::unordered_map<int, int> getParentMap();
  std::unordered_map<int, std::vector<int>> getParentStarMap();
  std::unordered_map<int, std::vector<int>> getParentedByStarMap();

  ParentTable();

  bool insertParent(int t_s1, int t_s2);
  bool isParent(int t_s1, int t_s2);
  bool isParentStar(int t_s1, int t_s2);
  int getParentOf(int t_s2);
  std::vector<int> getChildrenOf(int t_s1);
  std::vector<int> getParentStarOf(int t_s2);
  std::vector<int> getChildrenStarOf(int t_s1);
  std::unordered_map<int, std::vector<int>> getAllParents();
  void populateParentStarMap();
  void populateParentedByStarMap(std::unordered_map<int, int>::iterator t_mapItr);
  std::vector<int> getChildrenOfAnything(); //parent(_, s2)
  std::vector<int> getParentOfAnything(); //parent(s1, _)
  std::vector<int> getChildrenStarOfAnything(); //parent*(_, s2)
  std::vector<int> getParentStarOfAnything(); //parent*(s1, _)
  bool hasParentRelationship(); //Parent (_, _)
  bool hasParentStarRelationship(); //Parent* (_, _)
  bool isChildrenOfAnything(int t_s2);  //parent(_, 2)
  bool isParentOfAnything(int t_s1);  //parent(2, _)
  bool isChildrenOfStarAnything(int t_s2);  //parent*(_, 2)
  bool isParentOfStarAnything(int t_s1);  //parent*(2, _)
  
private:
  std::unordered_map<int, int> m_parentMap; //every statement can only have 1 parent.
  std::unordered_map<int, std::vector<int>> m_childMap;
  std::unordered_map<int, std::vector<int>> m_parentStarMap;
  std::unordered_map<int, std::vector<int>> m_parentedByStarMap;
};
