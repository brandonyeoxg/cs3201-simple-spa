#pragma once
#include "GlobalTypeDef.h"
class StmtListTable
{
public:
  StmtListTable() {};
  ~StmtListTable() {};

  void insertStmtLst(STMT_NUM t_line);
  LIST_OF_STMT_NUMS&  getStmtList();
private:
  LIST_OF_STMT_NUMS m_stmtLst;
};

