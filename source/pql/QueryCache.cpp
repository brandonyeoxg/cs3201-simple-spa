#pragma once

#include "QueryCache.h"

QueryCache::QueryCache() {
  m_cache = std::unordered_map<std::string, SET_OF_RESULTS>();
}

SET_OF_RESULTS QueryCache::getCache(Clause t_clause) {

  return SET_OF_RESULTS();
}

void QueryCache::cache(Clause t_clause, SET_OF_RESULTS t_results) {
  if (1) {  // isPattern
    Pattern *pattern = (Pattern*) &t_clause;
    //pattern->getStmt().getType()
  } else if (1) { //isRelation
    Relation *relation = (Relation*) &t_clause;
  }
}

bool QueryCache::isCacheable(Clause t_clause) {

  if (1) {  // isPattern
    Pattern *pattern = (Pattern*)&t_clause;
    return isPatternCacheable(*pattern);
  } else if (1) { //isRelation
    Relation *relation = (Relation*)&t_clause;
  } else {
    return false;
  }

  

  return false;
}

bool QueryCache::isPatternCacheable(Pattern t_pattern) {

  switch (t_pattern.getStmt().getType()) {
    case queryType::GType::WHILE: 
    case queryType::GType::IF: 
    case queryType::GType::ASGN:
      break;
    default:
      return false;
  }

  if (QueryUtil::isSynonymWithAnyPattern(t_pattern.getLeft(), t_pattern.getRight())) {
    return true;
  }

  return false;
}

bool QueryCache::isRelationCacheable(Relation t_relation) {

  switch (t_relation.getType()) {
    case queryType::RType::NEXT:
    case queryType::RType::NEXT_:
    case queryType::RType::FOLLOWS:
    case queryType::RType::FOLLOWS_:
    case queryType::RType::PARENT:
    case queryType::RType::PARENT_:
    case queryType::RType::CALLS:
    case queryType::RType::CALLS_:
    case queryType::RType::AFFECTS:
    case queryType::RType::AFFECTS_:
      break;
    case queryType::RType::USES:
    case queryType::RType::MODIFIES:
      return QueryUtil::hasOneLeftSynonym(t_relation.getG1(), t_relation.getG2())
        && QueryUtil::isUnderscore(t_relation.getG2());
    default:
      return false;
  }



  return false;
}
