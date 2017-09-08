#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class VarRelations {
public:
  VarRelations();
  void setVarName(std::string varName);
  bool insertUses(int line_num);
  bool insertModifies(int line_num);
  
  //getter methods
  std::string getVarName();
  std::vector<int> getUses();
  std::vector<int> getModifies();

private:
  std::string m_varName;
  std::vector<int> m_uses;
  std::vector<int> m_modifies;

};
