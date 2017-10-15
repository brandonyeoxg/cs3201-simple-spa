#pragma once
#include <unordered_map>
#include "GlobalTypeDef.h"
#include "VarTable.h"
/**
 * Represents the assignments statements made in the program.
 * The assignment statements are stored in different representation for fast retrieval from PQL through the PKB.
 *
 * @author Brandon Yeo
 * @date 7/9/2017
 */
class AssignTable {
public:

  /**
  * Inserts an assignment stmt into the table.
  *
  * @param t_stmtNum is the statement number that the assignment statement is on.
  * @param t_varIdx is the index as stored in the variable table.
  * @param t_varName is the variable name 
  */
  void insertAssignStmt(STMT_NUM t_stmtNum, VAR_INDEX t_varIdx, VAR_NAME t_varName);

  /**
  * Returns all assignment statements number that modifies the variable t_index.
  * @param t_index the index of the variable of interest.
  */
  LIST_OF_STMT_NUMS getAllAssignStmtListByVar(VAR_INDEX t_index);
  
  /**
  * Gets all assignment statements in the entire program.
  * Used in Select a; where a is assign a;
  */
  LIST_OF_STMT_NUMS& getAllAssignStmts();
  
  /**
  * Returns all assignment statements in a representation of VAR_INDEX : LIST_OF_STMT_NUMBERS.
  * The representation is a variable mapped to all statement number under that variable, i.e. { { "x" -> {1, 3, 6, 8} }, 
  *                                                                                             { "y" -> {2, 3 ,9} } };
  */
  MAP_OF_VAR_INDEX_TO_STMT_NUMS& getAllVarIndexWithAssignStmtNum();
  
  /**
  * Returns all assignment statements in a representation of STMT_NUMBER : VARIABLE_NAME.
  * The representation is a statement number mapped to the variable in that statement number, i.e. { {1 -> "x"}, {2 -> "y"}, {3 -> "x" } };
  */
  MAP_OF_STMT_NUM_TO_VAR_NAME& getAllAssignStmtWithVar();

  /**
  * Returns all assignment statements in a representation.
  * The repsentation is a statement number mapped to the variable in that statement number.
  */
  MAP_OF_VAR_NAME_TO_STMT_NUMS& getAllAssignVarNameWithStmtNum();
private:
  MAP_OF_VAR_INDEX_TO_STMT_NUMS m_assignVarWithAssignStmtNum;
  MAP_OF_VAR_NAME_TO_STMT_NUMS m_assignVarNameWithAssignStmtNum;
  MAP_OF_STMT_NUM_TO_VAR_NAME m_assignMapWithVar;
  LIST_OF_STMT_NUMS m_assignStmts;
};