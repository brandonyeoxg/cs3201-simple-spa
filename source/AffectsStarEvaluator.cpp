#pragma once

#include "AffectsStarEvaluator.h"

bool AffectsStarEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g2.getName() == OPERATOR_UNDERSCORE) {
    if (t_pkb->isAffectsAnythingStar(std::stoi(t_g1.getName()))) {
      return true;
    } else {
      return false;
    }
  } else if (t_g1.getName() == OPERATOR_UNDERSCORE) {
    if (t_pkb->isAffectedByAnythingStar(std::stoi(t_g2.getName()))) {
      return true;
    } else {
      return false;
    }
  } else {
    if (t_pkb->isAffectsStar(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      return true;
    } else {
      return false;
    }
  }
}

bool AffectsStarEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasAffectsRelationshipStar()) {
    return true;
  } else {
    return false;
  }
}

SET_OF_RESULTS AffectsStarEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    LIST_OF_AFFECTS_STMTS affectsStmts = t_pkb->getAffectedByStar(std::stoi(t_g1.getName()));
    if (affectsStmts.empty()) {
      return m_result;
    }

    LIST_OF_RESULTS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, affectsStmts, t_g2);
    m_result[t_g2.getName()] = stmtVector;
  } else if (t_g1.getName() == OPERATOR_UNDERSCORE) {
    LIST_OF_AFFECTS_STMTS stmtIntVector = t_pkb->getAffectedByAnythingStar();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector;
    for (auto& x : stmtIntVector) {
      stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, x, t_g2);
    }

    m_result[t_g2.getName()] = stmtStrVector;
  }

  return m_result;
}

SET_OF_RESULTS AffectsStarEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    LIST_OF_AFFECTS_STMTS affectsStmts = t_pkb->getAffectsStar(std::stoi(t_g2.getName()));
    if (affectsStmts.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, affectsStmts, t_g1);
    m_result[t_g1.getName()] = stmtVector;
  } else if (t_g2.getName() == OPERATOR_UNDERSCORE) {
    std::vector<int> stmtIntVector = t_pkb->getAffectsAnythingStar();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector;
    for (auto& x : stmtIntVector) {
      stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, x, t_g1);
    }

    m_result[t_g1.getName()] = stmtStrVector;
  }

  return m_result;
}

SET_OF_RESULTS AffectsStarEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS allAffects = t_pkb->getAllAffectsStar();
  if (t_g1.getName() == t_g2.getName()) {
    allAffects = EvaluatorUtil::filterSameResultsForSameSynonyms(allAffects);
  }
  
  if (allAffects.empty()) {
    return m_result;
  }

  for (auto& x : allAffects) {
    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_g2);
    m_result[std::to_string(x.first)] = EvaluatorUtil::filterStmts(typeOfStmts, x.first, t_g1, stmtVector);
  }

  return m_result;
}