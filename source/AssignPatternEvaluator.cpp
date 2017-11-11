#pragma once

#include "AssignPatternEvaluator.h"

SET_OF_RESULTS_INDICES AssignPatternEvaluator::getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS assignStmts = t_pkb->getAllAssignStmtByVarAndExactPattern(t_g1.getName(), t_g2.getVector());
  if (assignStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(assignStmts);
  m_result[0] = assignStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES AssignPatternEvaluator::getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS assignStmts = t_pkb->getAllAssignStmtByVarAndSubtreePattern(t_g1.getName(), t_g2.getVector());
  if (assignStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(assignStmts);
  m_result[0] = assignStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES AssignPatternEvaluator::getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS assignStmts = t_pkb->getAllAssignStmtListByVar(t_g1.getName());
  if (assignStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(assignStmts);
  m_result[0] = assignStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES AssignPatternEvaluator::getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS assignStmts = t_pkb->getAllAssignStmtByExactPattern(t_g2.getVector());
  if (assignStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(assignStmts);
  m_result[0] = assignStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES AssignPatternEvaluator::getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS assignStmts = t_pkb->getAllAssignStmtBySubtreePattern(t_g2.getVector());
  if (assignStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(assignStmts);
  m_result[0] = assignStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES AssignPatternEvaluator::getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS allAssignStmts = t_pkb->getAllAssignStmts();
  if (allAssignStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(allAssignStmts);
  m_result[0] = allAssignStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES AssignPatternEvaluator::getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  MAP_OF_STMT_NUM_TO_VAR_INDEX assignStmtsWithVar = t_pkb->getAllAssignStmtWithVarByExactPattern(t_g2.getVector());
  if (assignStmtsWithVar.empty()) {
    return m_result;
  }

  /*std::unordered_map<STMT_NUM, VAR_NAME> assignStmtsWithVarName;
  for (auto& x : assignStmtsWithVar) {
    VAR_NAME varName = t_pkb->getVarNameFromIdx(x.second);
    assignStmtsWithVarName[x.first] = varName;
  }*/

  m_result = Formatter::formatMapIntIntToMapIntVectorInt(assignStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS_INDICES AssignPatternEvaluator::getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  MAP_OF_STMT_NUM_TO_VAR_INDEX assignStmtsWithVar = t_pkb->getAllAssignStmtWithVarBySubtreePattern(t_g2.getVector());
  if (assignStmtsWithVar.empty()) {
    return m_result;
  }

  /*std::unordered_map<STMT_NUM, VAR_NAME> assignStmtsWithVarName;
  for (auto& x : assignStmtsWithVar) {
    VAR_NAME varName = t_pkb->getVarNameFromIdx(x.second);
    assignStmtsWithVarName[x.first] = varName;
  }*/

  m_result = Formatter::formatMapIntIntToMapIntVectorInt(assignStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS_INDICES AssignPatternEvaluator::getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  MAP_OF_STMT_NUM_TO_VAR_INDEX assignStmtsWithVar = t_pkb->getAllAssignStmtWithVarIndex();
  if (assignStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = Formatter::formatMapIntIntToMapIntVectorInt(assignStmtsWithVar);
  return m_result;
}