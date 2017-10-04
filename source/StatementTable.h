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
  std::unordered_map<STMT_NUM, queryType::GType> getTypeOfStatementTable();
  bool insertTypeOfStatementTable(STMT_NUM t_lineNum, queryType::GType t_type);
  std::unordered_map<queryType::GType, LIST_OF_STMT_NUMS>  getStatementTypeTable();
  bool insertStatementTypeTable(queryType::GType t_type, STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getListOfStatements(queryType::GType t_type);

private:
  std::unordered_map<STMT_NUM, queryType::GType> m_typeOfStatementTable;
  std::unordered_map<queryType::GType,LIST_OF_STMT_NUMS> m_statementTypeTable;

};