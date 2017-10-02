#pragma once

#include "IfPatternEvaluator.h"

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::list<int> ifStmts;// = t_pkb->getIfStmtByVarPattern(t_g1.getName(), t_g2.getName(), true);
  if (ifStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatListIntToVectorStr(ifStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::list<int> ifStmts;// = t_pkb->getIfStmtByVarPattern(t_g1.getName(), t_g2.getName(), false);
  if (ifStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatListIntToVectorStr(ifStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::list<int> ifStmts;// = t_pkb->getAllIfStmtListByVar(t_g1.getName());
  if (ifStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatListIntToVectorStr(ifStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::list<int> ifStmts;// = t_pkb->getAllIfStmtByExactPattern(t_g2.getName());
  if (ifStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatListIntToVectorStr(ifStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::list<int> ifStmts;// = t_pkb->getAllIfStmtBySubtreePattern(t_g2.getName());
  if (ifStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatListIntToVectorStr(ifStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> allIfStmts;// = t_pkb->getAllIfStmtList();
  if (allIfStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatVectorIntToVectorStr(allIfStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, std::string> ifStmtsWithVar;// = t_pkb->getAllIfStmtAndVarByPattern(t_g2.getName(), true);
  if (ifStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = formatMapIntStrToMapStrVectorStr(ifStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, std::string> ifStmtsWithVar;// = t_pkb->getAllIfStmtAndVarByPattern(t_g2.getName(), false);
  if (ifStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = formatMapIntStrToMapStrVectorStr(ifStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, std::string> ifStmtsWithVar;// = t_pkb->getAllIfStmtWithVarName();
  if (ifStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = formatMapIntStrToMapStrVectorStr(ifStmtsWithVar);
  return m_result;
}