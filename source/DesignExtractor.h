#pragma once
#include "PkbTablesOnly.h"
#include "AffectsExtractor.h"
/**
 * Represents a wrapper which extracts further complicated design abstractions from the PKB tables.
 * Designs are extracted through the ExtractorFactory.
 *
 * @author Brandon
 * @date 15/10/2017
 */
class DesignExtractor {
public:
  DesignExtractor();
  ~DesignExtractor();

  /*
  * Called to extract all design abstractions from the PKB table.
  */
  void extractRestOfDesignAbstractions(PkbTablesOnly *t_pkb);

  ///////////////////////////////////////////////////////
  //  Affects Extractor
  ///////////////////////////////////////////////////////
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS extractAllAffects(); // affects(a1,a2)
  LIST_OF_AFFECTS_STMTS extractAffects(STMT_NUM t_modifiesLine); // affects(2,a)
  LIST_OF_AFFECTS_STMTS extractAffectedBy(STMT_NUM t_usesLine); // affects(a,12)
  BOOLEAN extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine); // affects(1,12)
  BOOLEAN extractHasAffectsRelationship(); // affects(_,_)
  LIST_OF_AFFECTS_STMTS extractAffectsAnything();  // affects(a,_)
  LIST_OF_AFFECTS_STMTS extractAffectedByAnything(); // affects(_,a)
  BOOLEAN extractIsAffectsAnything(STMT_NUM t_modifiesLine); // affects(1,_)
  BOOLEAN extractIsAffectedByAnything(STMT_NUM t_usesLines); // affects(_,12)

  ///////////////////////////////////////////////////////
  //  Affects* Extractor
  ///////////////////////////////////////////////////////
  SET_OF_AFFECTS extractAllAffectsStar(); // affects*(a1,a2)
  LIST_OF_AFFECTS_STMTS extractAffectsStar(STMT_NUM t_modifiesLine); // affects*(2,a)
  LIST_OF_AFFECTS_STMTS extractAffectedByStar(STMT_NUM t_usesLine); // affects*(a,12)
  BOOLEAN extractIsAffectsStar(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine); // affects*(1,12)
  BOOLEAN extractHasAffectsRelationshipStar(); // affects*(_,_)
  LIST_OF_AFFECTS_STMTS extractAffectsAnythingStar();  // affects*(a,_)
  LIST_OF_AFFECTS_STMTS extractAffectedByAnythingStar(); // affects*(_,a)
  BOOLEAN extractIsAffectsAnythingStar(STMT_NUM t_modifiesLine); // affects*(1,_)
  BOOLEAN extractIsAffectedByAnythingStar(STMT_NUM t_usesLines); // affects*(_,12)
private:
  AffectsExtractor *m_affectsExtractor;
};