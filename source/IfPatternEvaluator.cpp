#pragma once

#include "IfPatternEvaluator.h"

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> ifStmts = t_pkb->getIfStmtByVar(t_g1.getName());
  if (ifStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(ifStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> ifStmts = t_pkb->getIfStmtByVar(t_g1.getName());
  if (ifStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(ifStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> ifStmts = t_pkb->getIfStmtByVar(t_g1.getName());
  if (ifStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(ifStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> ifStmts = t_pkb->getAllIfStmts();
  if (ifStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(ifStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> ifStmts = t_pkb->getAllIfStmts();
  if (ifStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(ifStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> allIfStmts = t_pkb->getAllIfStmts();
  if (allIfStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(allIfStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, std::string> ifStmtsWithVar = t_pkb->getAllIfStmtsWithVar();
  if (ifStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = Formatter::formatMapIntStrToMapStrVectorStr(ifStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, std::string> ifStmtsWithVar = t_pkb->getAllIfStmtsWithVar();
  if (ifStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = Formatter::formatMapIntStrToMapStrVectorStr(ifStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS IfPatternEvaluator::getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, std::string> ifStmtsWithVar = t_pkb->getAllIfStmtsWithVar();
  if (ifStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = Formatter::formatMapIntStrToMapStrVectorStr(ifStmtsWithVar);
  return m_result;
}