#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include "GlobalTypeDef.h"

class ModifiesTable {
public:
  void insertModifiesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum, VAR_INDEX t_varIdx);
  void insertToModifiesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName);
  bool isModifies(STMT_NUM t_lineNum, VAR_NAME t_varName);
  LIST_OF_VAR_NAMES getModifies(STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getStmtModifies(VAR_NAME t_varName);
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> getAllStmtModifies();
  bool isModifiesAnything(STMT_NUM t_lineNum);  //modifies(2, _)
  LIST_OF_STMT_NUMS getStmtModifiesAnything(); //modifies(s, _)
  LIST_OF_VAR_NAMES getAllModifiesVarNames();

  ModifiesTable();
  std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> getModifiesStmtMap();
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> getModifiesVarMap();

private:
  std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> m_modifiesStmtMap;
  MAP_OF_STMT_NUM_TO_SET_OF_NAMES m_modifiesStmtSet;
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> m_modifiesVarMap;
  SET_OF_VAR_NAMES m_allVariablesModified;
  SET_OF_STMT_NUMS m_allStmtNumsModified;


};