#pragma once
#include "../../GlobalTypeDef.h"
#include "../PkbTablesOnly.h"

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
                                                 m_stmtTable(t_pkbTablesOnly->getStatementTable()){
    m_isEarlyExit = m_isAffectsStar = false;
    m_targetStart = INVALID_PROG_LINE;
    m_targetEnd = INVALID_PROG_LINE;

  };
  ~AffectsTable() {};

  ///////////////////////////////////////////////////////
  //  Affects
  ///////////////////////////////////////////////////////
  /**
  * From verbena's doc on PKB > Affects
  * hasAffectsBetween(INT, INT)
  */
  BOOLEAN hasAffectsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound, PROG_LINE t_targetStart, PROG_LINE t_targetEnd);

  /**
  * From verbena's doc on PKB > Affects
  * isAffectsFromLMS(INT, INT)
  * Affects(4, 12) is true
  */
  BOOLEAN isAffects(STMT_NUM t_modfiesLine, STMT_NUM t_usesLine); 

  /**
  * From verbena's doc on PKB > Affects
  * isAffectsFromLMS(INT, INT)
  * Affects*(4, 12) is true
  */
  BOOLEAN isAffectsStar(STMT_NUM t_modfiesLine, STMT_NUM t_usesLine);

  /**
  * From verbena's doc on PKB > Affects
  * getAffectsListFromLMS(INT, INT)
  */
  PAIR_OF_AFFECTS_LIST getAffectsListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound);
 
  ///////////////////////////////////////////////////////
  //  Affects*
  ///////////////////////////////////////////////////////
  /**
  * From verbena's doc on PKB > Affects
  * getAffectsStarListFromLMS(INT, INT)
  */
  PAIR_OF_AFFECTS_LIST getAffectsStarListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound);

private:
  PkbTablesOnly* m_pkbTablesOnly;
  NextTable* m_nextTable;
  StatementTable* m_stmtTable;
  BOOLEAN m_isEarlyExit, m_isAffectsStar;
  PROG_LINE m_targetStart, m_targetEnd;

  // I need to have the AffectsList and AffectedByStorage
  MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS m_affectsList;
  MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS m_affectedByList;

  BOOLEAN traverseCfgWithinBound(PROG_LINE &t_nextLine, PROG_LINE t_endBound, 
    MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);
  BOOLEAN handleAssignStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);
  BOOLEAN handleCallStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);
  BOOLEAN handleIfStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);
  BOOLEAN handleWhileStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);

  BOOLEAN isContainerStmt(queryType::GType t_type);
  MAP_OF_VAR_INDEX_TO_SET_OF_STMT_NUMS mergeTable(MAP_OF_VAR_INDEX_TO_SET_OF_NUMS, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS);
  PROG_LINE getRealStartBound(PROG_LINE t_startBound);
  void insertIntoAffectsLists(PROG_LINE t_modifiesLine, PROG_LINE t_usesLine);
 
  void updateLutWithSameModifiesAndUses(VAR_INDEX modifiesIdx, LIST_OF_VAR_INDICES usesVars, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);
  void updateLutWithOtherUses(VAR_INDEX modifiesIdx, LIST_OF_VAR_INDICES usesVars, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);
  void updateLmt(PROG_LINE t_nextLine, VAR_INDEX modifiesIdx, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt);
};
