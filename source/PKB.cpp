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
  m_assignTable = new AssignTable();
  std::unordered_map<int, std::string> m_statementTypeTable;
  std::unordered_map<std::string, std::vector<int>> m_typeOfStatementTable;
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

//statementTypeTable and typeOfStatementTable Methods
std::unordered_map<int, Grammar::GType> PKB::getTypeOfStatementTable() {
  return m_typeOfStatementTable;
}

bool PKB::insertTypeOfStatementTable(int line_num, Grammar::GType t_type) {
  //if line_num already exists as key in table, return false.
  if (m_typeOfStatementTable.find(line_num) != m_typeOfStatementTable.end()) {
    return false;
  } else {
    m_typeOfStatementTable.emplace(line_num, t_type);
    return true;
  }
}
std::unordered_map<Grammar::GType, std::vector<int>> PKB::getStatementTypeTable() {
  return m_statementTypeTable;
}

bool PKB::insertStatementTypeTable(Grammar::GType t_type, int line_num) {
  //if type does not exist as key
  if (m_statementTypeTable.find(t_type) == m_statementTypeTable.end()) {
    std::vector<int> lineNums;
    lineNums.push_back(line_num);
    m_statementTypeTable.emplace(t_type, lineNums);
    return true;
  } else {  //type already exists
    std::vector<int> lineNums = m_statementTypeTable[t_type];
    if (std::find(lineNums.begin(), lineNums.end(), line_num) != lineNums.end()) {
      //return false if duplicate already exists in table.
      return false;
    }
    //else just push to the vector.
    lineNums.push_back(line_num);
    m_statementTypeTable[t_type] = lineNums;
    return true;
  }
}

/***********FollowTable Methods****************/
int PKB::insertUsesForStmt(int index, std::string varName, int lineNum) {
  return m_varTable->insertUsesForStmt(index, varName, lineNum);
}

int PKB::insertUsesForStmt(std::string t_varName, int t_lineNum) {
  return m_varTable->insertUsesForStmt(t_varName, t_lineNum);
}

int PKB::insertModifiesForStmt(std::string t_varName, int t_lineNum) {
  return m_varTable->insertModifiesForStmt(t_varName, t_lineNum);
}

int PKB::insertModifiesForStmt(int index, std::string varName, int lineNum) {
  return m_varTable->insertModifiesForStmt(index, varName, lineNum);
}

bool PKB::isModifies(int lineNum, std::string varName) {
  return m_varTable->isModifies(lineNum, varName);
}

bool PKB::isUses(int lineNum, std::string varName) {
  return m_varTable->isUses(lineNum, varName);
}

std::vector<std::string> PKB::getModifies(int lineNum) {
  return m_varTable->getModifies(lineNum);
}

std::vector<std::string> PKB::getUses(int lineNum) {
  return m_varTable->getUses(lineNum);
}

std::vector<int> PKB::getStmtModifies(std::string varName) {
  return m_varTable->getStmtModifies(varName);
}

std::vector<int> PKB::getStmtUses(std::string varName) {
  return m_varTable->getStmtUses(varName);
}

std::unordered_map<std::string, std::vector<int>> PKB::getAllStmtModifies() {
  return m_varTable->getAllStmtModifies();
}

std::unordered_map<std::string, std::vector<int>> PKB::getAllStmtUses() {
  return m_varTable->getAllStmtUses();
}

int PKB::getIndexOfVar(std::string varName) {
  return m_varTable->getIndexOfVar(varName);
}

/***********AssignTable Methods****************/
VAR_INDEX_NO PKB::insertAssignRelation(const VAR_INDEX_NO& t_index, AssignNode* t_node) {
  return m_assignTable->insertAssignRelation(t_index, t_node);
}

std::list<STMT_NO> PKB::getAllStmtListByVar(VAR_INDEX_NO &t_index) {
  return m_assignTable->getAllStmtListByVar(t_index);
}

std::list<STMT_NO> PKB::getAllStmtList() {
  return m_assignTable->getAllStmtList();
}

std::unordered_map<std::string, std::list<STMT_NO>> PKB::getAllAssignStmtWithVar() {
  return m_assignTable->getAllAssignStmtWithVar();
}

/***********ProcTable Methods****************/
bool PKB::insertProcModifies(PROC_INDEX_NO& t_procIdx, std::string& t_varIdx) {
  return m_procTable->insertModifies(t_procIdx, t_varIdx);
}
bool PKB::insertProcUses(PROC_INDEX_NO& t_procIdx, std::string& t_varIdx) {
  return m_procTable->insertUses(t_procIdx, t_varIdx);
}

void PKB::convertProcSetToList() {
  m_procTable->convertProcTableSetToList();
}

bool PKB::isModifies(PROC_INDEX_NO& t_procIdx, std::string t_varName) {
  return m_procTable->isModifies(t_procIdx, t_varName);
}

std::list<std::string>& PKB::getVarOfProcModifies(PROC_INDEX_NO& t_procIdx) {
  return m_procTable->getProcOfVarModifies(t_procIdx);
}

std::list<std::string>& PKB::getProcNameThatModifiesVar(std::string& t_varName) {
  return m_procTable->getProcNameThatModifiesVar(t_varName);
}

std::unordered_map<std::string, std::list<std::string>>& PKB::getProcAndVarModifies() {
  return m_procTable->getProcAndVarModifies();
}

bool PKB::isModifiesInProc(std::string& t_procName) {
  return m_procTable->isModifiesInProc(t_procName);
}

std::list<std::string>& PKB::getProcThatModifies() {
  return m_procTable->getProcNameThatModifies();
}

bool PKB::isUses(PROC_INDEX_NO& t_procIdx, std::string& t_varName) {
  return m_procTable->isUses(t_procIdx, t_varName);
}

std::list<std::string>& PKB::getVarOfProcUses(PROC_INDEX_NO& t_procIdx) {
  return m_procTable->getProcOfVarUses(t_procIdx);
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
  std::unordered_map<std::string, std::vector<int>> result;
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
  return m_procTable->getProcNodeWithIdx(t_index);
}
