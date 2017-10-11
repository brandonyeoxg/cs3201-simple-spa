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
  std::unordered_map<int, Grammar::GType> getTypeOfStatementTable();
  bool insertTypeOfStatementTable(int t_lineNum, Grammar::GType t_type);
  std::unordered_map<Grammar::GType, std::vector<int>>  getStatementTypeTable();
  bool insertStatementTypeTable(Grammar::GType t_type, int t_lineNum);

private:
  std::unordered_map<int, Grammar::GType> m_typeOfStatementTable;
  std::unordered_map<Grammar::GType, std::vector<int>> m_statementTypeTable;

};