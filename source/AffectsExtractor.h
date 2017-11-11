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
  AffectsExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~AffectsExtractor() {};
  void extractDesign();
  ///////////////////////////////////////////////////////
  //  Affects
  ///////////////////////////////////////////////////////
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS extractAllAffects(); // affects(a1,a2)
  LIST_OF_AFFECTS_STMTS extractAffects(STMT_NUM t_usesLine); // affects(a,12)
  LIST_OF_AFFECTS_STMTS extractAffectedBy(STMT_NUM t_modifiesLine); // affects(2,a)
  BOOLEAN extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine); // affects(1,12)
  BOOLEAN extractHasAffectsRelationship(); // affects(_,_)
  LIST_OF_AFFECTS_STMTS extractAffectsAnything();  // affects(a,_)
  LIST_OF_AFFECTS_STMTS extractAffectedByAnything(); // affects(_,a)
  BOOLEAN extractIsAffectsAnything(STMT_NUM t_usesLine); // affects(1,_)
  BOOLEAN extractIsAffectedByAnything(STMT_NUM t_modifiesLine); // affects(_,12)

  ///////////////////////////////////////////////////////
  //  Affects* Extractor
  ///////////////////////////////////////////////////////
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS extractAllAffectsStar(); // affects*(a1,a2)
  LIST_OF_AFFECTS_STMTS extractAffectedByStar(STMT_NUM t_modifiesLine); // affects*(2,a)
  LIST_OF_AFFECTS_STMTS extractAffectsStar(STMT_NUM t_usesLine); // affects*(a,12)
  BOOLEAN extractIsAffectsStar(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine); // affects*(1,12)
  BOOLEAN extractHasAffectsRelationshipStar(); // affects*(_,_)
  LIST_OF_AFFECTS_STMTS extractAffectsAnythingStar();  // affects*(a,_)
  LIST_OF_AFFECTS_STMTS extractAffectedByAnythingStar(); // affects*(_,a)
  BOOLEAN extractIsAffectsAnythingStar(STMT_NUM t_modifiesLine); // affects*(1,_)
  BOOLEAN extractIsAffectedByAnythingStar(STMT_NUM t_usesLines); // affects*(_,12)

private:
  /**
  * A helper method to obtain the list of statements of a procedure from a particular statement number that resides in the procedure.
  * It will return the vector of stmts that belongs to that procedure.
  * @param t_t_stmtNum a statement number that belongs to the target procedure.
  * @return a vector of stmts that belongs to the target procedure.
  */
  LIST_OF_STMT_NUMS getListOfStmtsFromStmtNum(STMT_NUM t_stmtNum);
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS appendAffectsList(MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS toAdd, MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS result);
  PAIR_OF_AFFECTS_LIST getAffectsListsFromBounds(LIST_OF_STMT_NUMS bound);
};

