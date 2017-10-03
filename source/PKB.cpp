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
  m_modifiesP = new ModifiesP();
  m_usesP = new UsesP();
  m_callsTable = new CallsTable();
}

PKB::~PKB() {
  delete m_followTable;
  delete m_parentTable;
  delete m_varTable;
  delete m_procTable;
  delete m_assignTable;
  delete m_constantTable;
  delete m_statementTable;
  delete m_modifiesP;
  delete m_usesP;
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
PROC_INDEX PKB::insertProcedure(const PROC_NAME& t_procName) {
  return m_procTable->insertProc(t_procName);
}

void PKB::insertModifiesVariableNew(std::string t_varName, int t_curLineNum,
  std::list<STMT_NUM> t_nestedStmtLines) {
  insertModifiesForStmt(t_varName, t_curLineNum);
  for (auto& containerItr : t_nestedStmtLines) {
    insertModifiesForStmt(t_varName, containerItr);
  }
}

void PKB::insertUsesVariableNew(std::string t_varName, int t_curLineNum, std::list<STMT_NUM> t_nestedStmtLines) {
  insertUsesForStmt(t_varName, t_curLineNum);
  for (auto& containerItr : t_nestedStmtLines) {
    insertUsesForStmt(t_varName, containerItr);
  }
}

void PKB::insertModifiesProc(PROC_INDEX t_procIdx, const VAR_NAME& t_varName) {
  PROC_NAME pName = m_procTable->getProcNameFromIdx(t_procIdx);
  VAR_INDEX vIdx = m_varTable->getIndexOfVar(t_varName);
  m_modifiesP->insertModifiesP(t_procIdx, pName, vIdx, t_varName);
}

void PKB::insertUsesProc(PROC_INDEX t_procIdx, const VAR_NAME& t_varName) {
  PROC_NAME pName = m_procTable->getProcNameFromIdx(t_procIdx);
  VAR_INDEX vIdx = m_varTable->getIndexOfVar(t_varName);
  m_usesP->insertUsesP(t_procIdx, pName, vIdx, t_varName);
}

void PKB::insertAssignStmt(STMT_NUM t_lineNum, VAR_NAME t_varName) {
  insertStatementTypeTable(queryType::GType::ASGN, t_lineNum);
  insertTypeOfStatementTable(t_lineNum, queryType::GType::ASGN);
  VAR_INDEX vIdx = m_varTable->getIndexOfVar(t_varName);
  m_assignTable->insertAssignStmt(t_lineNum, vIdx, t_varName);
}

void PKB::insertCallStmt(PROC_INDEX t_procIdx, PROC_NAME t_proc2, STMT_NUM t_curLineNum) {
  insertStatementTypeTable(queryType::GType::CALL, t_curLineNum);
  insertTypeOfStatementTable(t_curLineNum, queryType::GType::CALL);
  PROC_NAME proc1 = m_procTable->getProcNameFromIdx(t_procIdx);
  m_callsTable->insertCalls(proc1, t_proc2);
}

STMT_NUM PKB::insertWhileStmt(std::string t_varName, std::list<STMT_NUM> t_nestedStmtLineNum, int t_curLineNum) {
  insertStatementTypeTable(queryType::GType::WHILE, t_curLineNum);
  insertTypeOfStatementTable(t_curLineNum, queryType::GType::WHILE);
  insertUsesVariableNew(t_varName, t_curLineNum, t_nestedStmtLineNum);
  return t_curLineNum;
}

STMT_NUM PKB::insertIfStmt(std::string t_varName, std::list<STMT_NUM> t_nestedStmtLineNum, int t_curLineNum) {
  insertStatementTypeTable(queryType::GType::IF, t_curLineNum);
  insertTypeOfStatementTable(t_curLineNum, queryType::GType::IF);
  insertUsesVariableNew(t_varName, t_curLineNum, t_nestedStmtLineNum);
  return t_curLineNum;
}

ConstantNode* PKB::insertConstant(std::string t_constVal, int t_curLineNum) {
  ConstantNode* constNode = m_builder.createConstant(t_curLineNum, atoi(t_constVal.c_str()));
  return constNode;
}

void PKB::insertConstant(CONSTANT_TERM t_constant) {
  m_constantTable->insertConstant(t_constant);
}

PlusNode* PKB::insertPlusOp(TNode* left, TNode* right, int t_curLineNum) {
  return m_builder.buildAddition(t_curLineNum, left, right);
}

bool PKB::insertFollowsRelation(std::list<STMT_NUM> t_stmtInStmtList, int t_curLineNum) {
  if (t_stmtInStmtList.empty()) {
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
bool PKB::isFollows(STMT_NUM t_s1, STMT_NUM t_s2) {
  return m_followTable->isFollows(t_s1, t_s2);
}

bool PKB::isFollowsStar(STMT_NUM t_s1, STMT_NUM t_s2) {
  return m_followTable->isFollowsStar(t_s1, t_s2);
}

STMT_NUM PKB::getFollows(STMT_NUM t_s1) {
  return m_followTable->getFollows(t_s1);
}

STMT_NUM PKB::getFollowedBy(STMT_NUM t_s2) {
  return m_followTable->getFollowedBy(t_s2);
}

LIST_OF_STMT_NUMS PKB::getFollowsStar(STMT_NUM t_s1) {
  return m_followTable->getFollowsStar(t_s1);
}

LIST_OF_STMT_NUMS PKB::getFollowedByStar(STMT_NUM t_s2) {
  return m_followTable->getFollowedByStar(t_s2);
}

std::unordered_map<STMT_NUM, STMT_NUM> PKB::getAllFollows() {
  return m_followTable->getAllFollows();
}

std::unordered_map<STMT_NUM, LIST_OF_STMT_NUMS> PKB::getAllFollowsStar() {
  return m_followTable->getAllFollowsStar();
}

LIST_OF_STMT_NUMS PKB::getFollowedByAnything() {
  return m_followTable->getFollowedByAnything();
}

LIST_OF_STMT_NUMS PKB::getFollowsAnything() {
  return m_followTable->getFollowsAnything();
}

bool PKB::hasFollowRelationship() {
  return m_followTable->hasFollowRelationship();
}

bool PKB::isFollowsAnything(STMT_NUM t_s2) {
  return m_followTable->isFollowsAnything(t_s2);
}

bool PKB::isFollowedByAnything(STMT_NUM t_s1) {
  return m_followTable->isFollowedByAnything(t_s1);
}
///////////////////////////////////////////////////////
//  ParentTable methods
///////////////////////////////////////////////////////

bool PKB::isParent(STMT_NUM t_s1, STMT_NUM t_s2) {
  return m_parentTable->isParent(t_s1, t_s2);
}

bool PKB::isParentStar(STMT_NUM t_s1, STMT_NUM t_s2) {
  return m_parentTable->isParentStar(t_s1, t_s2);
}

STMT_NUM PKB::getParentOf(STMT_NUM t_s2) {
  return m_parentTable->getParentOf(t_s2);
}

std::vector<int> PKB::getChildrenOf(STMT_NUM t_s1) {
  return m_parentTable->getChildrenOf(t_s1);
}

std::vector<int> PKB::getParentStarOf(STMT_NUM t_s2) {
  return m_parentTable->getParentStarOf(t_s2);
}

std::vector<int> PKB::getChildrenStarOf(STMT_NUM t_s1) {
  return m_parentTable->getChildrenStarOf(t_s1);
}

std::unordered_map<STMT_NUM, LIST_OF_STMT_NUMS> PKB::getAllParents() {
  return m_parentTable->getAllParents();
}

std::unordered_map<STMT_NUM, LIST_OF_STMT_NUMS> PKB::getAllParentsStar() {
  return m_parentTable->getParentStarMap();
}

LIST_OF_STMT_NUMS PKB::getChildrenOfAnything() {
  return m_parentTable->getChildrenOfAnything();
}

LIST_OF_STMT_NUMS PKB::getParentOfAnything() {
  return m_parentTable->getParentOfAnything();
}

LIST_OF_STMT_NUMS PKB::getChildrenStarOfAnything() {
  return m_parentTable->getChildrenStarOfAnything();
}

LIST_OF_STMT_NUMS PKB::getParentStarOfAnything() {
  return m_parentTable->getParentStarOfAnything();
}

bool PKB::hasParentRelationship() {
  return m_parentTable->hasParentRelationship();
}

bool PKB::hasParentStarRelationship() {
  return m_parentTable->hasParentStarRelationship();
}

bool PKB::isChildrenOfAnything(STMT_NUM t_s2) {
  return m_parentTable->isChildrenOfAnything(t_s2);
}

bool PKB::isParentOfAnything(STMT_NUM t_s1) {
  return m_parentTable->isParentOfAnything(t_s1);
}

bool PKB::isChildrenOfStarAnything(STMT_NUM t_s2) {
  return m_parentTable->isChildrenOfStarAnything(t_s2);
}

bool PKB::isParentOfStarAnything(STMT_NUM t_s1) {
  return m_parentTable->isParentOfStarAnything(t_s1);
}

//////////////////////////////////////////////////////////
//  statementTypeTable and typeOfStatementTable Methods
//////////////////////////////////////////////////////////
std::unordered_map<STMT_NUM, queryType::GType> PKB::getTypeOfStatementTable() {
  return m_statementTable->getTypeOfStatementTable();
}

bool PKB::insertTypeOfStatementTable(STMT_NUM t_lineNum, queryType::GType t_type) {
  return m_statementTable->insertTypeOfStatementTable(t_lineNum, t_type);
}
std::unordered_map<queryType::GType, LIST_OF_STMT_NUMS> PKB::getStatementTypeTable() {
  return m_statementTable->getStatementTypeTable();
}

bool PKB::insertStatementTypeTable(queryType::GType t_type, STMT_NUM t_lineNum) {
  return m_statementTable->insertStatementTypeTable(t_type, t_lineNum);
}

///////////////////////////////////////////////////////
//  VarTable methods
///////////////////////////////////////////////////////
STMT_NUM PKB::insertUsesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum) {
  return m_varTable->insertUsesForStmt(t_varName, t_lineNum);
}

STMT_NUM PKB::insertModifiesForStmt(VAR_NAME t_varName, STMT_NUM t_lineNum) {
  return m_varTable->insertModifiesForStmt(t_varName, t_lineNum);
}

bool PKB::isModifies(STMT_NUM t_lineNum, VAR_NAME t_varName) {
  return m_varTable->isModifies(t_lineNum, t_varName);
}

bool PKB::isUses(STMT_NUM t_lineNum, VAR_NAME t_varName) {
  return m_varTable->isUses(t_lineNum, t_varName);
}

LIST_OF_VAR_NAMES PKB::getModifies(STMT_NUM t_lineNum) {
  return m_varTable->getModifies(t_lineNum);
}

LIST_OF_VAR_NAMES PKB::getUses(STMT_NUM t_lineNum) {
  return m_varTable->getUses(t_lineNum);
}

LIST_OF_STMT_NUMS PKB::getStmtModifies(VAR_NAME t_varName) {
  return m_varTable->getStmtModifies(t_varName);
}

LIST_OF_STMT_NUMS PKB::getStmtUses(VAR_NAME t_varName) {
  return m_varTable->getStmtUses(t_varName);
}

std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> PKB::getAllStmtModifies() {
  return m_varTable->getAllStmtModifies();
}

std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> PKB::getAllStmtUses() {
  return m_varTable->getAllStmtUses();
}

int PKB::getIndexOfVar(VAR_NAME t_varName) {
  return m_varTable->getIndexOfVar(t_varName);
}

std::string PKB::getVarNameFromIndex(STMT_NUM t_index) {
  return m_varTable->getVarNameFromIndex(t_index);
}

bool PKB::isModifiesAnything(STMT_NUM t_lineNum) {
  return m_varTable->isModifiesAnything(t_lineNum);
}

bool PKB::isUsesAnything(STMT_NUM t_lineNum) {
  return m_varTable->isUsesAnything(t_lineNum);
}

LIST_OF_STMT_NUMS PKB::getStmtModifiesAnything() {
  return m_varTable->getStmtModifiesAnything();
}

LIST_OF_STMT_NUMS PKB::getStmtUsesAnything() {
  return m_varTable->getStmtUsesAnything();
}

LIST_OF_VAR_NAMES PKB::getAllVariables() {
  return m_varTable->getAllVariables();
}

///////////////////////////////////////////////////////
//  AssignTable methods
///////////////////////////////////////////////////////
AssignTable* PKB::getAssignTable() {
  return m_assignTable;
}

std::list<STMT_NUM> PKB::getAllAssignStmtListByVar(VAR_NAME& t_varName) {
  VAR_INDEX varIdx = m_varTable->getIndexOfVar(t_varName);
  if (varIdx == INVALID_INDEX) {
    return {};
  }
  return m_assignTable->getAllAssignStmtListByVar(varIdx);
}

LIST_OF_STMT_NUMS PKB::getAllAssignStmtList() {
  return m_assignTable->getAllAssignStmtList();
}

std::unordered_map<VAR_NAME, LIST_OF_STMT_NUMS> PKB::getAllVarNameWithAssignStmt() {
  return m_assignTable->getAllAssignVarNameWithStmtNum();
}

std::unordered_map<STMT_NUM, VAR_NAME> PKB::getAllAssignStmtWithVarName() {
  return m_assignTable->getAllAssignStmtWithVar();
}

///////////////////////////////////////////////////////
//  ParentTable methods
///////////////////////////////////////////////////////


std::list<VAR_NAME> PKB::getAllConstants() {
  return m_constantTable->getAllConstants();
}

///////////////////////////////////////////////////////
//  ProcTable methods
///////////////////////////////////////////////////////
ProcTable* PKB::getProcTable() {
  return m_procTable;
}

std::vector<PROC_NAME>& PKB::getAllProcsName() {
  return m_procTable->getAllProcsName();
}

///////////////////////////////////////////////////////
//  Pattern methods
///////////////////////////////////////////////////////
std::list<STMT_NUM> PKB::getAllAssignStmtByExactPattern(std::string t_pattern) {
  return PatternMatch::getInstance().getAllStmtNumWithExactPattern(t_pattern);
}

std::list<STMT_NUM> PKB::getAllAssignStmtBySubtreePattern(std::string t_pattern) {
  return PatternMatch::getInstance().getAllStmtNumWithSubtreePattern(t_pattern);
}

std::list<STMT_NUM> PKB::getAllAssignStmtByVar(std::string t_varName) {
  VAR_INDEX varIndex = m_varTable->getIndexOfVar(t_varName);
  if (varIndex == INVALID_INDEX) {
    return {};
  }
  return m_assignTable->getAllAssignStmtListByVar(varIndex);
}

std::list<STMT_NUM> PKB::getAllAssignStmtByVarAndExactPattern(std::string t_varName, std::string t_pattern) {
  std::list<STMT_NUM> list = {};
  VAR_INDEX varIndex = m_varTable->getIndexOfVar(t_varName);
  if (varIndex == INVALID_INDEX) {
    return list;
  }
  auto stmtNums = m_assignTable->getAllAssignStmtListByVar(varIndex);
  for (auto iterator : stmtNums) {
    if (PatternMatch::getInstance().isExactPatternInStmt(iterator, t_pattern)) {
      list.push_back(iterator);
    }
  }
  return list;
}

std::list<STMT_NUM> PKB::getAllAssignStmtByVarAndSubtreePattern(std::string t_varName, std::string t_pattern) {
  std::list<STMT_NUM> list = {};
  VAR_INDEX varIndex = m_varTable->getIndexOfVar(t_varName);

  if (varIndex == INVALID_INDEX) {
    return list;
  }

  std::list<STMT_NUM> stmtNums = m_assignTable->getAllAssignStmtListByVar(varIndex);

  for (auto iterator : stmtNums) {
    if (PatternMatch::getInstance().isSubtreePatternInStmt(iterator, t_pattern)) {
      list.push_back(iterator);
    }
  }

  return list;
}

std::unordered_map<STMT_NUM, VAR_NAME> PKB::getAllAssignStmtWithVarByExactPattern(std::string t_pattern) {
  std::list<STMT_NUM> stmtsWithMatch = PatternMatch::getInstance().getAllStmtNumWithExactPattern(t_pattern);

  std::unordered_map<STMT_NUM, VAR_NAME> mapStmtToVar = std::unordered_map<STMT_NUM, VAR_NAME>();

  for (auto stmtNum : stmtsWithMatch) {
    assert(getModifies(stmtNum).size() == 1);
    std::string varName = getModifies(stmtNum).at(0); // there should only be 1 variable modified for an assignment statement
    mapStmtToVar.insert({ stmtNum, varName });
  }

  return mapStmtToVar;
}

std::unordered_map<STMT_NUM, VAR_NAME> PKB::getAllAssignStmtWithVarBySubtreePattern(std::string t_pattern) {
  std::list<STMT_NUM> stmtsWithMatch = PatternMatch::getInstance().getAllStmtNumWithSubtreePattern(t_pattern);

  std::unordered_map<STMT_NUM, VAR_NAME> mapStmtToVar = std::unordered_map<STMT_NUM, VAR_NAME>();

  for (auto stmtNum : stmtsWithMatch) {
    assert(getModifies(stmtNum).size() == 1);
    std::string varName = getModifies(stmtNum).at(0); // there should only be 1 variable modified for an assignment statement
    mapStmtToVar.insert({ stmtNum, varName });
  }

  return mapStmtToVar;
}

void PKB::insertAssignStmtPattern(STMT_NUM t_stmtNum, std::vector<std::string> t_stmtTokens) {
  PatternMatch::getInstance().addAssignStmt(t_stmtNum, t_stmtTokens);
}

///////////////////////////////////////////////////////
//  CallsTable methods
///////////////////////////////////////////////////////
CallsTable* PKB::getCallsTable() {
  return m_callsTable;
}

bool PKB::isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2) {
  return m_callsTable->isCalls(t_proc1, t_proc2);
}

bool PKB::isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2) {
  return m_callsTable->isCallsStar(t_proc1, t_proc2);
}
LIST_OF_PROC_NAMES PKB::getCalls(PROC_NAME t_proc2) {
  return m_callsTable->getCalls(t_proc2);
}

LIST_OF_PROC_NAMES PKB::getCalledBy(PROC_NAME t_proc1) {
  return m_callsTable->getCalledBy(t_proc1);
}

LIST_OF_PROC_NAMES PKB::getCallsStar(PROC_NAME t_proc2) {
  return m_callsTable->getCallsStar(t_proc2);
}
LIST_OF_PROC_NAMES PKB::getCalledByStar(PROC_NAME t_proc1) {
  return m_callsTable->getCalledByStar(t_proc1);
}

std::unordered_map<PROC_NAME, PROC_NAME> PKB::getAllCalls() {
  return m_callsTable->getAllCalls();
}

std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> PKB::getAllCallsStar() {
  return m_callsTable->getAllCallsStar();
}

LIST_OF_PROC_NAMES PKB::getCallsAnything() {
  return m_callsTable->getCallsAnything();
}

LIST_OF_PROC_NAMES PKB::getCallsStarAnything() {
  return m_callsTable->getCallsStarAnything();
}

LIST_OF_PROC_NAMES PKB::getCalledByAnything() {
  return m_callsTable->getCalledByAnything();
}

LIST_OF_PROC_NAMES PKB::getCalledByStarAnything() {
  return m_callsTable->getCalledByStarAnything();
}

bool PKB::hasCallsRelationship() {
  return m_callsTable->hasCallsRelationship();
}

bool PKB::isCallsAnything(PROC_NAME t_proc1) {
  return m_callsTable->isCallsAnything(t_proc1);
}

bool PKB::isCalledByAnything(PROC_NAME t_proc2) {
  return m_callsTable->isCalledByAnything(t_proc2);
}
///////////////////////////////////////////////////////
//  ModifiesP methods
///////////////////////////////////////////////////////
ModifiesP* PKB::getModifiesP() {
  return m_modifiesP;
}

bool PKB::isModifiesP(const PROC_NAME& t_procName, const VAR_NAME& t_varName) {
  PROC_INDEX procIdx = m_procTable->getProcIdxFromName(t_procName);
  VAR_INDEX varIdx = m_varTable->getIndexOfVar(t_varName);
  return m_modifiesP->isModifiesP(procIdx, varIdx);
}

bool PKB::isModifiesInProc(const PROC_NAME& t_procName) {
  PROC_INDEX procIdx = m_procTable->getProcIdxFromName(t_procName);
  return m_modifiesP->isModifiesInProc(procIdx);
}

LIST_OF_VAR_NAMES PKB::getModifiesPVarNamesWithProcIdx(const PROC_NAME& t_procName) {
  PROC_INDEX procIdx = m_procTable->getProcIdxFromName(t_procName);
  return m_modifiesP->getVarNamesWithProcIdx(procIdx);
}

LIST_OF_PROC_NAMES PKB::getModifiesPProcNamesWithVarIdx(const VAR_NAME& t_varName) {
  VAR_INDEX varIdx = m_varTable->getIndexOfVar(t_varName);
  return m_modifiesP->getProcNamesWithVarIdx(varIdx);
}

MAP_OF_PROC_TO_VAR& PKB::getModifiesPAllProcToVar() {
  return m_modifiesP->getAllProcToVar();
}
LIST_OF_PROC_NAMES& PKB::getModifiesPAllProcNames() {
  return m_modifiesP->getAllProcNames();
}

///////////////////////////////////////////////////////
//  UsesP methods
///////////////////////////////////////////////////////
UsesP* PKB::getUsesP() {
  return m_usesP;
}

bool PKB::isUsesP(const PROC_NAME& t_procName, const VAR_NAME& t_varName) {
  PROC_INDEX procIdx = m_procTable->getProcIdxFromName(t_procName);
  VAR_INDEX varIdx = m_varTable->getIndexOfVar(t_varName);
  return m_usesP->isUsesP(procIdx, varIdx);
}

bool PKB::isUsesInProc(const PROC_NAME& t_procName) {
  PROC_INDEX procIdx = m_procTable->getProcIdxFromName(t_procName);
  return m_usesP->isUsesInProc(procIdx);
}

LIST_OF_VAR_NAMES PKB::getUsesPVarNamesWithProcIdx(const PROC_NAME& t_procName) {
  PROC_INDEX procIdx = m_procTable->getProcIdxFromName(t_procName);
  return m_usesP->getVarNamesWithProcIdx(procIdx);
}

LIST_OF_PROC_NAMES PKB::getUsesPProcNamesWithVarIdx(const VAR_NAME& t_varName) {
  VAR_INDEX varIdx = m_varTable->getIndexOfVar(t_varName);
  return m_usesP->getProcNamesWithVarIdx(varIdx);
}

MAP_OF_PROC_TO_VAR& PKB::getUsesPAllProcToVar() {
  return m_usesP->getAllProcToVar();
}
LIST_OF_PROC_NAMES& PKB::getUsesPAllProcNames() {
  return m_usesP->getAllProcNames();
}
