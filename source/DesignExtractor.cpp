#pragma once
#include "DesignExtractor.h"
#include "ExtractorFactory.h"

DesignExtractor::DesignExtractor() {}

DesignExtractor::~DesignExtractor() {
  if (m_affectsExtractor != nullptr) {
    delete m_affectsExtractor;
    m_affectsExtractor = nullptr;
  }
}

void DesignExtractor::extractRestOfDesignAbstractions(PkbTablesOnly *t_pkb) {
  for (DESIGN_TYPE type = DESIGN_TYPE::FOLLOWS; type != DESIGN_TYPE::END_ENUM; type = DESIGN_TYPE(type + 1)) {
    Extractor *extractor = ExtractorFactory::makeExtractor(type, t_pkb);
    extractor->extractDesign();
    delete extractor;
  }

  m_affectsExtractor = (AffectsExtractor *)ExtractorFactory::makeExtractor(DESIGN_TYPE::AFFECTS, t_pkb);
}

///////////////////////////////////////////////////////
//  Affects Extractor
///////////////////////////////////////////////////////
MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS DesignExtractor::extractAllAffects() { // affects(a1,a2)
  return m_affectsExtractor->extractAllAffects();
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffectedBy(STMT_NUM t_modifiesLine) { // affects(2,a)
  return m_affectsExtractor->extractAffectedBy(t_modifiesLine);
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffects(STMT_NUM t_usesLine) { // affects(a,12)
  return m_affectsExtractor->extractAffects(t_usesLine);
}

BOOLEAN DesignExtractor::extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) { // affects(1,12)
  return m_affectsExtractor->extractIsAffects(t_modifiesLine, t_usesLine);
}

BOOLEAN DesignExtractor::extractHasAffectsRelationship() { // affects(_,_)
  return m_affectsExtractor->extractHasAffectsRelationship();
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffectsAnything() { // affects(a,_)
  return m_affectsExtractor->extractAffectsAnything();
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffectedByAnything() { // affects(_,a)
  return m_affectsExtractor->extractAffectedByAnything();
}

BOOLEAN DesignExtractor::extractIsAffectsAnything(STMT_NUM t_modifiesLine) { // affects(1,_)
  return m_affectsExtractor->extractIsAffectsAnything(t_modifiesLine);
}

BOOLEAN DesignExtractor::extractIsAffectedByAnything(STMT_NUM t_usesLines) { // affects(_,12)
  return m_affectsExtractor->extractIsAffectedByAnything(t_usesLines);
}

///////////////////////////////////////////////////////
//  Affects* Extractor
///////////////////////////////////////////////////////
MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS DesignExtractor::extractAllAffectsStar() { // affects*(a1,a2)
  return m_affectsExtractor->extractAllAffectsStar();
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffectedByStar(STMT_NUM t_modifiesLine) { // affects*(2,a)
  return m_affectsExtractor->extractAffectedByStar(t_modifiesLine);
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffectsStar(STMT_NUM t_usesLine) { // affects*(a,12)
  return m_affectsExtractor->extractAffectsStar(t_usesLine);
}

BOOLEAN DesignExtractor::extractIsAffectsStar(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) { // affects*(1,12)
  return m_affectsExtractor->extractIsAffectsStar(t_modifiesLine, t_usesLine);
}

BOOLEAN DesignExtractor::extractHasAffectsRelationshipStar() { // affects*(_,_)
  return m_affectsExtractor->extractHasAffectsRelationshipStar();
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffectsAnythingStar() {  // affects*(a,_)
  return m_affectsExtractor->extractAffectsAnything();
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffectedByAnythingStar() { // affects*(_,a)
  return m_affectsExtractor->extractAffectedByAnything();
}

BOOLEAN DesignExtractor::extractIsAffectsAnythingStar(STMT_NUM t_modifiesLine) { // affects*(1,_)
  return m_affectsExtractor->extractIsAffectsAnythingStar(t_modifiesLine);
}

BOOLEAN DesignExtractor::extractIsAffectedByAnythingStar(STMT_NUM t_usesLines) { // affects*(_,12)
  return m_affectsExtractor->extractIsAffectedByAnythingStar(t_usesLines);
}