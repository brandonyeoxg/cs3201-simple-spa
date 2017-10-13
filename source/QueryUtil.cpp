#include "QueryUtil.h"

bool QueryUtil::isAllUnderscores(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() == "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() == "_";
}

bool QueryUtil::hasNoSynonyms(Grammar t_g1, Grammar t_g2) {
  return (t_g1.getType() == queryType::GType::STMT_NO || t_g1.getType() == queryType::GType::STR) && (t_g2.getType() == queryType::GType::STMT_NO || t_g2.getType() == queryType::GType::STR);
}

bool QueryUtil::hasOneRightSynonym(Grammar t_g1, Grammar t_g2) {
  return (t_g1.getType() == queryType::GType::STMT_NO || t_g1.getType() == queryType::GType::STR) && t_g2.getType() != queryType::GType::STMT_NO && t_g2.getType() != queryType::GType::STR;
}

bool QueryUtil::hasOneLeftSynonym(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() != queryType::GType::STMT_NO && t_g1.getType() != queryType::GType::STR && (t_g2.getType() == queryType::GType::STMT_NO || t_g2.getType() == queryType::GType::STR);
}

bool QueryUtil::hasTwoSynonyms(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() != queryType::GType::STMT_NO && t_g1.getType() != queryType::GType::STR && t_g2.getType() != queryType::GType::STMT_NO && t_g2.getType() != queryType::GType::STR;
}

bool QueryUtil::isAnythingWithAnyPattern(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() == "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() == "_";
}

bool QueryUtil::isAnythingWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() == "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && t_isExact;
}

bool QueryUtil::isAnythingWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() == "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && !t_isExact;
}

bool QueryUtil::isVarWithAnyPattern(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() != "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() == "_";
}

bool QueryUtil::isVarWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() != "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && t_isExact;
}

bool QueryUtil::isVarWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() != "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && !t_isExact;
}

bool QueryUtil::isSynonymWithAnyPattern(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() == queryType::GType::VAR && t_g2.getType() == queryType::GType::STR && t_g2.getName() == "_";
}

bool QueryUtil::isSynonymWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::VAR && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && t_isExact;
}

bool QueryUtil::isSynonymWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::VAR && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && !t_isExact;
}