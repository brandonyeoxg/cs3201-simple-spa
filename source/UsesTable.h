#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include "GlobalTypeDef.h"


class UsesTable {
public:
  void insertUsesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum);
  void insertToUsesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName);
  bool isUses(STMT_NUM t_lineNum, VAR_NAME t_varName);
  LIST_OF_VAR_NAMES getUses(STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getStmtUses(VAR_NAME t_varName);
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> getAllStmtUses();
  bool isUsesAnything(STMT_NUM t_lineNum);  //uses(2, _)
  LIST_OF_STMT_NUMS getStmtUsesAnything(); //uses(s, _)
  LIST_OF_VAR_NAMES getAllUsesVarNames();

  UsesTable();
  std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> getUsesStmtMap();
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> getUsesVarMap();

private:
  std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> m_usesStmtMap;
  MAP_OF_STMT_NUM_TO_SET_OF_NAMES m_usesStmtSet;
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> m_usesVarMap;
  SET_OF_VAR_NAMES m_allVariablesUsed;
  SET_OF_STMT_NUMS m_allStmtNumsUsed;
};