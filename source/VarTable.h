#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include "VarRelations.h"

/**
* Represents the variable table, which is a vector of unordered_maps to 
* track which variable appear on which line.
*
* @author pengcheng
* @date 8/27/2017
*
*/
class VarTable {
public:
  std::unordered_map<int, VarRelations> getVarTable();

  VarTable();

  int insertUsesForStmt(std::string t_varName, int t_lineNum);
  int insertModifiesForStmt(std::string varName, int lineNum);
  bool isModifies(int lineNum, std::string varName);
  bool isUses(int lineNum, std::string varName);
  std::vector<std::string> getModifies(int lineNum);
  std::vector<std::string> getUses(int line_num);
  std::vector<int> getStmtModifies(std::string varName);
  std::vector<int> getStmtUses(std::string varName);
  std::unordered_map<std::string, std::vector<int>> getAllStmtModifies();
  std::unordered_map<std::string, std::vector<int>> getAllStmtUses();
  int getIndexOfVar(std::string varName);
  bool isModifiesAnything(int t_line_num);  //uses(2, _)
  bool isUsesAnything(int t_line_num);  //modifies(2, _)
  std::vector<int> getStmtModifiesAnything(); //uses(s, _)
  std::vector<int> getStmtUsesAnything(); //modifies(s, _)

private:
  std::unordered_map<int, VarRelations> m_varTable;
  std::set<std::string> m_allVariables;
};
