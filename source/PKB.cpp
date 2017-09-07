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
  m_procTable = new ProcTable();
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

void PKB::setFollowTable(std::unordered_map<int, std::vector<int>> &table) {
  m_followTable->setFollowTable(table);
}
/***********FollowTable Methods****************/
bool PKB::insertFollows(int s1, int s2) {
  return m_followTable->insertFollows(s1, s2);
}

bool PKB::isFollows(int s1, int s2) {
  return m_followTable->isFollows(s1, s2);
}

bool PKB::isFollowsStar(int s1, int s2) {
  return m_followTable->isFollowsStar(s1, s2);
}

int PKB::getFollows(int s1) {
  return m_followTable->getFollows(s1);
}

int PKB::getFollowedBy(int s2) {
  return m_followTable->getFollowedBy(s2);
}

std::vector<int> PKB::getFollowsStar(int s1) {
  return m_followTable->getFollowedByStar(s1);
}

std::vector<int> PKB::getFollowedByStar(int s2) {
  return m_followTable->getFollowedByStar(s2);
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
PROC_INDEX_NO PKB::insertProcToAST(ProcedureNode* t_node) {
  TNode* rootNode = m_programNode.getRoot();
  m_builder.linkParentToChild(rootNode, t_node);
  return m_procTable->insertProcByProcNode(t_node);
}

//TBD
ProcedureNode* PKB::getRootAST(PROC_INDEX_NO t_index) {
  return m_procTable->getProcWithIndex(t_index);
}
