#pragma once
#include <unordered_map>
#include "GlobalTypeDef.h"
#include "VarTable.h"
/**
 * Represents the assignments statements made in the program in difference representation.
 *
 * @author Brandon Yeo
 * @date 7/9/2017
 */
class AssignTable {
public:

  void insertAssignStmt(STMT_NUM t_stmtNum, VAR_INDEX t_varIdx, VAR_NAME t_varName);

  LIST_OF_STMT_NUMS& getAllAssignStmt();

  /*
  * Returns all assignment statements number that modifies the variable t_index.
  * @param t_index the index of the variable of interest.
  */
  LIST_OF_STMT_NUMS getAllAssignStmtListByVar(VAR_INDEX t_index);
  
  /*
  * Returns all assignment statements.
  */
  LIST_OF_STMT_NUMS& getAllAssignStmtList();
  
  /*
  * Returns all assignment statements in a representation.
  * The representation is a variable mapped to all statement number under that variable.
  */
  MAP_OF_VAR_INDEX_TO_STMT_NUMS& getAllVarIndexWithAssignStmtNum();
  
  /*
  * Returns all assignment statements in a representation.
  * The repsentation is a statement number mapped to the variable in that statement number.
  */
  MAP_OF_STMT_NUM_TO_VAR_NAME& getAllAssignStmtWithVar();
 
  std::unordered_map<STMT_NUM, VAR_NAME>& getAssignMapWithVar();
  MAP_OF_VAR_NAME_TO_STMT_NUMS& getAllAssignVarNameWithStmtNum();
private:
  MAP_OF_VAR_INDEX_TO_STMT_NUMS m_assignVarWithAssignStmtNum;
  MAP_OF_VAR_NAME_TO_STMT_NUMS m_assignVarNameWithAssignStmtNum;
  MAP_OF_STMT_NUM_TO_VAR_NAME m_assignMapWithVar;
  LIST_OF_STMT_NUMS m_assignStmts;
};