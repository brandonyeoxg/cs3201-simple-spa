#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class VarRelations {
public:
  VarRelations();
  void setVarName(std::string t_varName);
  bool insertUses(int t_line_num);
  bool insertModifies(int t_line_num);
  
  //getter methods
  std::string getVarName();
  std::vector<int> getUses();
  std::vector<int> getModifies();

private:
  std::string m_varName;
  std::vector<int> m_uses;
  std::vector<int> m_modifies;

};
