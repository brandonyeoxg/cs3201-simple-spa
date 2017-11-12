#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include "../../GlobalTypeDef.h"

class ModifiesTable {
public:
  void insertModifiesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum, VAR_INDEX t_varIdx);
  void insertToModifiesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName, VAR_INDEX t_varIdx);
  BOOLEAN isModifies(STMT_NUM t_lineNum, VAR_NAME t_varName);
  LIST_OF_VAR_NAMES getModifies(STMT_NUM t_lineNum);
  LIST_OF_VAR_INDICES getModifiesByIdx(STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getStmtModifies(VAR_NAME t_varName);
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS getAllStmtModifiesByIdx();
  BOOLEAN isModifiesAnything(STMT_NUM t_lineNum);  //modifies(2, _)
  LIST_OF_STMT_NUMS getStmtModifiesAnything(); //modifies(s, _)

  ModifiesTable();
  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_NAMES getModifiesStmtMap();
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS getModifiesVarMap();

  //method called by the DesignExtractor
  void populateModifiesAnythingRelationships();

private:
  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_NAMES m_modifiesStmtMap;
  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_INDICES m_modifiesStmtByIdxMap;

  MAP_OF_STMT_NUM_TO_SET_OF_NAMES m_modifiesStmtSet;
  MAP_OF_STMT_NUM_TO_SET_OF_VAR_INDICES m_modifiesStmtByIdxSet;

  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS m_modifiesVarMap;
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS m_modifiesVarByIdxMap;

  SET_OF_STMT_NUMS m_allStmtNumsModified;
  LIST_OF_STMT_NUMS m_allStmtNumsModifiedList;


};