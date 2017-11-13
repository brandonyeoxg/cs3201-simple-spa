#pragma once

#include "WhilePatternEvaluator.h"

SET_OF_RESULTS_INDICES WhilePatternEvaluator::getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS whileStmts = t_pkb->getWhileStmtByVar(t_g1.getName());
  if (whileStmts.empty()) {
    return m_result;
  }

  m_result[0] = whileStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES WhilePatternEvaluator::getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS whileStmts = t_pkb->getWhileStmtByVar(t_g1.getName());
  if (whileStmts.empty()) {
    return m_result;
  }

  m_result[0] = whileStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES WhilePatternEvaluator::getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS whileStmts = t_pkb->getWhileStmtByVar(t_g1.getName());
  if (whileStmts.empty()) {
    return m_result;
  }

  m_result[0] = whileStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES WhilePatternEvaluator::getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS whileStmts = t_pkb->getAllWhileStmts();
  if (whileStmts.empty()) {
    return m_result;
  }

  m_result[0] = whileStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES WhilePatternEvaluator::getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS whileStmts = t_pkb->getAllWhileStmts();
  if (whileStmts.empty()) {
    return m_result;
  }

  m_result[0] = whileStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES WhilePatternEvaluator::getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS allWhileStmts = t_pkb->getAllWhileStmts();
  if (allWhileStmts.empty()) {
    return m_result;
  }

  m_result[0] = allWhileStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES WhilePatternEvaluator::getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  MAP_OF_STMT_NUM_TO_VAR_INDEX whileStmtsWithVar = t_pkb->getAllWhileStmtsWithVar();
  if (whileStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = Formatter::formatMapIntIntToMapIntVectorInt(whileStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS_INDICES WhilePatternEvaluator::getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  MAP_OF_STMT_NUM_TO_VAR_INDEX whileStmtsWithVar = t_pkb->getAllWhileStmtsWithVar();
  if (whileStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = Formatter::formatMapIntIntToMapIntVectorInt(whileStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS_INDICES WhilePatternEvaluator::getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  MAP_OF_STMT_NUM_TO_VAR_INDEX whileStmtsWithVar = t_pkb->getAllWhileStmtsWithVar();
  if (whileStmtsWithVar.empty()) {
    return m_result;
  }

  m_result = Formatter::formatMapIntIntToMapIntVectorInt(whileStmtsWithVar);
  return m_result;
}