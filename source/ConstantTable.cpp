#pragma once

#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>

#include "ConstantTable.h"

/**
* A constructor.
* Instantiates an unordered map (hashmap) of line numbers to vector of line numbers associated.
*/
ConstantTable::ConstantTable() {
  std::set<int> m_listOfConstants;
  std::unordered_map<int, std::string> m_constantMap;
}

int ConstantTable::insertConstant(std::string t_constant) {
  int index = m_constantMap.size();
  //check if t_constant exists in the set. If it does, do not add into the map.
  if (m_constantSet.find(t_constant) != m_constantSet.end()) {
    return 0;
  } else {
    m_constantSet.insert(t_constant);
    m_constantMap.emplace(index, t_constant);
    return index;
  }

}

LIST_OF_RESULTS ConstantTable::getAllConstants() {
  LIST_OF_RESULTS values;
  //copy the m_allFollows set to values vector.
  values.assign(m_constantSet.begin(), m_constantSet.end());
  return values;
}