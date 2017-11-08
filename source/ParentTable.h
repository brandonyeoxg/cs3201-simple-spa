#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "GlobalTypeDef.h"
#include "InvalidArgumentException.h"

/**
* Represents the parent relationship table, which is a vector of unordered_maps to
* track which line is nested within which lines.
*
* @author pengcheng
* @date 8/27/2017
*
*/
class ParentTable {
public:
  void setChildMap(MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS &t_map);
  void setParentMap(MAP_OF_STMT_NUMS&t_map);
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS& getChildMap();
  MAP_OF_STMT_NUMS& getParentMap();
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS& getParentStarMap();
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS& getParentedByStarMap();

  ParentTable();

  BOOLEAN insertParent(int t_s1, int t_s2);
  BOOLEAN isParent(int t_s1, int t_s2);
  BOOLEAN isParentStar(int t_s1, int t_s2);
  STMT_NUM getParentOf(int t_s2);
  LIST_OF_STMT_NUMS getChildrenOf(int t_s1);
  LIST_OF_STMT_NUMS getParentStarOf(int t_s2);
  LIST_OF_STMT_NUMS getChildrenStarOf(int t_s1);
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS getAllParents();
  LIST_OF_STMT_NUMS getChildrenOfAnything(); //parent(_, s2)
  LIST_OF_STMT_NUMS getParentOfAnything(); //parent(s1, _)
  LIST_OF_STMT_NUMS getChildrenStarOfAnything(); //parent*(_, s2)
  LIST_OF_STMT_NUMS getParentStarOfAnything(); //parent*(s1, _)
  BOOLEAN hasParentRelationship(); //Parent (_, _)
  BOOLEAN hasParentStarRelationship(); //Parent* (_, _)
  BOOLEAN isChildrenOfAnything(int t_s2);  //parent(_, 2)
  BOOLEAN isParentOfAnything(int t_s1);  //parent(2, _)
  BOOLEAN isChildrenOfStarAnything(int t_s2);  //parent*(_, 2)
  BOOLEAN isParentOfStarAnything(int t_s1);  //parent*(2, _)
  void populateParentMatrix(TOTAL_NUMBER_OF_STMTS total);
  void populateParentStarMap();
private:
  MAP_OF_STMT_NUMS m_parentMap; //every statement can only have 1 parent.
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_childMap;
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_parentStarMap;
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS m_parentedByStarMap;
  BOOLEAN_MATRIX m_parentMatrix;
};
