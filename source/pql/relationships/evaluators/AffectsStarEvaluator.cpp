#pragma once

#include "AffectsStarEvaluator.h"

bool AffectsStarEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (StringUtil::isUnderscore(t_g2.getName())) {
    if (t_pkb->isAffectsAnythingStar(std::stoi(t_g1.getName()))) {
      return true;
    } else {
      return false;
    }
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
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

SET_OF_RESULTS_INDICES AffectsStarEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    LIST_OF_AFFECTS_STMTS affectsStmts = t_pkb->getAffectedByStar(std::stoi(t_g1.getName()));
    if (affectsStmts.empty()) {
      return m_result;
    }

    m_result[0] = affectsStmts;
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    LIST_OF_AFFECTS_STMTS affectsStmts = t_pkb->getAffectedByAnythingStar();
    if (affectsStmts.empty()) {
      return m_result;
    }

    m_result[0] = affectsStmts;
  }

  return m_result;
}

SET_OF_RESULTS_INDICES AffectsStarEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    LIST_OF_AFFECTS_STMTS affectsStmts = t_pkb->getAffectsStar(std::stoi(t_g2.getName()));
    if (affectsStmts.empty()) {
      return m_result;
    }

    m_result[0] = affectsStmts;
  } else if (StringUtil::isUnderscore(t_g2.getName())) {
    std::vector<int> affectsStmts = t_pkb->getAffectsAnythingStar();
    if (affectsStmts.empty()) {
      return m_result;
    }

    m_result[0] = affectsStmts;
  }

  return m_result;
}

SET_OF_RESULTS_INDICES AffectsStarEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS allAffects = t_pkb->getAllAffectsStar();
  if (t_g1.getName() == t_g2.getName()) {
    allAffects = EvaluatorUtil::filterSameResultsForSameSynonyms(allAffects);
  }

  if (allAffects.empty()) {
    return m_result;
  }

  return allAffects;
}