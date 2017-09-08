#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "VarRelations.h"

//constructor
VarRelations::VarRelations() {
  std::string m_varName;
  std::vector<int> m_uses;
  std::vector<int> m_modifies;
}

void VarRelations::setVarName(std::string varName) {
  m_varName = varName;
}

bool VarRelations::insertUses(int line_num) {
  //if line_num already exists in uses vector, return false
  if (std::find(m_uses.begin(), m_uses.end(), line_num) != m_uses.end()) {
    return false;
  } else {
    m_uses.push_back(line_num);
    return true;
  }
}

bool VarRelations::insertModifies(int line_num) {
  //if line_num already exists in uses vector, return false
  if (std::find(m_modifies.begin(), m_modifies.end(), line_num) != m_modifies.end()) {
    return false;
  } else {
    m_modifies.push_back(line_num);
    return true;
  }
}

std::string VarRelations::getVarName() {
  return m_varName;
}

std::vector<int> VarRelations::getUses() {
  return m_uses;
}

std::vector<int> VarRelations::getModifies() {
  return m_modifies;
}
