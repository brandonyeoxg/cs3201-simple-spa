#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

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
  void insert(int s1, int s2);
  vector<int> getS1(int s2);
  vector<int> getS2(int s1);
  void setFollowTable(unordered_map<int, vector<int>> table);
  unordered_map<int, vector<int>> getFollowTable();
  FollowTable();

private:
  unordered_map<int, vector<int>> m_followTable;
};