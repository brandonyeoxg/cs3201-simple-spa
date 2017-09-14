#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>
#include <unordered_map>

class ConstantTable {
public:
  ConstantTable();
  int insertConstant(std::string t_constant);
  std::list<std::string> getAllConstants();

private:
  std::set<std::string> m_constantSet;
  std::unordered_map<int, std::string> m_constantMap;
};