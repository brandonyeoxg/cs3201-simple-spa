#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Grammar.h"
#include "GlobalTypeDef.h"

class StatementTable {
public:
  StatementTable();

  void insertStatementIntoStatementTable(STMT_NUM t_lineNum, queryType::GType t_type, PROC_INDEX t_procIdx, PROC_NAME t_procName);
  BOOLEAN insertTypeOfStatementTable(STMT_NUM t_lineNum, queryType::GType t_type);
  BOOLEAN insertStatementTypeTable(queryType::GType t_type, STMT_NUM t_lineNum);

  MAP_OF_STMT_NUM_TO_GTYPE getTypeOfStatementTable();
  MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS  getStatementTypeTable();
  LIST_OF_STMT_NUMS getListOfStatements(queryType::GType t_type);
  TOTAL_NUMBER_OF_STMTS getNumberOfStatements();


private:
  std::unordered_map<STMT_NUM, queryType::GType> m_typeOfStatementTable;
  std::unordered_map<queryType::GType,LIST_OF_STMT_NUMS> m_statementTypeTable;

  MAP_OF_PROC_NAME_TO_LIST_OF_STMT_NUMS m_procNameToStmts;
  MAP_OF_STMT_NUM_TO_PROC_NAME m_stmtToProcName;
};