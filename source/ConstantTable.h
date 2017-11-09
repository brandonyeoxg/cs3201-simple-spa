#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"

class ConstantTable {
public:
  ConstantTable();
  int insertConstant(std::string t_constant);
  LIST_OF_RESULTS getAllConstants();
  LIST_OF_CONSTANT_INDICES getAllConstantsByIdx();
  STRING getConstantFromIdx(int t_constantIdx);
  CONSTANT_INDEX getConstantIdxFromConstant(CONSTANT_TERM t_constant);
private:
  std::set<std::string> m_constantSet;
  std::set<int> m_constantSetByIdx;
  std::unordered_map<int, std::string> m_constantMap;
  MAP_OF_CONSTANT_TERM_TO_CONSTANT_INDEX m_constantByIdxMap;
};