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
  * Returns true if there are affects relations between target start and target end  between the bounds.
  * @param t_startBound the starting bound where computation of the affects relation starts
  * @param t_endBound the ending bound where computation of the affects relation ends
  * @param t_targetStart if specified, checks for any affects after the target start
  * @param t_targetEnd if specified, check for any affects before the target end.
  *
  */
  BOOLEAN hasAffectsFromBounds(PROG_LINE t_startBound, PROG_LINE t_endBound, PROG_LINE t_targetStart, PROG_LINE t_targetEnd);

  /**
  * Returns true if there is an affects relation with the modifies line and uses line.
  * @param t_modifiesLine the modifies line.
  * @param t_usesLine the uses line.
  *
  */
  BOOLEAN isAffects(PROG_LINE t_modfiesLine, PROG_LINE t_usesLine);

  /**
  * Returns true if there is an affects* relation with the modifies line and uses line.
  * @param t_modifiesLine the modifies line.
  * @param t_usesLine the uses line.
  *
  */
  BOOLEAN isAffectsStar(PROG_LINE t_modfiesLine, PROG_LINE t_usesLine);

  /**
  * Returns a pair list, the affects list and affected by list, from the start bound and end bound
  * @param t_startBound the start bound for the affects to start running from.
  * @param t_endBound the end bound for the affects to end at.
  *
  */
  PAIR_OF_AFFECTS_LIST getAffectsListsFromBounds(PROG_LINE t_startBound, PROG_LINE t_endBound);
 
  ///////////////////////////////////////////////////////
  //  Affects*
  ///////////////////////////////////////////////////////
  /**
  * Returns a pair list, the affects* list and affected* by list, from the start bound and end bound
  * @param t_startBound the start bound for the affects* to start running from.
  * @param t_endBound the end bound for the affects* to end at.
  *
  */
  PAIR_OF_AFFECTS_LIST getAffectsStarListsFromBounds(PROG_LINE t_startBound, PROG_LINE t_endBound);

private:
  PkbTablesOnly* m_pkbTablesOnly;
  NextTable* m_nextTable;
  StatementTable* m_stmtTable;
  BOOLEAN m_isEarlyExit, m_isAffectsStar;
  PROG_LINE m_targetStart, m_targetEnd;

  // I need to have the AffectsList and AffectedByStorage
  MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS m_affectsList;
  MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS m_affectedByList;

  /**
  * Traverses the control flow graph within a bound.
  * 
  */
  BOOLEAN traverseCfgWithinBound(PROG_LINE &t_nextLine, PROG_LINE t_endBound, 
    MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);
  
  /**
  * Handles how the affects algorithm behaves when encountering an assignmnet statement.
  *
  */
  BOOLEAN handleAssignStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);
  
  /**
  * Handles how the affects algorithm behaves when encountering a calls statement.
  *
  */
  BOOLEAN handleCallStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);
  
  /**
  * Handles how the affects algorithm behaves when encounteing an if statement.
  */
  BOOLEAN handleIfStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);

  /**
  ( Handles how the affects algorithm behaves when encountering a while statement.
  */
  BOOLEAN handleWhileStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);

  /**
  * Merges the tables into a single unified table and returns it
  * @param t_table1 represents the first table
  * @param t_table2 represents the second table
  *
  */
  MAP_OF_VAR_INDEX_TO_SET_OF_STMT_NUMS mergeTable(MAP_OF_VAR_INDEX_TO_SET_OF_NUMS t_table1, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS t_table2);

  /**
  * Returns the real starting bound based off the starting bound.
  * @param t_startBound the starting bound for which the real starting bound is found out.
  *
  */
  PROG_LINE getRealStartBound(PROG_LINE t_startBound);

  /**
  * Inserts into the affects lists and affected by lists.
  * @param t_modifiesLine the line that modifies.
  * @param t_usesLine the line that uses.
  *
  */
  void insertIntoAffectsLists(PROG_LINE t_modifiesLine, PROG_LINE t_usesLine);
  
  /**
  * Updates the last uses table with the any previous uses statement if the modifies variable can be found in the uses variables. 
  *
  */
  void updateLutWithSameModifiesAndUses(VAR_INDEX modifiesIdx, LIST_OF_VAR_INDICES usesVars, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);

  /**
  * Updates the last uses table with the uses stmt numbers from the statement list.
  *
  */
  void updateLutWithOtherUses(VAR_INDEX modifiesIdx, LIST_OF_VAR_INDICES usesVars, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut);
  

  /**
  * Updates the last modified table the statement numbers from the last uses table corresponding to the modifies var hash key.
  *
  */
  void updateLmt(PROG_LINE t_nextLine, VAR_INDEX modifiesIdx, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt);
};

