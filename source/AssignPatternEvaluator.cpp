#pragma once

#include "AssignPatternEvaluator.h"

SET_OF_RESULTS AssignPatternEvaluator::getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS assignStmts = t_pkb->getAllAssignStmtByVarAndExactPattern(t_g1.getName(), t_g2.getVector());
  if (assignStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(assignStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS AssignPatternEvaluator::getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS assignStmts = t_pkb->getAllAssignStmtByVarAndSubtreePattern(t_g1.getName(), t_g2.getVector());
  if (assignStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(assignStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS AssignPatternEvaluator::getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS assignStmts = t_pkb->getAllAssignStmtListByVar(t_g1.getName());
  if (assignStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(assignStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS AssignPatternEvaluator::getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS assignStmts = t_pkb->getAllAssignStmtByExactPattern(t_g2.getVector());
  if (assignStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(assignStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS AssignPatternEvaluator::getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS assignStmts = t_pkb->getAllAssignStmtBySubtreePattern(t_g2.getVector());
  if (assignStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(assignStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS AssignPatternEvaluator::getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> allAssignStmts = t_pkb->getAllAssignStmts();
  if (allAssignStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(allAssignStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS AssignPatternEvaluator::getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<STMT_NUM, VAR_INDEX> assignStmtsWithVar = t_pkb->getAllAssignStmtWithVarByExactPattern(t_g2.getVector());
  if (assignStmtsWithVar.empty()) {
    return m_result;
  }

  std::unordered_map<STMT_NUM, VAR_NAME> assignStmtsWithVarName;
  for (auto& x : assignStmtsWithVar) {
    VAR_NAME varName = t_pkb->getVarNameFromIdx(x.second);
    assignStmtsWithVarName[x.first] = varName;
  }

  m_result = Formatter::formatMapIntStrToMapStrVectorStr(assignStmtsWithVarName);
  return m_result;
}

SET_OF_RESULTS AssignPatternEvaluator::getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<STMT_NUM, VAR_INDEX> assignStmtsWithVar = t_pkb->getAllAssignStmtWithVarBySubtreePattern(t_g2.getVector());
  if (assignStmtsWithVar.empty()) {
    return m_result;
  }

  std::unordered_map<STMT_NUM, VAR_NAME> assignStmtsWithVarName;
  for (auto& x : assignStmtsWithVar) {
    VAR_NAME varName = t_pkb->getVarNameFromIdx(x.second);
    assignStmtsWithVarName[x.first] = varName;
  }

  m_result = Formatter::formatMapIntStrToMapStrVectorStr(assignStmtsWithVarName);
  return m_result;
}

SET_OF_RESULTS AssignPatternEvaluator::getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, std::string> assignStmtsWithVar = t_pkb->getAllAssignStmtWithVarName();
  if (assignStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = Formatter::formatMapIntStrToMapStrVectorStr(assignStmtsWithVar);
  return m_result;
}