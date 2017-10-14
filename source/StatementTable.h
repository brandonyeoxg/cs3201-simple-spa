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
  MAP_OF_STMT_NUM_TO_GTYPE getTypeOfStatementTable();
  bool insertTypeOfStatementTable(STMT_NUM t_lineNum, queryType::GType t_type);
  MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS  getStatementTypeTable();
  bool insertStatementTypeTable(queryType::GType t_type, STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getListOfStatements(queryType::GType t_type);

private:
  std::unordered_map<STMT_NUM, queryType::GType> m_typeOfStatementTable;
  std::unordered_map<queryType::GType,LIST_OF_STMT_NUMS> m_statementTypeTable;

};