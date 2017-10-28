#pragma once
#include "DesignExtractor.h"
#include "ExtractorFactory.h"

DesignExtractor::DesignExtractor(PkbTablesOnly* t_pkb) {
  m_pkb = t_pkb;
}

void DesignExtractor::extractRestOfDesignAbstractions() {
  for (DESIGN_TYPE type = DESIGN_TYPE::FOLLOWS; type != DESIGN_TYPE::END_ENUM; type = DESIGN_TYPE(type + 1)) {
    Extractor *extractor = ExtractorFactory::makeExtractor(type, m_pkb);
    extractor->extractDesign();
    delete extractor;
  }
}

SET_OF_AFFECTS DesignExtractor::extractAllAffects() { // affects(a1,a2)
  return {};
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffects(STMT_NUM t_modifiesLine) { // affects(2,a)
  return {};
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffectedBy(STMT_NUM t_usesLine) { // affects(a,12)
  return {};
}

BOOLEAN DesignExtractor::extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) { // affects(1,12)
  return false;
}

BOOLEAN DesignExtractor::extractHasAffectsRelationship() { // affects(_,_)
  return false;
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffectsAnything() { // affects(a,_)
  return {};
}

LIST_OF_AFFECTS_STMTS DesignExtractor::extractAffectedByAnything() { // affects(_,a)
  return {};
}

BOOLEAN DesignExtractor::extractIsAffectsAnything(STMT_NUM t_modifiesLine) { // affects(1,_)
  return false;
}

BOOLEAN DesignExtractor::extractIsAffectedByAnything(STMT_NUM t_usesLines) { // affects(_,12)
  return false;
}
