#pragma once

#include "WhilePatternEvaluator.h"

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::list<int> whileStmts;// = t_pkb->getWhileStmtByVarPattern(t_g1.getName(), t_g2.getName(), true);
  if (whileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatListIntToVectorStr(whileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::list<int> whileStmts;// = t_pkb->getWhileStmtByVarPattern(t_g1.getName(), t_g2.getName(), false);
  if (whileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatListIntToVectorStr(whileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::list<int> whileStmts;// = t_pkb->getAllWhileStmtListByVar(t_g1.getName());
  if (whileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatListIntToVectorStr(whileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::list<int> whileStmts;// = t_pkb->getAllWhileStmtByExactPattern(t_g2.getName());
  if (whileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatListIntToVectorStr(whileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::list<int> whileStmts;// = t_pkb->getAllWhileStmtBySubtreePattern(t_g2.getName());
  if (whileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatListIntToVectorStr(whileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> allWhileStmts;// = t_pkb->getAllWhileStmtList();
  if (allWhileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = formatVectorIntToVectorStr(allWhileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, std::string> whileStmtsWithVar;// = t_pkb->getAllWhileStmtAndVarByPattern(t_g2.getName(), true);
  if (whileStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = formatMapIntStrToMapStrVectorStr(whileStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, std::string> whileStmtsWithVar;// = t_pkb->getAllWhileStmtAndVarByPattern(t_g2.getName(), false);
  if (whileStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = formatMapIntStrToMapStrVectorStr(whileStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, std::string> whileStmtsWithVar;// = t_pkb->getAllWhileStmtWithVarName();
  if (whileStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = formatMapIntStrToMapStrVectorStr(whileStmtsWithVar);
  return m_result;
}