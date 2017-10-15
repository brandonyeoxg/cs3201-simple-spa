#pragma once
#include "GlobalTypeDef.h"

/*
* Represents the all statement lists in the program.
* Statement list are represented by the first statement number in the statement list.
*/
class StmtListTable
{
public:
  StmtListTable() {};
  ~StmtListTable() {};

  /*
  * Inserts the statement list in to the table
  * @param t_line should not be a negative number.
  */
  void insertStmtLst(STMT_NUM t_line);

  /*
  * Returns all the statement list in the table
  */
  LIST_OF_STMT_NUMS&  getStmtList();
private:
  LIST_OF_STMT_NUMS m_stmtLst;
};

