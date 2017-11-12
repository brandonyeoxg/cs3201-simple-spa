#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "StatementTable.h"


StatementTable::StatementTable() {}

MAP_OF_STMT_NUM_TO_GTYPE StatementTable::getTypeOfStatementTable() {
  return m_typeOfStatementTable;
}

void StatementTable::insertStatementIntoStatementTable(STMT_NUM t_lineNum, queryType::GType t_type, PROC_INDEX t_procIdx, PROC_NAME t_procName) {
  insertStatementTypeTable(t_type, t_lineNum);
  insertTypeOfStatementTable(t_lineNum, t_type);
  insertAsProcToStmt(t_lineNum, t_procIdx);
  insertAsStmtToProc(t_lineNum, t_procName);
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

void StatementTable::insertAsProcToStmt(STMT_NUM t_lineNum, PROC_INDEX t_procIdx) {
  auto pItr = m_procIdxToStmts.find(t_procIdx);
  if (pItr == m_procIdxToStmts.end()) {
    LIST_OF_STMT_NUMS stmts = { t_lineNum };
    m_procIdxToStmts.insert({ t_procIdx, stmts });
    return;
  }
  pItr->second.push_back(t_lineNum);
}

void StatementTable::insertAsStmtToProc(STMT_NUM t_lineNum, PROC_NAME t_procName) {
  auto pItr = m_stmtToProcName.find(t_lineNum);
  if (pItr == m_stmtToProcName.end()) {
    m_stmtToProcName.insert({t_lineNum, t_procName});
  }
  return;
}


MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS StatementTable::getStatementTypeTable() {
  return m_statementTypeTable;
}

LIST_OF_STMT_NUMS StatementTable::getListOfStatements(queryType::GType t_type) {
  LIST_OF_STMT_NUMS emptyResult;
  auto itr = m_statementTypeTable.find(t_type);
  if (itr != m_statementTypeTable.end()) {
    return itr->second;
  } else {
    return emptyResult;
  }
}

/* Method to return the type of the statement.
*  pre-condition: assumts t_lineNum is valid in the program, i.e. less than or equal to the total number of statements in the source program.
*  @param t_lineNum a statement number.
*  @return the GType of the statement.
*/
queryType::GType StatementTable::getTypeOfStatement(STMT_NUM t_lineNum) {
  auto pItr = m_typeOfStatementTable.find(t_lineNum);
  return pItr->second;
}

TOTAL_NUMBER_OF_STMTS StatementTable::getNumberOfStatements() {
  return m_typeOfStatementTable.size();
}

LIST_OF_STMT_NUMS StatementTable::getStmtsFromProcIdx(PROC_INDEX t_procIdx) {
  auto pItr = m_procIdxToStmts.find(t_procIdx);
  if (pItr == m_procIdxToStmts.end()) {
    return {};
  }
  return m_procIdxToStmts.find(t_procIdx)->second;
}

PROC_NAME StatementTable::getProcNameFromStmtNum(STMT_NUM t_lineNum) {
  auto pItr = m_stmtToProcName.find(t_lineNum);
  if (pItr == m_stmtToProcName.end()) {
    return "";
  }
  return m_stmtToProcName.find(t_lineNum)->second;
}