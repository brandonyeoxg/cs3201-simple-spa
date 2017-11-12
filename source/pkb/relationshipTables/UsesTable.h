#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include "../../GlobalTypeDef.h"

/**
* Represents the uses relations made in the program.
* The relations are stored in different representation for fast retrieval from PQL through the PKB.
*
* @author pengcheng
*/
class UsesTable {
public:

  /**
  * Inserts an uses relation into the table.
  *
  * @param t_varName is the name of the variable that the statement is using.
  * @param t_lineNum is the stmt#.
  * @param t_varIdx is the index of the variable in VarTable.
  */
  void insertUsesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum, VAR_INDEX t_varIdx);

  /**
  * Inserts an uses relation into the stmtMap representation.
  * Provides duality (2-way) mapping between stmt# and variable name/indices.
  *
  * @param t_varName is the name of the variable that the statement is using.
  * @param t_lineNum is the stmt#.
  * @param t_varIdx is the index of the variable in VarTable.
  */
  void insertToUsesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName, VAR_INDEX t_varIdx);

  /**
  * Checks if uses(lineNum, varName) is true.
  * @param t_lineNum the stmt#
  * @param t_varName the variable being used
  * @return true if the relationship holds.
  */
  BOOLEAN isUses(STMT_NUM t_lineNum, VAR_NAME t_varName);

  /**
  * Get the list of variables that are being used in stmt# lineNum.
  * @param t_lineNum the stmt#
  * @return the list of variables.
  */
  LIST_OF_VAR_INDICES getUsesByIdx(STMT_NUM t_lineNum);

  /**
  * Get the list of stmt# that use the variable varName.
  * @param t_varName the variable
  * @return the list of stmt#.
  */
  LIST_OF_STMT_NUMS getStmtUses(VAR_NAME t_varName);

  /**
  * Get the map of all uses relationship i.e. for uses(s, v) where s and v are synonyms.
  * @return the map that contains all uses relationship.
  */
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS getAllStmtUsesByIdx();

  /**
  * Checks if the stmt# uses any variables.
  * i.e. uses(2, _)
  * @param t_lineNum the stmt#
  * @return true if uses(2, _) hold.
  */
  BOOLEAN isUsesAnything(STMT_NUM t_lineNum);

  /**
  * Get the list of stmt# that use any variables i.e. uses(s, _).
  * @return the list of stmt#
  */
  LIST_OF_STMT_NUMS getStmtUsesAnything();

  UsesTable();
  ~UsesTable() {};

  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_NAMES getUsesStmtMap();
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS getUsesVarMap();

  /**
  * Method called by DesignExtractor.
  * Populates the vectors that store all the unique statement numbers
  * that involve in uses relationship.
  */
  void populateUsesAnythingRelationships();

private:
  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_NAMES m_usesStmtMap;
  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_INDICES m_usesStmtByIdxMap;

  MAP_OF_STMT_NUM_TO_SET_OF_NAMES m_usesStmtSet;
  MAP_OF_STMT_NUM_TO_SET_OF_VAR_INDICES m_usesStmtByIdxSet;

  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS m_usesVarMap;
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS m_usesVarByIdxMap;

  SET_OF_STMT_NUMS m_allStmtNumsUsed;
  LIST_OF_STMT_NUMS m_allStmtNumsUsedList;
};