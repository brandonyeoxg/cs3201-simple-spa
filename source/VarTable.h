#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include "GlobalTypeDef.h"
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
  int insertModifiesForStmt(std::string t_varName, int t_lineNum);
  bool isModifies(int t_lineNum, std::string t_varName);
  bool isUses(int t_lineNum, std::string t_varName);
  std::vector<std::string> getModifies(int t_lineNum);
  std::vector<std::string> getUses(int t_line_num);
  std::vector<int> getStmtModifies(std::string t_varName);
  std::vector<int> getStmtUses(std::string t_varName);
  std::unordered_map<std::string, std::vector<int>> getAllStmtModifies();
  std::unordered_map<std::string, std::vector<int>> getAllStmtUses();
  int getIndexOfVar(std::string t_varName);
  std::string getVarNameFromIndex(int t_index);
  bool isModifiesAnything(int t_line_num);  //uses(2, _)
  bool isUsesAnything(int t_line_num);  //modifies(2, _)
  std::vector<int> getStmtModifiesAnything(); //uses(s, _)
  std::vector<int> getStmtUsesAnything(); //modifies(s, _)
  std::vector<std::string> getAllVariables();

private:
  std::unordered_map<VAR_INDEX, VAR_NAME> m_varIndexMap;
  std::unordered_map<VAR_NAME, VAR_INDEX> m_varNameMap;
  std::unordered_map<int, VarRelations> m_varTable;
  std::set<std::string> m_allVariables;
};
