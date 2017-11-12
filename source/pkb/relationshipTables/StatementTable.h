#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "../../pql/query-types/Grammar.h"
#include "../../GlobalTypeDef.h"

class StatementTable {
public:
  StatementTable();
  ~StatementTable() {};

  /**
  * Driver method for insertion of the statement into various data structures in StatementTable.
  *
  * @param t_lineNum is the stmt#.
  * @param t_type the type of statement.
  * @param t_procIdx the procedure which the statement resides in.
  * @param t_procName the name of the procedure which the statement resides in.
  */
  void insertStatementIntoStatementTable(STMT_NUM t_lineNum, queryType::GType t_type, PROC_INDEX t_procIdx, PROC_NAME t_procName);

  /**
  * Inserts the statement into an unordered_map of stmt# mapped to a type of statement.
  *
  * @param t_lineNum is the stmt#.
  * @param t_type the type of statement.
  * @return true if insertion is successful.
  */
  BOOLEAN insertTypeOfStatementTable(STMT_NUM t_lineNum, queryType::GType t_type);

  /**
  * Inserts the statement into an unordered_map of a statement type mapped to a vector of stmt#.
  * @param t_type the type of statement.
  * @param t_lineNum is the stmt#.
  * @return true if insertion is successful.
  */
  BOOLEAN insertStatementTypeTable(queryType::GType t_type, STMT_NUM t_lineNum);

  void insertAsProcToStmt(STMT_NUM t_lineNum, PROC_INDEX t_procIdx);
  void insertAsStmtToProc(STMT_NUM t_lineNum, PROC_NAME t_procName);

  /**
  * Method that returns the entire map that tracks every statement's type.
  * @return the unordered map that keep tracks of type of statements.
  */
  MAP_OF_STMT_NUM_TO_GTYPE getTypeOfStatementTable();

  /**
  * Method that returns the entire map of the list of statements that each type of statement has.
  * @return the unordered map that keep tracks of type of statements.
  */
  MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS  getStatementTypeTable();

  /**
  * Method that returns the list of statements that are of the given type.
  * @return the vector of stmt#.
  */
  LIST_OF_STMT_NUMS getListOfStatements(queryType::GType t_type);

  /**
  * Method that returns the type of statement of the given stmt#.
  * @return the type of the statement.
  */
  queryType::GType getTypeOfStatement(STMT_NUM t_lineNum);

  /**
  * Method that returns the total number of statements.
  * @return the number of statements.
  */
  TOTAL_NUMBER_OF_STMTS getNumberOfStatements();

  /**
  * Method that returns the list of statements that the given procedure has.
  * @return the vector of stmt#.
  */
  LIST_OF_STMT_NUMS getStmtsFromProcIdx(PROC_INDEX t_procIdx);

  /**
  * Method that returns the procedure name from the given stmt#.
  * @return the name of the procedure.
  */
  PROC_NAME getProcNameFromStmtNum(STMT_NUM t_lineNum);

private:
  MAP_OF_STMT_NUM_TO_GTYPE m_typeOfStatementTable;
  MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS m_statementTypeTable;

  MAP_OF_PROC_INDEX_TO_LIST_OF_STMT_NUMS m_procIdxToStmts;
  MAP_OF_STMT_NUM_TO_PROC_NAME m_stmtToProcName;
};