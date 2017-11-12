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
  void insertStatementIntoStatementTable(STMT_NUM t_lineNum, queryType::GType t_type, PROC_INDEX t_procIdx, PROC_NAME t_procName);
  BOOLEAN insertTypeOfStatementTable(STMT_NUM t_lineNum, queryType::GType t_type);
  BOOLEAN insertStatementTypeTable(queryType::GType t_type, STMT_NUM t_lineNum);
  void insertAsProcToStmt(STMT_NUM t_lineNum, PROC_INDEX t_procIdx);
  void insertAsStmtToProc(STMT_NUM t_lineNum, PROC_NAME t_procName);


  MAP_OF_STMT_NUM_TO_GTYPE getTypeOfStatementTable();
  MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS  getStatementTypeTable();
  LIST_OF_STMT_NUMS getListOfStatements(queryType::GType t_type);
  queryType::GType getTypeOfStatement(STMT_NUM t_lineNum);
  TOTAL_NUMBER_OF_STMTS getNumberOfStatements();

  LIST_OF_STMT_NUMS getStmtsFromProcIdx(PROC_INDEX t_procIdx);
  PROC_NAME getProcNameFromStmtNum(STMT_NUM t_lineNum);

private:
  MAP_OF_STMT_NUM_TO_GTYPE m_typeOfStatementTable;
  MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS m_statementTypeTable;

  MAP_OF_PROC_INDEX_TO_LIST_OF_STMT_NUMS m_procIdxToStmts;
  MAP_OF_STMT_NUM_TO_PROC_NAME m_stmtToProcName;
};