#pragma once

#include "QueryCache.h"

QueryCache::QueryCache() {
  m_allNext = nullptr;
  m_allNextStar = nullptr;
  m_allLinesAfterAnyLine = nullptr;
  m_allLinesBeforeAnyLine = nullptr;

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

  m_allAffects = nullptr;
}

MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES * QueryCache::getAllNext() {
  return m_allNext;
}

MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES * QueryCache::getAllNextStar() {
  return m_allNextStar;
}

LIST_OF_PROG_LINES * QueryCache::getAllLinesAfter(PROG_LINE t_line) {
  if (m_allNextStar == nullptr) {
    return nullptr;
  }

  if (!isKeyInMap(t_line, *m_allNextStar)) {
    return new LIST_OF_PROG_LINES();
  }

  return &m_allNextStar->at(t_line);
}

LIST_OF_PROG_LINES * QueryCache::getAllLinesAfterAnyLine() {
  return m_allLinesAfterAnyLine;
}

LIST_OF_PROG_LINES * QueryCache::getAllLinesBeforeAnyLine() {
  return m_allLinesBeforeAnyLine;
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

MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS * QueryCache::getAllAffects() {
  return m_allAffects;
}

void QueryCache::cacheAllNext(MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES &t_allNext) {
  assert(m_allNext == nullptr); // prevent re-insertion
  m_allNext = &t_allNext;
}

void QueryCache::cacheAllNextStar(MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES &t_allNextStar) {
  assert(m_allNextStar == nullptr); // prevent re-insertion
  m_allNextStar = &t_allNextStar;
}

void QueryCache::cacheAllLinesAfterAnyLine(LIST_OF_PROG_LINES &t_allLinesAfterAnyLine) {
  assert(m_allLinesAfterAnyLine == nullptr); // prevent re-insertion
  m_allLinesAfterAnyLine = &t_allLinesAfterAnyLine;
}

void QueryCache::cacheAllLinesBeforeAnyLine(LIST_OF_PROG_LINES &t_allLinesBeforeAnyLine) {
  assert(m_allLinesBeforeAnyLine == nullptr); // prevent re-insertion
  m_allLinesBeforeAnyLine = &t_allLinesBeforeAnyLine;
}

void QueryCache::cacheFollowsAnything(LIST_OF_STMT_NUMS &t_followsAnything) {
  assert(m_followsAnything == nullptr); // prevent re-insertion
  m_followsAnything = &t_followsAnything;
}

void QueryCache::cacheFollowedByAnything(LIST_OF_STMT_NUMS &t_followedByAnything) {
  assert(m_followedByAnything == nullptr); // prevent re-insertion
  m_followedByAnything = &t_followedByAnything;
}

void QueryCache::cacheChildrenOfAnything(LIST_OF_STMT_NUMS &t_childrenOfAnything) {
  assert(m_childrenOfAnything == nullptr); // prevent re-insertion
  m_childrenOfAnything = &t_childrenOfAnything;
}

void QueryCache::cacheParentOfAnything(LIST_OF_STMT_NUMS &t_parentOfAnything) {
  assert(m_parentOfAnything == nullptr); // prevent re-insertion
  m_parentOfAnything = &t_parentOfAnything;
}

void QueryCache::cacheChildrenStarOfAnything(LIST_OF_STMT_NUMS &t_childrenStarOfAnything) {
  assert(m_childrenStarOfAnything == nullptr); // prevent re-insertion
  m_childrenStarOfAnything = &t_childrenStarOfAnything;
}

void QueryCache::cacheParentStarOfAnything(LIST_OF_STMT_NUMS &t_parentStarOfAnything) {
  assert(m_parentStarOfAnything == nullptr); // prevent re-insertion
  m_parentStarOfAnything = &t_parentStarOfAnything;
}

void QueryCache::cacheStmtUsesAnything(LIST_OF_STMT_NUMS &t_stmtUsesAnything) {
  assert(m_stmtUsesAnything == nullptr); // prevent re-insertion
  m_stmtUsesAnything = &t_stmtUsesAnything;
}

void QueryCache::cacheStmtModifiesAnything(LIST_OF_STMT_NUMS &t_stmtModifiesAnything) {
  assert(m_stmtModifiesAnything == nullptr); // prevent re-insertion
  m_stmtModifiesAnything = &t_stmtModifiesAnything;
}

void QueryCache::cacheAllWhileStmtsWithVar(MAP_OF_STMT_NUM_TO_VAR_INDEX &t_allWhileStmtsWithVar) {
  assert(m_allWhileStmtsWithVar == nullptr); // prevent re-insertion
  m_allWhileStmtsWithVar = &t_allWhileStmtsWithVar;
}

void QueryCache::cacheAllIfStmtsWithVar(MAP_OF_STMT_NUM_TO_VAR_INDEX &t_allIfStmtsWithVar) {
  assert(m_allIfStmtsWithVar == nullptr); // prevent re-insertion
  m_allIfStmtsWithVar = &t_allIfStmtsWithVar;
}

void QueryCache::cacheAllAffects(MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS & t_allAffects) {
  assert(m_allAffects == nullptr); // prevent re-insertion
  m_allAffects = &t_allAffects;
}
