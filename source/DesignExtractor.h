#pragma once
#include "PkbTablesOnly.h"
/**
 * Represents a wrapper which extracts further complicated design abstractions from the PKB tables.
 * Designs are extracted through the ExtractorFactory.
 *
 * @author Brandon
 * @date 15/10/2017
 */
class DesignExtractor {
public:
  DesignExtractor(PkbTablesOnly* t_pkb);

  /*
  * Called to extract all design abstractions from the PKB table.
  */
  void extractRestOfDesignAbstractions();

  ///////////////////////////////////////////////////////
  //  Affects Extractor
  ///////////////////////////////////////////////////////
  SET_OF_AFFECTS extractAllAffects(); // affects(a1,a2)
  LIST_OF_AFFECTS_STMTS extractAffects(STMT_NUM t_modifiesLine); // affects(2,a)
  LIST_OF_AFFECTS_STMTS extractAffectedBy(STMT_NUM t_usesLine); // affects(a,12)
  BOOLEAN extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine); // affects(1,12)
  BOOLEAN extractHasAffectsRelationship(); // affects(_,_)
  LIST_OF_AFFECTS_STMTS extractAffectsAnything();  // affects(a,_)
  LIST_OF_AFFECTS_STMTS extractAffectedByAnything(); // affects(_,a)
  BOOLEAN extractIsAffectsAnything(STMT_NUM t_modifiesLine); // affects(1,_)
  BOOLEAN extractIsAffectedByAnything(STMT_NUM t_usesLines); // affects(_,12)

private:
  PkbTablesOnly* m_pkb;
};