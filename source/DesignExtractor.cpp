#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include "PKB.h"
#include "DesignExtractor.h"

DesignExtractor::DesignExtractor(PKB* t_pkb) {
  m_pkb = t_pkb;
}

void DesignExtractor::extractRestOfDesignAbstractions() {
  extractParentStarAbstraction();
}


std::unordered_map<int, std::list<std::list<int>>> DesignExtractor::testExtractParentStarMap() {
  ParentTable* parentTable = m_pkb->getParentTable();
  auto parentMap = parentTable->getParentMap();
  std::unordered_map<int, std::list<std::list<int>>> parentStarMap;

  for (auto mapItr = parentMap.begin(); mapItr != parentMap.end(); mapItr++) {
    extractParentStarMap(mapItr, parentMap, parentStarMap);
  }

  return parentStarMap;
}

std::unordered_map<int, std::list<int>> DesignExtractor::testExtractParentedByStarMap() {
  ParentTable* parentTable = m_pkb->getParentTable();
  auto parentMap = parentTable->getParentMap();
  std::unordered_map<int, std::list<int>> parentedByStarMap;

  for (auto mapItr = parentMap.begin(); mapItr != parentMap.end(); mapItr++) {
    extractParentedByStarMap(mapItr, parentMap, parentedByStarMap);
  }
  return parentedByStarMap;
}

void DesignExtractor::extractParentStarAbstraction() {
  ParentTable* parentTable = m_pkb->getParentTable();
  auto parentMap = parentTable->getParentMap();
  std::unordered_map<int, std::list<std::list<int>>> parentStarMap;
  std::unordered_map<int, std::list<int>> parentedByStarMap;

  for (auto mapItr = parentMap.begin(); mapItr != parentMap.end(); mapItr++) {
    extractParentStarMap(mapItr, parentMap, parentStarMap);
    extractParentedByStarMap(mapItr, parentMap, parentedByStarMap);
  }
}

void DesignExtractor::extractParentStarMap(
    std::unordered_map<int, int>::iterator t_mapItr,
    std::unordered_map<int,int>& t_parentMap,
    std::unordered_map<int, std::list<std::list<int>>>& t_parentStarMap) {
  std::stack<int> stmtStack;
  int baseStmtNo = t_mapItr->first;
  stmtStack.push(baseStmtNo);
  std::list<int> stmtsOfParent;
  auto nextParentLink = t_parentMap.find(t_mapItr->second);
  // Forward algorithm
  int finalParentStmtNo = t_mapItr->second;
  while (nextParentLink != t_parentMap.end()) {
    stmtStack.push(nextParentLink->first);
    finalParentStmtNo = nextParentLink->second;
    nextParentLink = t_parentMap.find(nextParentLink->second);
  }
  while (!stmtStack.empty()) {
    stmtsOfParent.push_back(stmtStack.top());
    stmtStack.pop();
  }
  auto parentStarEntry = t_parentStarMap.find(finalParentStmtNo);
  if (parentStarEntry != t_parentStarMap.end()) {
    parentStarEntry->second.push_back(stmtsOfParent);
  } else { // New entry
    std::list<std::list<int>> parentOfList; // parent key -> value1, parent key -> value2
    parentOfList.push_back(stmtsOfParent);
    t_parentStarMap.insert({ t_mapItr->second, parentOfList });
  }
}

void DesignExtractor::extractParentedByStarMap(
    std::unordered_map<int, int>::iterator t_mapItr,
    std::unordered_map<int, int>& t_parentMap,
    std::unordered_map<int, std::list<int>>& t_parentedByStarMap) {
  int baseStmtNo = t_mapItr->first;
  std::list<int> stmtsOfParentedBy;
  stmtsOfParentedBy.push_back(t_mapItr->second);
  auto nextParentLink = t_parentMap.find(t_mapItr->second);
  while (nextParentLink != t_parentMap.end()) {
    stmtsOfParentedBy.push_back(nextParentLink->second);
    nextParentLink = t_parentMap.find(nextParentLink->second);
  }
  t_parentedByStarMap.insert({ baseStmtNo, stmtsOfParentedBy });
}