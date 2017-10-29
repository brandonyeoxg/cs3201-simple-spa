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

  SET_OF_AFFECTS extractAllAffects(); // affects(a1,a2)
  LIST_OF_AFFECTS_STMTS extractAffects(STMT_NUM t_usesLine); // affects(a,12)
  LIST_OF_AFFECTS_STMTS extractAffectedBy(STMT_NUM t_modifiesLine); // affects(2,a)
  BOOLEAN extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine); // affects(1,12)
  BOOLEAN extractHasAffectsRelationship(); // affects(_,_)
  LIST_OF_AFFECTS_STMTS extractAffectsAnything();  // affects(_,a)
  LIST_OF_AFFECTS_STMTS extractAffectedByAnything(); // affects(a,_)
  BOOLEAN extractIsAffectsAnything(STMT_NUM t_usesLine); // affects(_,12)
  BOOLEAN extractIsAffectedByAnything(STMT_NUM t_modifiesLine); // affects(1,_)
  void appendAffectsList(std::unordered_map<int, std::vector<int>> toAdd, std::unordered_map<int, std::vector<int>> result);

private:
  AffectsTable *m_affectsTable;
};

