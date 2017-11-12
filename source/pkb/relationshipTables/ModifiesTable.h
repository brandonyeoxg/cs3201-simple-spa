#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include "../../GlobalTypeDef.h"

/**
* Represents the modifies relations made in the program.
* The relations are stored in different representation for fast retrieval from PQL through the PKB.
*
* @author pengcheng
*/
class ModifiesTable {
public:
  /**
  * Establish the modifies(lineNum, varName) relationship.
  * @param t_varName the variable being modified
  * @param t_lineNum the stmt#
  * @param t_varIdx the index of the t_varName
  */
  void insertModifiesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum, VAR_INDEX t_varIdx);

  /**
  * Inserts a modifies relation into the stmtMap representation.
  * Provides duality (2-way) mapping between stmt# and variable name/indices.
  *
  * @param t_varName is the name of the variable that the statement is modifying.
  * @param t_lineNum is the stmt#.
  * @param t_varIdx is the index of the variable in VarTable.
  */
  void insertToModifiesStmtMap(STMT_NUM t_lineNum, VAR_NAME t_varName, VAR_INDEX t_varIdx);

  /**
  * Checks if modifies(lineNum, varName) is true.
  * @param t_lineNum the stmt#
  * @param t_varName the variable being modified
  * @return true if the relationship holds.
  */
  BOOLEAN isModifies(STMT_NUM t_lineNum, VAR_NAME t_varName);

  /**
  * Get the list of variables that are being modified in stmt# lineNum.
  * @param t_lineNum the stmt#
  * @return the list of variables.
  */
  LIST_OF_VAR_NAMES getModifies(STMT_NUM t_lineNum);
  LIST_OF_VAR_INDICES getModifiesByIdx(STMT_NUM t_lineNum);

  /**
  * Get the list of stmt# that modifies the variable varName.
  * @param t_varName the variable
  * @return the list of stmt#.
  */
  LIST_OF_STMT_NUMS getStmtModifies(VAR_NAME t_varName);

  /**
  * Get the map of all modifies relationship i.e. for modifies(s, v) where s and v are synonyms.
  * @return the map that contains all uses relationship.
  */
  MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS getAllStmtModifiesByIdx();

  /**
  * Checks if the stmt# modifies any variables.
  * i.e. modifies(2, _)
  * @param t_lineNum the stmt#
  * @return true if modifies(2, _) hold.
  */
  BOOLEAN isModifiesAnything(STMT_NUM t_lineNum);

  /**
  * Get the list of stmt# that modifies any variables i.e. modifies(s, _).
  * @return the list of stmt#
  */
  LIST_OF_STMT_NUMS getStmtModifiesAnything();

  ModifiesTable();
  MAP_OF_STMT_NUM_TO_LIST_OF_VAR_NAMES getModifiesStmtMap();
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS getModifiesVarMap();

  /**
  * Method called by DesignExtractor.
  * Populates the vectors that store all the unique statement numbers
  * that involve in modifies relationship.
  */
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