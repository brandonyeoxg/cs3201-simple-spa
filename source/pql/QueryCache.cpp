#pragma once

#include "QueryCache.h"

QueryCache::QueryCache() {
  m_cache = std::unordered_map<std::string, SET_OF_RESULTS>();
}

SET_OF_RESULTS QueryCache::getCache(Clause *t_clause) {

  return SET_OF_RESULTS();
}

void QueryCache::cache(Clause *t_clause, SET_OF_RESULTS t_results) {
  assert(t_clause->isPatternType() || t_clause->isRelationType());
  assert(isCacheable(t_clause));

  std::string key = getKey(*t_clause);
  m_cache.insert({ key, t_results });
}

bool QueryCache::isCacheable(Clause *t_clause) {

  if (t_clause->isPatternType()) {  // isPattern
    return isPatternCacheable((Pattern *) t_clause);
  } else if (t_clause->isRelationType()) { // isRelation
    return isRelationCacheable((Relation *)t_clause);
  }

  return false;
}

bool QueryCache::isPatternCacheable(Pattern *t_pattern) {

  switch (t_pattern->getStmt().getType()) {
    case queryType::GType::WHILE: 
    case queryType::GType::IF: 
    case queryType::GType::ASGN:
      break;
    default:
      return false;
  }

  if (QueryUtil::isSynonymWithAnyPattern(t_pattern->getLeft(), t_pattern->getRight())) {
    return true;
  }

  return false;
}

bool QueryCache::isRelationCacheable(Relation * t_relation) {

  switch (t_relation->getType()) {
    case queryType::RType::NEXT:
    case queryType::RType::NEXT_:
    case queryType::RType::CALLS:
    case queryType::RType::CALLS_:
    case queryType::RType::AFFECTS:
    case queryType::RType::AFFECTS_:
      break;
    case queryType::RType::FOLLOWS:
    case queryType::RType::FOLLOWS_:
    case queryType::RType::PARENT:
    case queryType::RType::PARENT_:
      if (QueryUtil::hasTwoSynonyms(t_relation->getG1(), t_relation->getG2())) {
        return false;
      } else {
        break;
      }
    case queryType::RType::USES:
    case queryType::RType::MODIFIES:
      return QueryUtil::hasOneLeftSynonym(t_relation->getG1(), t_relation->getG2())
        && QueryUtil::isUnderscore(t_relation->getG2());
    default:
      return false;
  }

  if (QueryUtil::hasTwoSynonyms(t_relation->getG1(), t_relation->getG2())) {
    return true;
  } else if (QueryUtil::hasOneLeftSynonym(t_relation->getG1(), t_relation->getG2())
    && QueryUtil::isUnderscore(t_relation->getG2())) {
    return true;
  } else if (QueryUtil::hasOneRightSynonym(t_relation->getG1(), t_relation->getG2())
    && QueryUtil::isUnderscore(t_relation->getG1())) {
    return true; 
  }

  return false;
}

std::string QueryCache::getKey(Clause &t_clause) {
  assert(t_clause.isPatternType() || t_clause.isRelationType());

  if (t_clause.isPatternType()) {  // isPattern
    Pattern *pattern = (Pattern*) &t_clause;
    return getKeyWithPattern(*pattern);
  } else if (t_clause.isRelationType()) { //isRelation
    Relation *relation = (Relation*)&t_clause;
    return getKeyWithRelation(*relation);
  }
}

std::string QueryCache::getKeyWithPattern(Pattern t_pattern) {
  std::string key = "Pattern_";

  switch (t_pattern.getStmt().getType()) {
    case queryType::GType::WHILE:
      key += "While";
      break;
    case queryType::GType::IF:
      key += "If";
      break;
    case queryType::GType::ASGN:
      key += "Assign";
      break;
  }

  return key;
}

std::string QueryCache::getKeyWithRelation(Relation t_relation) {
  std::string key = t_relation.getTypeInString();

  key += getKeyWithGrammar(t_relation.getG1());
  key += getKeyWithGrammar(t_relation.getG2());

  return key;
}

std::string QueryCache::getKeyWithGrammar(Grammar t_grammar) {

  if (QueryUtil::isSynonym(t_grammar)) {
    return "s";
  } else if (QueryUtil::isUnderscore(t_grammar)) {
    return "_";
  } else {
    assert(false);  // should be either synonym or underscore only
  }
}