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
  * getAffectsListFromLMS(INT, INT)
  */
  PAIR_OF_AFFECTS_LIST getAffectsListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound);

private:
  PkbTablesOnly* m_pkbTablesOnly;
  NextTable* m_nextTable;
  StatementTable* m_stmtTable;

  // I need to have the AffectsList and AffectedByStorage
  MAP_OF_STMT_NUMS affectsList;
  MAP_OF_STMT_NUMS affectedByList;
  // Need to have LMS
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS LMS;

  void traverseCfg(PROG_LINE m_curProgLine, PROG_LINE m_endBound);
  BOOLEAN isContainerStmt(queryType::GType t_type);
};

