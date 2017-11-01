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
  void insertUsesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum, VAR_INDEX t_varIdx);
  void insertToUsesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName, VAR_INDEX t_varIdx);
  bool isUses(STMT_NUM t_lineNum, VAR_NAME t_varName);
  LIST_OF_VAR_NAMES getUses(STMT_NUM t_lineNum);
  LIST_OF_VAR_INDICES getUsesByIdx(STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getStmtUses(VAR_NAME t_varName);
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> getAllStmtUses();
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS getAllStmtUsesByIdx();
  bool isUsesAnything(STMT_NUM t_lineNum);  //uses(2, _)
  LIST_OF_STMT_NUMS getStmtUsesAnything(); //uses(s, _)
  LIST_OF_VAR_NAMES getAllUsesVarNames();
  LIST_OF_VAR_INDICES getAllUsesVarNamesByIdx();

  UsesTable();
  std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> getUsesStmtMap();
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> getUsesVarMap();

private:
  std::unordered_map<STMT_NUM, LIST_OF_VAR_NAMES> m_usesStmtMap;
  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_INDICES m_usesStmtByIdxMap;

  MAP_OF_STMT_NUM_TO_SET_OF_NAMES m_usesStmtSet;
  MAP_OF_STMT_NUM_TO_SET_OF_VAR_INDICES m_usesStmtByIdxSet;

  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> m_usesVarMap;
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS m_usesVarByIdxMap;

  SET_OF_VAR_NAMES m_allVariablesUsed;
  SET_OF_VAR_INDICES m_allVariablesUsedByIdx;

  SET_OF_STMT_NUMS m_allStmtNumsUsed;
};