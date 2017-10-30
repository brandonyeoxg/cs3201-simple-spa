#include "AffectsExtractor.h"

void AffectsExtractor::extractDesign() {  
}

SET_OF_AFFECTS AffectsExtractor::extractAllAffects() { // affects(a1,a2)
  return{};
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffects(STMT_NUM t_modifiesLine) { // affects(2,a)
  return{};
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectedBy(STMT_NUM t_usesLine) { // affects(a,12)
  return{};
}

BOOLEAN AffectsExtractor::extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) { // affects(1,12)
  return false;
}

BOOLEAN AffectsExtractor::extractHasAffectsRelationship() { // affects(_,_)
  return false;
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectsAnything() { // affects(a,_)
  return{};
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectedByAnything() { // affects(_,a)
  return{};
}

BOOLEAN AffectsExtractor::extractIsAffectsAnything(STMT_NUM t_modifiesLine) { // affects(1,_)
  return false;
}

BOOLEAN AffectsExtractor::extractIsAffectedByAnything(STMT_NUM t_usesLines) { // affects(_,12)
  return false;
}

SET_OF_AFFECTS AffectsExtractor::extractAllAffectsStar() { // affects*(a1,a2)
  return{};
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectsStar(STMT_NUM t_modifiesLine) { // affects*(2,a)
  return{};
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectedByStar(STMT_NUM t_usesLine) { // affects*(a,12)
  return{};
}

BOOLEAN AffectsExtractor::extractIsAffectsStar(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) { // affects*(1,12)
  return false;
}

BOOLEAN AffectsExtractor::extractHasAffectsRelationshipStar() { // affects*(_,_)
  return false;
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectsAnythingStar() {  // affects*(a,_)
  return{};
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectedByAnythingStar() { // affects*(_,a)
  return{};
}

BOOLEAN AffectsExtractor::extractIsAffectsAnythingStar(STMT_NUM t_modifiesLine) { // affects*(1,_)
  return false;
}

BOOLEAN AffectsExtractor::extractIsAffectedByAnythingStar(STMT_NUM t_usesLines) { // affects*(_,12)
  return false;
}