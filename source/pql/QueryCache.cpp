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
}

QueryCache::~QueryCache() {
  delete m_allNextStar;
  delete m_followsAnything;
  delete m_followedByAnything;
  delete m_childrenOfAnything;
  delete m_parentOfAnything;
  delete m_childrenStarOfAnything;
  delete m_parentStarOfAnything;
}

MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES * QueryCache::getAllNextStar() {
  return m_allNextStar;
}
