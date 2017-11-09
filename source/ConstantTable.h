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
  ~ConstantTable() {};
  CONSTANT_INDEX insertConstant(std::string t_constant);
  LIST_OF_RESULTS getAllConstants();
  LIST_OF_CONSTANT_INDICES getAllConstantsByIdx();
  CONSTANT_TERM getConstantFromIdx(int t_constantIdx);
private:
  SET_OF_CONSTANT_TERMS m_constantSet;
  SET_OF_CONSTANT_INDICES m_constantSetByIdx;
  MAP_OF_CONSTANT_INDEX_TO_CONSTANT_TERM m_constantMap;
};