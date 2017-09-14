#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

/**
* Represents the follow relationship table, which is a vector of unordered_maps to
* track which line follows which lines.
*
* @author pengcheng
* @date 8/27/2017
*
*/
class FollowTable {
public:
  FollowTable* insert(FollowTable* table, int s1, int s2);
  std::vector<int> getS1(int s2);
  std::vector<int> getS2(int s1);
  void setFollowTable(std::unordered_map<int, std::vector<int>> table);
  std::unordered_map<int, std::vector<int>> getFollowTable();
  FollowTable();

private:
  std::unordered_map<int, std::vector<int>> m_followTable;
};