#pragma once
#include "GlobalTypeDef.h"
#include "PkbTablesOnly.h"

/**
* Represents the affects relationship.
* Computes Affects relation on demand using a tablular format.
*
* @author Brandon
* @date 28/10/2017
*/
class AffectsTable
{
public:
  AffectsTable(PkbTablesOnly *t_pkbTablesOnly) : m_pkbTablesOnly(t_pkbTablesOnly),
                                                 m_nextTable(t_pkbTablesOnly->getNextTable()),
                                                 m_stmtTable(t_pkbTablesOnly->getStatementTable()){};
  ~AffectsTable() {};

  /**
  * From verbena's doc on PKB > Affects
  * hasAffectsFromLMS()
  */
  BOOLEAN hasAnyAffects();

  /**
  * From verbena's doc on PKB > Affects
  * hasAffectsBetween(INT, INT)
  */
  BOOLEAN hasAffectsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound);

  /**
  * From verbena's doc on PKB > Affects
  * isAffectsFromLMS(INT, INT)
  */
  BOOLEAN isAffects(STMT_NUM t_modfiesLine, STMT_NUM t_usesLine); // Affects(4, 12) is true

  /**
  * From verbena's doc on PKB > Affects
  * getAffectsListFromLMS(INT, INT)
  */
  PAIR_OF_AFFECTS_LIST getAffectsListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound);

private:
  PkbTablesOnly* m_pkbTablesOnly;
  NextTable* m_nextTable;
  StatementTable* m_stmtTable;

  // I need to have the AffectsList and AffectedByStorage
  MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectsList;
  MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectedByList;
  // Need to have LMS
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS LMS;

  void traverseCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt);
  void traverseContainerCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type);
  void traverseNonContainerCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type);
  void traverseIfStmtWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt);
  void traverseWhileStmtWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt);

  void handleAffectsOnAssgnStmt(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt);
  void handleAffectsOnCallStmt(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt);

  BOOLEAN traverseCfgWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt);
  BOOLEAN traverseContainerCfgWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type);
  BOOLEAN traverseNonContainerCfgWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type);
  BOOLEAN traverseIfStmtWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt);
  BOOLEAN traverseWhileStmtWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt);

  BOOLEAN handleAffectsOnAssgnStmtEarlyExit(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt);

  BOOLEAN isContainerStmt(queryType::GType t_type);
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS mergeLmt(MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS);

  PROG_LINE getRealStartBound(PROG_LINE t_startBound);
};

