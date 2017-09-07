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

/***********ParentTable Methods****************/
bool PKB::insertParent(int s1, int s2) {
  return m_parentTable->insertParent(s1, s2);
}

bool PKB::isParent(int s1, int s2) {
  return m_parentTable->isParent(s1, s2);
}

bool PKB::isParentStar(int s1, int s2) {
  return m_parentTable->isParentStar(s1, s2);
}

int PKB::getParentOf(int s2) {
  return m_parentTable->getParentOf(s2);
}

std::vector<int> PKB::getChildrenOf(int s1) {
  return m_parentTable->getChildrenOf(s1);
}

std::vector<int> PKB::getParentStarOf(int s2) {
  return m_parentTable->getParentStarOf(s2);
}

std::vector<int> PKB::getChildrenStarOf(int s1) {
  return m_parentTable->getChildrenStarOf(s1);
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
  std::unordered_map<int, std::vector<int>> finalResult;
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
