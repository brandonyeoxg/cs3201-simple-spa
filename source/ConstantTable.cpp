#pragma once

#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>

#include "ConstantTable.h"


ConstantTable::ConstantTable() {}

CONSTANT_INDEX ConstantTable::insertConstant(std::string t_constant) {
  int index = m_constantMap.size();
  //check if t_constant exists in the set. If it does, do not add into the map.
  if (m_constantSet.find(t_constant) != m_constantSet.end()) {
    return 0;
  } else {
    m_constantSet.insert(t_constant);
    m_constantSetByIdx.insert(index);
    m_constantMap.emplace(index, t_constant);
    return index;
  }

}
CONSTANT_TERM ConstantTable::getConstantFromIdx(int t_constantIdx) {
  if (m_constantMap.find(t_constantIdx) == m_constantMap.end()) {
    //if index is not present in map, throw exception
    throw std::invalid_argument("key constantIdx does not exist in ConstantTable");
  } else {
    return m_constantMap[t_constantIdx];
  }
}

LIST_OF_RESULTS ConstantTable::getAllConstants() {
  LIST_OF_RESULTS values;
  //copy the m_constants set to values vector.
  values.assign(m_constantSet.begin(), m_constantSet.end());
  return values;
}

LIST_OF_CONSTANT_INDICES ConstantTable::getAllConstantsByIdx() {
  LIST_OF_CONSTANT_INDICES values;
  values.assign(m_constantSetByIdx.begin(), m_constantSetByIdx.end());
  return values;
}