#pragma once
#include "Extractor.h"
#include "AffectsTable.h"
#include <algorithm>

/**
* Handles extractions for the Affects relation.
*
* @author Brandon
* @date 28/10/2017
*/
class AffectsExtractor : public Extractor
{
public:
  AffectsExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {
    m_affectsTable = new AffectsTable(t_pkb);
  };

  ~AffectsExtractor() {};

  void extractDesign();

  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS extractAllAffects(); // affects(a1,a2)
  LIST_OF_AFFECTS_STMTS extractAffects(STMT_NUM t_usesLine); // affects(a,12)
  LIST_OF_AFFECTS_STMTS extractAffectedBy(STMT_NUM t_modifiesLine); // affects(2,a)
  BOOLEAN extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine); // affects(1,12)
  BOOLEAN extractHasAffectsRelationship(); // affects(_,_)
  LIST_OF_AFFECTS_STMTS extractAffectsAnything();  // affects(a,_)
  LIST_OF_AFFECTS_STMTS extractAffectedByAnything(); // affects(_,a)
  BOOLEAN extractIsAffectsAnything(STMT_NUM t_usesLine); // affects(1,_)
  BOOLEAN extractIsAffectedByAnything(STMT_NUM t_modifiesLine); // affects(_,12)
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS appendAffectsList(MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS toAdd, MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS result);

  /**
  * A helper method to obtain the list of statements of a procedure from a particular statement number that resides in the procedure.
  * It will return the vector of stmts that belongs to that procedure.
  * @param t_t_stmtNum a statement number that belongs to the target procedure.
  * @return a vector of stmts that belongs to the target procedure.
  */
  LIST_OF_STMT_NUMS getListOfStmtsFromStmtNum(STMT_NUM t_stmtNum);

private:
  AffectsTable *m_affectsTable;
};

