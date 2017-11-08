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
  BOOLEAN isUses(STMT_NUM t_lineNum, VAR_NAME t_varName);
  LIST_OF_VAR_NAMES getUses(STMT_NUM t_lineNum);
  LIST_OF_VAR_INDICES getUsesByIdx(STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getStmtUses(VAR_NAME t_varName);
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS getAllStmtUses();
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS getAllStmtUsesByIdx();
  BOOLEAN isUsesAnything(STMT_NUM t_lineNum);  //uses(2, _)
  LIST_OF_STMT_NUMS getStmtUsesAnything(); //uses(s, _)
  LIST_OF_VAR_NAMES getAllUsesVarNames();
  LIST_OF_VAR_INDICES getAllUsesVarNamesByIdx();

  UsesTable();
  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_NAMES getUsesStmtMap();
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS getUsesVarMap();

private:
  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_NAMES m_usesStmtMap;
  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_INDICES m_usesStmtByIdxMap;

  MAP_OF_STMT_NUM_TO_SET_OF_NAMES m_usesStmtSet;
  MAP_OF_STMT_NUM_TO_SET_OF_VAR_INDICES m_usesStmtByIdxSet;

  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS m_usesVarMap;
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS m_usesVarByIdxMap;

  SET_OF_VAR_NAMES m_allVariablesUsed;
  SET_OF_VAR_INDICES m_allVariablesUsedByIdx;

  SET_OF_STMT_NUMS m_allStmtNumsUsed;
};