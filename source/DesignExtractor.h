#pragma once
#include "ParentTable.h"
#include "PKB.h"
/**
 * Represents an extractor which extract complicated abstractions from AST.
 *
 * @author Brandon
 * @date 8/27/2017
 */
class DesignExtractor {
public:
  DesignExtractor(PKB* t_pkb);
  void extractRestOfDesignAbstractions();

private:
  void extractParentStarAbstraction();
  void extractParentStarMap(
    std::unordered_map<int, int>::iterator t_mapItr,
    std::unordered_map<int, int>& t_parentMap,
    std::unordered_map<int, std::list<std::list<int>>>& t_parentStarMap);
  void extractParentedByStarMap(
    std::unordered_map<int, int>::iterator t_mapItr,
    std::unordered_map<int, int>& t_parentMap,
    std::unordered_map<int, std::list<int>>& t_parentedByStarMap);
  PKB* m_pkb;
};