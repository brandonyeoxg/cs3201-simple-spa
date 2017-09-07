#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB.h"
#include "nodes\TNode.h"

/**
* A constructor.
* Instantiates one VarTable, FollowTable, ParentTable each.
*/
PKB::PKB() {
  m_followTable = new FollowTable();
  m_parentTable = new ParentTable();
  m_varTable = new VarTable();
}

/* Getter methods*/
FollowTable* PKB:: getFollowTable() {
  return m_followTable;
}

ParentTable* PKB:: getParentTable() {
  return m_parentTable;
}

VarTable* PKB::getVarTable() {
  return m_varTable;
}

/**
* STUB. To be removed.
*/
std::unordered_map<int, std::vector<int>> PKB:: returnFollowTable(int s1, int s2) {
  std::unordered_map<int, std::vector<int>> finalResult;
  return finalResult;
}

/**
* Method that returns an unordered_map for parent relationship.
* @param s1 an integer argument (-1 denotes it being the querying variable).
* @param s2 an integer argument (-2 denotes it being the querying variable).
*/
std::unordered_map<int, std::vector<int>> PKB::returnParentTable(int s1, int s2) {
  ParentTable* table = getParentTable();

  //check s1 and s2
  //if both are querying variables, return the whole followTable i.e. every possible follow relationships
  if (s1 == VARIABLE_S1 && s2 == VARIABLE_S2) {
    //if both input are querying variable, just return the entire map.
    return table->getParentTable();
  }

  std::unordered_map<int, std::vector<int>> finalResult;
  //if only s1 is querying variable
  if (s1 == VARIABLE_S1 && s2 != VARIABLE_S2) {
    std::vector<int> s1Results = table->getS1(s2);
    finalResult.emplace(s2, s1Results);
  } else if (s1 != VARIABLE_S1 && s2 == VARIABLE_S2) {
    std::vector<int> s2Results = table->getS2(s1);
    finalResult.emplace(s1, s2Results);
  } else if (s1 != VARIABLE_S1 && s2 != VARIABLE_S2) {
    //both s1 and s2 are not querying variables
    //e.g. follow(3,4)
    //just return the map with 3 as key
    std::vector<int> s2Results = table->getS2(s1);
    finalResult.emplace(s1, s2Results);
  }

  return finalResult;
}


/**
* Method that returns an unordered_map for variable query.
* @param s1 an integer argument (-1 denotes it being the querying variable).
* @param s2 an integer argument (-2 denotes it being the querying variable).
*/
std::unordered_map<std::string, std::vector<int>> PKB::returnVarTable(std::string var) {
  VarTable* table = getVarTable();
  std::unordered_map<std::string, std::vector<int>> result;
  result.emplace(var, table->get(var));

  return result;
}

//TBD
int PKB::setProcToAST(PROC p, TNode* r) {
  return NULL;
}

//TBD
TNode* PKB::getRootAST(PROC p) {
  return NULL;
}
