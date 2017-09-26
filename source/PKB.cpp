#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB.h"
#include "nodes\TNode.h"
#include "ASTUtilities.h"

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
  m_constantTable = new ConstantTable();
  m_statementTable = new StatementTable();
}

///////////////////////////////////////////////////////
//  Getter and Setter methods 
///////////////////////////////////////////////////////
FollowTable* PKB:: getFollowTable() {
  return m_followTable;
}

ParentTable* PKB:: getParentTable() {
  return m_parentTable;
}

VarTable* PKB::getVarTable() {
  return m_varTable;
}

///////////////////////////////////////////////////////
//  PKB building methods
///////////////////////////////////////////////////////
StmtListNode* PKB::insertProcedure(std::string& t_procName) {
  ProcedureNode *procNode = m_builder.createProcedure(t_procName);
  // pkb build procedure unlink
  insertProcToAST(procNode);
  StmtListNode *stmtLst = m_builder.createStmtList(PROC_LINE_NUM);
  m_builder.linkParentToChild(procNode, stmtLst);
  return stmtLst;
}

VariableNode* PKB::insertModifiedVariable(std::string t_varName, int t_curLineNum, std::list<STMT_NUM> t_nestedStmLines) {
  VAR_INDEX varIndx = insertModifiesForStmt(t_varName, t_curLineNum);
  VariableNode* varNode = m_builder.createVariable(t_curLineNum, t_varName, varIndx);
  for (auto containerItr = t_nestedStmLines.begin(); containerItr != t_nestedStmLines.end(); containerItr++) {
    insertModifiesForStmt(varNode->getVarName(), (*containerItr));
  }
  return varNode;
}

VariableNode* PKB::insertUsesVariable(std::string t_varName, int t_curLineNum, std::list<STMT_NUM> t_nestedStmtLines) {
  VAR_INDEX index = insertUsesForStmt(t_varName, t_curLineNum);
  VariableNode* varNode = m_builder.createVariable(t_curLineNum, t_varName, index);
  for (auto containerItr = t_nestedStmtLines.begin(); containerItr != t_nestedStmtLines.end(); containerItr++) {
    insertUsesForStmt(t_varName, *containerItr);
  }
  return varNode;
}

void PKB::insertAssignStmt(VariableNode* t_varNode, TNode* t_exprNode, int t_curLineNum) {
  AssignNode* stmt = m_builder.buildAssignment(t_curLineNum, t_varNode, t_exprNode);
  insertStatementTypeTable(Grammar::GType::ASGN, t_curLineNum);
  insertTypeOfStatementTable(t_curLineNum, Grammar::GType::ASGN);
  insertAssignRelation(t_varNode->getVarIndex(), stmt);
}

STMT_NUM PKB::insertWhileStmt(std::string varName, std::list<STMT_NUM> m_nestedStmtLineNum, int t_curLineNum) {
  insertStatementTypeTable(Grammar::GType::WHILE, t_curLineNum);
  insertTypeOfStatementTable(t_curLineNum, Grammar::GType::WHILE);
  insertUsesVariable(varName, t_curLineNum, m_nestedStmtLineNum);
  return t_curLineNum;
}

StmtListNode* PKB::insertIfStmt(TNode* t_parentNode, VariableNode* t_varNode, int t_curLineNum) {
  return NULL;
}

ConstantNode* PKB::insertConstant(std::string t_constVal, int t_curLineNum) {
  ConstantNode* constNode = m_builder.createConstant(t_curLineNum, atoi(t_constVal.c_str()));
  insertConstant(t_constVal);
  return constNode;
}

PlusNode* PKB::insertPlusOp(TNode* left, TNode* right, int t_curLineNum) {
  return m_builder.buildAddition(t_curLineNum, left, right);
}

bool PKB::insertFollowsRelation(std::list<STMT_NUM> t_stmtInStmtList, int t_curLineNum) {
  if (t_stmtInStmtList.empty()) {
    /*m_followTable->insertFollows(TNode::NO_LINE_NUM, t_curLineNum);*/
    return false;
  }
  int prevStmtNum = t_stmtInStmtList.back();
  return m_followTable->insertFollows(prevStmtNum, t_curLineNum);
}

bool PKB::insertParentRelation(std::list<STMT_NUM> t_nestedStmtInStmtList, int t_curLineNum) {
  if (t_nestedStmtInStmtList.empty()) {
    return false;
  }
  int prevStmtNum = t_nestedStmtInStmtList.back();
  return m_parentTable->insertParent(prevStmtNum, t_curLineNum);
}
///////////////////////////////////////////////////////
//  FollowTable methods 
///////////////////////////////////////////////////////



bool PKB::insertFollows(int t_s1, int t_s2) {
  return m_followTable->insertFollows(t_s1, t_s2);
}

void PKB::populateParentStarMap() {
    m_parentTable->populateParentStarMap();
}

void PKB::populateParentedByStarMap() {
  auto parentMap = m_parentTable->getParentMap();
  std::unordered_map<int, std::list<std::list<int>>> parentStarMap;

  for (auto mapItr = parentMap.begin(); mapItr != parentMap.end(); mapItr++) {
    m_parentTable->populateParentedByStarMap(mapItr);
  }
}

bool PKB::isFollows(int t_s1, int t_s2) {
  return m_followTable->isFollows(t_s1, t_s2);
}

bool PKB::isFollowsStar(int t_s1, int t_s2) {
  return m_followTable->isFollowsStar(t_s1, t_s2);
}

int PKB::getFollows(int t_s1) {
  return m_followTable->getFollows(t_s1);
}

int PKB::getFollowedBy(int t_s2) {
  return m_followTable->getFollowedBy(t_s2);
}

std::vector<int> PKB::getFollowsStar(int t_s1) {
  return m_followTable->getFollowsStar(t_s1);
}

std::vector<int> PKB::getFollowedByStar(int t_s2) {
  return m_followTable->getFollowedByStar(t_s2);
}

std::unordered_map<int, int> PKB::getAllFollows() {
  return m_followTable->getAllFollows();
}

std::unordered_map<int, std::vector<int>> PKB::getAllFollowsStar() {
  return m_followTable->getAllFollowsStar();
}

std::vector<int> PKB::getFollowedByAnything() {
  return m_followTable->getFollowedByAnything();
}

std::vector<int> PKB::getFollowsAnything() {
  return m_followTable->getFollowsAnything();
}

bool PKB::hasFollowRelationship() {
  return m_followTable->hasFollowRelationship();
}

bool PKB::isFollowsAnything(int t_s2) {
  return m_followTable->isFollowsAnything(t_s2);
}

bool PKB::isFollowedByAnything(int t_s1) {
  return m_followTable->isFollowedByAnything(t_s1);
}
///////////////////////////////////////////////////////
//  ParentTable methods 
///////////////////////////////////////////////////////
bool PKB::insertParent(int t_s1, int t_s2) {
  return m_parentTable->insertParent(t_s1, t_s2);
}

bool PKB::isParent(int t_s1, int t_s2) {
  return m_parentTable->isParent(t_s1, t_s2);
}

bool PKB::isParentStar(int t_s1, int t_s2) {
  return m_parentTable->isParentStar(t_s1, t_s2);
}

int PKB::getParentOf(int t_s2) {
  return m_parentTable->getParentOf(t_s2);
}

std::vector<int> PKB::getChildrenOf(int t_s1) {
  return m_parentTable->getChildrenOf(t_s1);
}

std::vector<int> PKB::getParentStarOf(int t_s2) {
  return m_parentTable->getParentStarOf(t_s2);
}

std::vector<int> PKB::getChildrenStarOf(int t_s1) {
  return m_parentTable->getChildrenStarOf(t_s1);
}

std::unordered_map<int, std::vector<int>> PKB::getAllParents() {
  return m_parentTable->getAllParents();
}

std::unordered_map<int, std::vector<int>> PKB::getAllParentsStar() {
  return m_parentTable->getParentStarMap();
}

std::vector<int> PKB::getChildrenOfAnything() {
  return m_parentTable->getChildrenOfAnything();
}

std::vector<int> PKB::getParentOfAnything() {
  return m_parentTable->getParentOfAnything();
}

std::vector<int> PKB::getChildrenStarOfAnything() {
  return m_parentTable->getChildrenStarOfAnything();
}

std::vector<int> PKB::getParentStarOfAnything() {
  return m_parentTable->getParentStarOfAnything();
}

bool PKB::hasParentRelationship() {
  return m_parentTable->hasParentRelationship();
}

bool PKB::hasParentStarRelationship() {
  return m_parentTable->hasParentStarRelationship();
}

bool PKB::isChildrenOfAnything(int t_s2) {
  return m_parentTable->isChildrenOfAnything(t_s2);
}

bool PKB::isParentOfAnything(int t_s1) {
  return m_parentTable->isParentOfAnything(t_s1);
}

bool PKB::isChildrenOfStarAnything(int t_s2) {
  return m_parentTable->isChildrenOfStarAnything(t_s2);
}

bool PKB::isParentOfStarAnything(int t_s1) {
  return m_parentTable->isParentOfStarAnything(t_s1);
}

//////////////////////////////////////////////////////////
//  statementTypeTable and typeOfStatementTable Methods
//////////////////////////////////////////////////////////
std::unordered_map<int, Grammar::GType> PKB::getTypeOfStatementTable() {
  return m_statementTable->getTypeOfStatementTable();
}

bool PKB::insertTypeOfStatementTable(int t_lineNum, Grammar::GType t_type) {
  return m_statementTable->insertTypeOfStatementTable(t_lineNum, t_type);
}
std::unordered_map<Grammar::GType, std::vector<int>> PKB::getStatementTypeTable() {
  return m_statementTable->getStatementTypeTable();
}

bool PKB::insertStatementTypeTable(Grammar::GType t_type, int t_lineNum) {
  return m_statementTable->insertStatementTypeTable(t_type, t_lineNum);
}

///////////////////////////////////////////////////////
//  VarTable methods 
///////////////////////////////////////////////////////
int PKB::insertUsesForStmt(std::string t_varName, int t_lineNum) {
  return m_varTable->insertUsesForStmt(t_varName, t_lineNum);
}

int PKB::insertModifiesForStmt(std::string t_varName, int t_lineNum) {
  return m_varTable->insertModifiesForStmt(t_varName, t_lineNum);
}

bool PKB::isModifies(int t_lineNum, std::string t_varName) {
  return m_varTable->isModifies(t_lineNum, t_varName);
}

bool PKB::isUses(int t_lineNum, std::string t_varName) {
  return m_varTable->isUses(t_lineNum, t_varName);
}

std::vector<std::string> PKB::getModifies(int t_lineNum) {
  return m_varTable->getModifies(t_lineNum);
}

std::vector<std::string> PKB::getUses(int t_lineNum) {
  return m_varTable->getUses(t_lineNum);
}

std::vector<int> PKB::getStmtModifies(std::string t_varName) {
  return m_varTable->getStmtModifies(t_varName);
}

std::vector<int> PKB::getStmtUses(std::string t_varName) {
  return m_varTable->getStmtUses(t_varName);
}

std::unordered_map<std::string, std::vector<int>> PKB::getAllStmtModifies() {
  return m_varTable->getAllStmtModifies();
}

std::unordered_map<std::string, std::vector<int>> PKB::getAllStmtUses() {
  return m_varTable->getAllStmtUses();
}

int PKB::getIndexOfVar(std::string t_varName) {
  return m_varTable->getIndexOfVar(t_varName);
}

std::string PKB::getVarNameFromIndex(int t_index) {
  return m_varTable->getVarNameFromIndex(t_index);
}

bool PKB::isModifiesAnything(int t_lineNum) {
  return m_varTable->isModifiesAnything(t_lineNum);
}

bool PKB::isUsesAnything(int t_lineNum) {
  return m_varTable->isUsesAnything(t_lineNum);
}

std::vector<int> PKB::getStmtModifiesAnything() {
  return m_varTable->getStmtModifiesAnything();
}

std::vector<int> PKB::getStmtUsesAnything() {
  return m_varTable->getStmtUsesAnything();
}

std::vector<std::string> PKB::getAllVariables() {
  return m_varTable->getAllVariables();
}

///////////////////////////////////////////////////////
//  AssignTable methods 
///////////////////////////////////////////////////////

VAR_INDEX PKB::insertAssignRelation(const VAR_INDEX& t_index, AssignNode* t_node) {
  return m_assignTable->insertAssignRelation(t_index, t_node);
}

std::list<STMT_NUM> PKB::getAllAssignStmtListByVar(VAR_NAME& t_varName) {
  VAR_INDEX varIdx = m_varTable->getIndexOfVar(t_varName);
  if (varIdx == INVALID_INDEX) {
    return std::list<STMT_NUM>();
  }
  return m_assignTable->getAllAssignStmtListByVar(varIdx);
}

std::list<STMT_NUM> PKB::getAllAssignStmtList() {
  return m_assignTable->getAllAssignStmtList();
}

std::unordered_map<std::string, std::list<STMT_NUM>> PKB::getAllVarNameWithAssignStmt() {
  return m_assignTable->getAllVarInWithAssignStmtNum();
}

std::unordered_map<STMT_NUM, VAR_NAME> PKB::getAllAssignStmtWithVarName() {
  return m_assignTable->getAllAssignStmtWithVar();
}


void PKB::populateAssignTableAbstractions() {
  m_assignTable->populateAssignToVarMap(m_varTable);
}

///////////////////////////////////////////////////////
//  ParentTable methods 
///////////////////////////////////////////////////////
int PKB::insertConstant(std::string t_constant) {
  return m_constantTable->insertConstant(t_constant);
}

std::list<std::string> PKB::getAllConstants() {
  return m_constantTable->getAllConstants();
}

///////////////////////////////////////////////////////
//  ProcTable methods
///////////////////////////////////////////////////////
ProcTable* PKB::getProcTable() {
  return m_procTable;
}

bool PKB::insertProcModifies(PROC_INDEX& t_procIdx, std::string& t_varIdx) {
  return m_procTable->insertModifies(t_procIdx, t_varIdx);
}
bool PKB::insertProcUses(PROC_INDEX& t_procIdx, std::string& t_varIdx) {
  return m_procTable->insertUses(t_procIdx, t_varIdx);
}

void PKB::convertProcSetToList() {
  m_procTable->convertProcTableSetToList();
}

bool PKB::isModifies(std::string& t_procName, std::string t_varName) {
  return m_procTable->isModifies(t_procName, t_varName);
}

std::list<std::string>& PKB::getVarOfProcModifies(PROC_INDEX& t_procIdx) {
  return m_procTable->getVarFromProcModifies(t_procIdx);
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

bool PKB::isUses(std::string& t_procName, std::string& t_varName) {
  return m_procTable->isUses(t_procName, t_varName);
}

std::list<std::string>& PKB::getVarOfProcUses(PROC_INDEX& t_procIdx) {
  return m_procTable->getVarFromProcUses(t_procIdx);
}

///////////////////////////////////////////////////////
//  Pattern methods
///////////////////////////////////////////////////////
std::list<STMT_NUM> PKB::getAssignStmtByVarPattern(std::string t_varName, std::string pattern, bool t_isExact) {
  VAR_INDEX index = m_varTable->getIndexOfVar(t_varName);
  if (index == INVALID_INDEX) {
    return std::list<STMT_NUM>();
  }
  std::list<AssignData> aItr = m_assignTable->getAssignDataByVar(index);
  if (aItr.empty()) {
    return std::list<STMT_NUM>();
  }
  std::list<STMT_NUM> output;
  for (auto& assignData : aItr) {
    TNode* opNode = assignData.m_assignNode->getRightChild();
    if (t_isExact && ASTUtilities::matchExact(opNode, pattern)) {
      output.push_back(opNode->getLineNum());
      continue;
    }
    if (!t_isExact && ASTUtilities::matchSubtree(opNode, pattern)) {
      output.push_back(opNode->getLineNum());
    }
  }
  return output;
}

std::unordered_map<STMT_NUM, VAR_NAME> PKB::getAllAssignStmtAndVarByPattern(std::string t_pattern, bool t_isExact) {
  std::unordered_map<STMT_NUM, VAR_NAME> output;
  for (auto& aItr : m_assignTable->getAssignData()) {
    TNode* oprNode = aItr.m_assignNode->getRightChild();
    VAR_NAME varName = ((VariableNode*)aItr.m_assignNode->getLeftChild())->getVarName();
    if (t_isExact && ASTUtilities::matchExact(oprNode, t_pattern)) {
      output.emplace(aItr.m_assignStmt, varName);
      continue;
    }
    if (!t_isExact && ASTUtilities::matchSubtree(oprNode, t_pattern)) {
      output.emplace(aItr.m_assignStmt, varName);
    }
  }
  return output;
}

std::list<STMT_NUM> PKB::getAllAssignStmtByExactPattern(std::string t_pattern) {
  std::list<STMT_NUM> list = std::list<STMT_NUM>();
  for (auto& iterator : m_assignTable->getAssignData()) {
    if (ASTUtilities::matchExact(iterator.m_assignNode->getRightChild(), t_pattern)) {
      list.push_back(iterator.m_assignStmt);
    }
  }
  return list;
}

std::list<STMT_NUM> PKB::getAllAssignStmtBySubtreePattern(std::string t_pattern) {
  std::list<STMT_NUM> list = std::list<STMT_NUM>();
  for (auto& iterator : m_assignTable->getAssignData()) {
    if (ASTUtilities::matchSubtree(iterator.m_assignNode->getRightChild(), t_pattern)) {
      list.push_back(iterator.m_assignStmt);
    }
  }
  return list;
}


//TBD
PROC_INDEX PKB::insertProcToAST(ProcedureNode* t_node) {
  TNode* rootNode = m_programNode.getRoot();
  m_builder.linkParentToChild(rootNode, t_node);
  return m_procTable->insertProcByProcNode(t_node);
}

//TBD
ProcedureNode* PKB::getRootAST(PROC_INDEX t_index) {
  return m_procTable->getProcNodeWithIdx(t_index);
}
