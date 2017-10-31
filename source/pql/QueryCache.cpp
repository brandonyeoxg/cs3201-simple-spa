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

  m_allWhileStmtsWithVar = nullptr;
  m_allIfStmtsWithVar = nullptr;
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
  delete m_allWhileStmtsWithVar;
  delete m_allIfStmtsWithVar;
}

MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES * QueryCache::getAllNextStar() {
  return m_allNextStar;
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

MAP_OF_STMT_NUM_TO_VAR_INDEX * QueryCache::getAllWhileStmtsWithVar() {
  return m_allWhileStmtsWithVar;
}

MAP_OF_STMT_NUM_TO_VAR_INDEX * QueryCache::getAllIfStmtsWithVar() {
  return m_allIfStmtsWithVar;
}

void QueryCache::cacheAllNextStar(MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES t_allNextStar) {
  assert(m_allNextStar == nullptr); // prevent re-insertion
  m_allNextStar = &t_allNextStar;
}

void QueryCache::cacheAllFollows(MAP_OF_STMT_NUMS t_allFollows) {
  assert(m_allFollows == nullptr); // prevent re-insertion
  m_allFollows = &t_allFollows;
}

void QueryCache::cacheFollowsAnything(LIST_OF_STMT_NUMS t_followsAnything) {
  assert(m_followsAnything == nullptr); // prevent re-insertion
  m_followsAnything = &t_followsAnything;
}

void QueryCache::cacheFollowedByAnything(LIST_OF_STMT_NUMS t_followedByAnything) {
  assert(m_followedByAnything == nullptr); // prevent re-insertion
  m_followedByAnything = &t_followedByAnything;
}

void QueryCache::cacheChildrenOfAnything(LIST_OF_STMT_NUMS t_childrenOfAnything) {
  assert(m_childrenOfAnything == nullptr); // prevent re-insertion
  m_childrenOfAnything = &t_childrenOfAnything;
}
