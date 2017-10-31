#pragma once

#include "WhilePatternEvaluator.h"

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> whileStmts = t_pkb->getWhileStmtByVar(t_g1.getName());
  if (whileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(whileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> whileStmts = t_pkb->getWhileStmtByVar(t_g1.getName());
  if (whileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(whileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> whileStmts = t_pkb->getWhileStmtByVar(t_g1.getName());
  if (whileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(whileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> whileStmts = t_pkb->getAllWhileStmts();
  if (whileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(whileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> whileStmts = t_pkb->getAllWhileStmts();
  if (whileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(whileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::vector<int> allWhileStmts = t_pkb->getAllWhileStmts();
  if (allWhileStmts.empty()) {
    return m_result;
  }

  std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(allWhileStmts);
  m_result[t_stmt.getName()] = stmtVector;
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<STMT_NUM, VAR_INDEX> whileStmtsWithVar = t_pkb->getAllWhileStmtsWithVar();
  if (whileStmtsWithVar.empty()) {
    return m_result;
  }

  std::unordered_map<STMT_NUM, VAR_NAME> whileStmtsWithVarName;
  for (auto& x : whileStmtsWithVar) {
    VAR_NAME varName = t_pkb->getVarNameFromIdx(x.second);
    whileStmtsWithVarName[x.first] = varName;
  }

  m_result = Formatter::formatMapIntStrToMapStrVectorStr(whileStmtsWithVarName);
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<STMT_NUM, VAR_INDEX> whileStmtsWithVar = t_pkb->getAllWhileStmtsWithVar();
  if (whileStmtsWithVar.empty()) {
    return m_result;
  }

  std::unordered_map<STMT_NUM, VAR_NAME> whileStmtsWithVarName;
  for (auto& x : whileStmtsWithVar) {
    VAR_NAME varName = t_pkb->getVarNameFromIdx(x.second);
    whileStmtsWithVarName[x.first] = varName;
  }

  m_result = Formatter::formatMapIntStrToMapStrVectorStr(whileStmtsWithVarName);
  return m_result;
}

SET_OF_RESULTS WhilePatternEvaluator::getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<STMT_NUM, VAR_INDEX> whileStmtsWithVar = t_pkb->getAllWhileStmtsWithVar();
  if (whileStmtsWithVar.empty()) {
    return m_result;
  }

  std::unordered_map<STMT_NUM, VAR_NAME> whileStmtsWithVarName;
  for (auto& x : whileStmtsWithVar) {
    VAR_NAME varName = t_pkb->getVarNameFromIdx(x.second);
    whileStmtsWithVarName[x.first] = varName;
  }

  m_result = Formatter::formatMapIntStrToMapStrVectorStr(whileStmtsWithVarName);
  return m_result;
}