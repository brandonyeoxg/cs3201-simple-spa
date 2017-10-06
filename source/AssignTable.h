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
  std::list<STMT_NUM> getAllAssignStmtListByVar(VAR_INDEX t_index);
  
  /*
  * Returns all assignment statements.
  */
  LIST_OF_STMT_NUMS getAllAssignStmtList();
  
  /*
  * Returns all assignment statements in a representation.
  * The representation is a variable mapped to all statement number under that variable.
  */
  std::unordered_map<VAR_INDEX, std::list<STMT_NUM>>& getAllVarInWithAssignStmtNum();
  
  /*
  * Returns all assignment statements in a representation.
  * The repsentation is a statement number mapped to the variable in that statement number.
  */
  std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVar();
 
  std::unordered_map<STMT_NUM, VAR_NAME>& getAssignMapWithVar();
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS>& getAllAssignVarNameWithStmtNum();
private:
  std::unordered_map<VAR_INDEX, std::list<STMT_NUM>> m_assignVarWithAssignStmtNum;
  std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> m_assignVarNameWithAssignStmtNum;
  std::unordered_map<STMT_NUM, VAR_NAME> m_assignMapWithVar;
  LIST_OF_STMT_NUMS m_assignStmts;
};