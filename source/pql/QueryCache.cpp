#pragma once

#include "QueryCache.h"

QueryCache::QueryCache() {
  m_cache = std::unordered_map<std::string, SET_OF_RESULTS_INDICES>();
}

SET_OF_RESULTS_INDICES *QueryCache::getCache(Clause *t_clause) {

  SET_OF_RESULTS_INDICES *results = nullptr;

  std::string clauseKey = getKey(*t_clause);

  if (isKeyInMap(clauseKey, m_cache)) {
    results = &m_cache.at(clauseKey);
  } else {
    results = getCacheFromOtherClauses(t_clause);
  }

  return results;
}

void QueryCache::cache(Clause *t_clause, SET_OF_RESULTS_INDICES t_results) {
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
      // PKB has O(1) time retrieval for (s1, s2)
      if (QueryUtil::hasTwoSynonyms(t_relation->getG1(), t_relation->getG2())) {
        return false;
      } else {
        break;
      }
    case queryType::RType::USES:
    case queryType::RType::MODIFIES:
      return QueryUtil::hasOneLeftSynonym(t_relation->getG1(), t_relation->getG2()) // (a1, _) or (a1, "x")
        || QueryUtil::hasTwoSynonyms(t_relation->getG1(), t_relation->getG2()); // (a1, v)
    default:
      return false;
  }

  if (QueryUtil::hasTwoSynonyms(t_relation->getG1(), t_relation->getG2())) {  // (s1, s2)
    return true;
  } else if (QueryUtil::hasOneLeftSynonym(t_relation->getG1(), t_relation->getG2())) {  
    // (s1, _) or (s1, "x") or (s1, 500)
    return true;
  } else if (QueryUtil::hasOneRightSynonym(t_relation->getG1(), t_relation->getG2())) {  
    // (_, s2) or ("x", s2) or (500, s2)
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
    Relation *relation = (Relation*) &t_clause;
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

  if (QueryUtil::hasTwoSynonyms(t_relation.getG1(), t_relation.getG2())) {
    key += getKeyWithPairGrammar(t_relation.getG1(), t_relation.getG2());
  } else {
    key += getKeyWithGrammar(t_relation.getG1());
    key += getKeyWithGrammar(t_relation.getG2());
  }

  return key;
}

// to be used if only 1 grammar is synonym
std::string QueryCache::getKeyWithGrammar(Grammar t_grammar) {

  if (QueryUtil::isSynonym(t_grammar)) {
    return "/s";
  } else if (QueryUtil::isUnderscore(t_grammar)) {
    return "/_";
  } else {
    return "/+" + t_grammar.getName();
  }
}

// to be used if both grammar are synonyms
std::string QueryCache::getKeyWithPairGrammar(Grammar t_grammar1, Grammar t_grammar2) {
  assert(QueryUtil::hasTwoSynonyms(t_grammar1, t_grammar2));

  if (QueryUtil::areBothSameSynonyms(t_grammar1, t_grammar2)) {
    return "/s/s";
  } else {
    return "/s1/s2"; // different synonyms
  }
}

SET_OF_RESULTS_INDICES * QueryCache::getCacheFromOtherClauses(Clause * t_clause) {
  if (t_clause->isRelationType()) { // isRelation
    return getCacheFromOtherRelations((Relation *)t_clause);
  }

  return nullptr;
}

SET_OF_RESULTS_INDICES * QueryCache::getCacheFromOtherRelations(Relation *t_relation) {
  SET_OF_RESULTS_INDICES *results = new SET_OF_RESULTS_INDICES();

  switch (t_relation->getType()) {
    case queryType::RType::NEXT_:
      return getCacheForNextStar(t_relation);
  }

  return nullptr;
}

SET_OF_RESULTS_INDICES * QueryCache::getCacheForNextStar(Relation * t_relation) {
  SET_OF_RESULTS_INDICES *results = new SET_OF_RESULTS_INDICES();

  // Next*(given_line, l)
  if (QueryUtil::hasOneRightSynonym(t_relation->getG1(), t_relation->getG2())
    && !QueryUtil::isUnderscore(t_relation->getG1())) {
    if (isKeyInMap(KEY_ALL_NEXT_STAR, m_cache)) {
      int g1Name = std::stoi(t_relation->getG1().getName());
      auto list = m_cache.at(KEY_ALL_NEXT_STAR).at(g1Name);
      results->insert({ g1Name, list });
      return results;
    }
  } else if (QueryUtil::hasOneRightSynonym(t_relation->getG1(), t_relation->getG2())
    && QueryUtil::isUnderscore(t_relation->getG1())) {
    // Next*(_, p2)
    if (isKeyInMap(KEY_NEXT_RIGHT_SYN, m_cache)) {
      return &m_cache.at(KEY_NEXT_RIGHT_SYN);  // use Next(_, p2)
    }
  } else if (QueryUtil::hasOneLeftSynonym(t_relation->getG1(), t_relation->getG2())
    && QueryUtil::isUnderscore(t_relation->getG2())) {
    // Next*(p1, _)
    if (isKeyInMap(KEY_NEXT_LEFT_SYN, m_cache)) {
      return &m_cache.at(KEY_NEXT_LEFT_SYN); // use Next(p1, _)
    }
  }

  return nullptr;
}
