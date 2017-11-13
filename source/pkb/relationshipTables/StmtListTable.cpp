#include "StmtListTable.h"

void StmtListTable::insertStmtLst(STMT_NUM t_line) {
  m_stmtLst.push_back(t_line);
}

LIST_OF_STMT_NUMS&  StmtListTable::getStmtList() {
  return m_stmtLst;
}