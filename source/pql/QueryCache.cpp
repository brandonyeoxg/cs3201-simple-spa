#pragma once

#include "QueryCache.h"

QueryCache::QueryCache() {
  m_allNextStar = nullptr;

  m_followsAnything = nullptr;
  m_followedByAnything = nullptr;
  
  m_childrenOfAnything = nullptr;
  m_parentOfAnything = nullptr;
  m_childrenStarOfAnything = nullptr;
  m_parentStarOfAnything = nullptr;

  m_stmtUsesAnything = nullptr;
  m_stmtModifiesAnything = nullptr;
}

QueryCache::~QueryCache() {
  delete m_allNextStar;
  delete m_followsAnything;
  delete m_followedByAnything;
  delete m_childrenOfAnything;
  delete m_parentOfAnything;
  delete m_childrenStarOfAnything;
  delete m_parentStarOfAnything;
  delete m_stmtModifiesAnything;
}

MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES * QueryCache::getAllNextStar() {
  return m_allNextStar;
}

MAP_OF_STMT_NUMS * QueryCache::getAllFollows() {
  return m_allFollows;
}

LIST_OF_STMT_NUMS * QueryCache::getFollowsAnything() {
  return m_followsAnything;
}

LIST_OF_STMT_NUMS * QueryCache::getFollowedByAnything() {
  return m_followedByAnything;
}

LIST_OF_STMT_NUMS * QueryCache::getChildrenOfAnything() {
  return m_childrenOfAnything;
}

LIST_OF_STMT_NUMS * QueryCache::getParentOfAnything() {
  return m_parentOfAnything;
}

LIST_OF_STMT_NUMS * QueryCache::getChildrenStarOfAnything() {
  return m_childrenStarOfAnything;
}

LIST_OF_STMT_NUMS * QueryCache::getParentStarOfAnything() {
  return m_parentStarOfAnything;
}

LIST_OF_STMT_NUMS * QueryCache::getStmtUsesAnything() {
  return m_stmtUsesAnything;
}

LIST_OF_STMT_NUMS * QueryCache::getStmtModifiesAnything() {
  return m_stmtModifiesAnything;
}
