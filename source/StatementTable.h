#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Grammar.h"

class StatementTable {
public:
  StatementTable();
  std::unordered_map<int, queryType::GType> getTypeOfStatementTable();
  bool insertTypeOfStatementTable(int t_lineNum, queryType::GType t_type);
  std::unordered_map<queryType::GType, std::vector<int>>  getStatementTypeTable();
  bool insertStatementTypeTable(queryType::GType t_type, int t_lineNum);

private:
  std::unordered_map<int, queryType::GType> m_typeOfStatementTable;
  std::unordered_map<queryType::GType, std::vector<int>> m_statementTypeTable;

};