#pragma once

#include "IfPatternEvaluator.h"

SET_OF_RESULTS_INDICES IfPatternEvaluator::getAllStmtsWithVarAndExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS ifStmts = t_pkb->getIfStmtByVar(t_g1.getName());
  if (ifStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(ifStmts);
  m_result[0] = ifStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES IfPatternEvaluator::getAllStmtsWithVarAndSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS ifStmts = t_pkb->getIfStmtByVar(t_g1.getName());
  if (ifStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(ifStmts);
  m_result[0] = ifStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES IfPatternEvaluator::getAllStmtsWithVarAndAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS ifStmts = t_pkb->getIfStmtByVar(t_g1.getName());
  if (ifStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(ifStmts);
  m_result[0] = ifStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES IfPatternEvaluator::getAllStmtsWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS ifStmts = t_pkb->getAllIfStmts();
  if (ifStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(ifStmts);
  m_result[0] = ifStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES IfPatternEvaluator::getAllStmtsWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS ifStmts = t_pkb->getAllIfStmts();
  if (ifStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(ifStmts);
  m_result[0] = ifStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES IfPatternEvaluator::getAllStmtsWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  LIST_OF_STMT_NUMS allIfStmts = t_pkb->getAllIfStmts();
  if (allIfStmts.empty()) {
    return m_result;
  }

  //std::vector<std::string> stmtVector = Formatter::formatVectorIntToVectorStr(allIfStmts);
  m_result[0] = allIfStmts;
  return m_result;
}

SET_OF_RESULTS_INDICES IfPatternEvaluator::getAllStmtsAndVarWithExactPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  MAP_OF_STMT_NUM_TO_VAR_INDEX ifStmtsWithVar = t_pkb->getAllIfStmtsWithVar();
  if (ifStmtsWithVar.empty()) {
    return m_result;
  }

  /*std::unordered_map<STMT_NUM, VAR_NAME> ifStmtsWithVarName;
  for (auto& x : ifStmtsWithVar) {
    VAR_NAME varName = t_pkb->getVarNameFromIdx(x.second);
    ifStmtsWithVarName[x.first] = varName;
  }*/

  m_result = Formatter::formatMapIntIntToMapIntVectorInt(ifStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS_INDICES IfPatternEvaluator::getAllStmtsAndVarWithSubPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  MAP_OF_STMT_NUM_TO_VAR_INDEX ifStmtsWithVar = t_pkb->getAllIfStmtsWithVar();
  if (ifStmtsWithVar.empty()) {
    return m_result;
  }

  /*std::unordered_map<STMT_NUM, VAR_NAME> ifStmtsWithVarName;
  for (auto& x : ifStmtsWithVar) {
    VAR_NAME varName = t_pkb->getVarNameFromIdx(x.second);
    ifStmtsWithVarName[x.first] = varName;
  }*/

  m_result = Formatter::formatMapIntIntToMapIntVectorInt(ifStmtsWithVar);
  return m_result;
}

SET_OF_RESULTS_INDICES IfPatternEvaluator::getAllStmtsAndVarWithAnyPattern(PkbReadOnly *t_pkb, Grammar t_stmt, Grammar t_g1, Grammar t_g2) {
  MAP_OF_STMT_NUM_TO_VAR_INDEX ifStmtsWithVar = t_pkb->getAllIfStmtsWithVar();
  if (ifStmtsWithVar.empty()) {
    return m_result;
  }

  /*std::unordered_map<STMT_NUM, VAR_NAME> ifStmtsWithVarName;
  for (auto& x : ifStmtsWithVar) {
    VAR_NAME varName = t_pkb->getVarNameFromIdx(x.second);
    ifStmtsWithVarName[x.first] = varName;
  }*/

  m_result = Formatter::formatMapIntIntToMapIntVectorInt(ifStmtsWithVar);
  return m_result;
}