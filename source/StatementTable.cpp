#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "StatementTable.h"

/**
* A constructor.
* Instantiates 2 unordered_maps that maps statement number to the type of statement, and vice versa.
*/
StatementTable::StatementTable() {
  std::unordered_map<STMT_NUM, queryType::GType> m_typeOfStatementTable;
  std::unordered_map<queryType::GType, LIST_OF_STMT_NUMS> m_statementTypeTable;
}

std::unordered_map<int, queryType::GType> StatementTable::getTypeOfStatementTable() {
  return m_typeOfStatementTable;
}

bool StatementTable::insertTypeOfStatementTable(STMT_NUM t_lineNum, queryType::GType t_type) {
  //if lineNum already exists as key in table, return false.
  if (m_typeOfStatementTable.find(t_lineNum) != m_typeOfStatementTable.end()) {
    return false;
  } else {
    m_typeOfStatementTable.emplace(t_lineNum, t_type);
    return true;
  }
}

std::unordered_map<queryType::GType, LIST_OF_STMT_NUMS> StatementTable::getStatementTypeTable() {
  return m_statementTypeTable;
}

bool StatementTable::insertStatementTypeTable(queryType::GType t_type, STMT_NUM t_lineNum) {
  //if type does not exist as key
  if (m_statementTypeTable.find(t_type) == m_statementTypeTable.end()) {
    LIST_OF_STMT_NUMS lineNums;
    lineNums.push_back(t_lineNum);
    m_statementTypeTable.emplace(t_type, lineNums);
    return true;
  } else {  //type already exists
    LIST_OF_STMT_NUMS lineNums = m_statementTypeTable[t_type];
    if (std::find(lineNums.begin(), lineNums.end(), t_lineNum) != lineNums.end()) {
      //return false if duplicate already exists in table.
      return false;
    }
    //else just push to the vector.
    lineNums.push_back(t_lineNum);
    m_statementTypeTable[t_type] = lineNums;
    return true;
  }
}