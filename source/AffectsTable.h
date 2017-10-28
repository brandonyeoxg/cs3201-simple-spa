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
  AffectsTable(PkbTablesOnly *t_pkbTablesOnly) : m_pkbTablesOnly(t_pkbTablesOnly) {};
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
  PkbTablesOnly *m_pkbTablesOnly;
};

