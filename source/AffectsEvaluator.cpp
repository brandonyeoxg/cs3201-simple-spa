#pragma once

#include "AffectsEvaluator.h"

bool AffectsEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (StringUtil::isUnderscore(t_g2.getName())) {
    if (t_pkb->isAffectsAnything(std::stoi(t_g1.getName()))) {
      return true;
    } else {
      return false;
    }
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    if (t_pkb->isAffectedByAnything(std::stoi(t_g2.getName()))) {
      return true;
    } else {
      return false;
    }
  } else {
    if (t_pkb->isAffects(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      return true;
    } else {
      return false;
    }
  }
}

bool AffectsEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasAffectsRelationship()) {
    return true;
  } else {
    return false;
  }
}

SET_OF_RESULTS AffectsEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    LIST_OF_AFFECTS_STMTS affectsStmts = t_pkb->getAffectedBy(std::stoi(t_g1.getName()));
    if (affectsStmts.empty()) {
      return m_result;
    }

    LIST_OF_RESULTS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, affectsStmts, t_g2);
    if (!stmtVector.empty()) {
      m_result[t_g2.getName()] = stmtVector;
    }
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    LIST_OF_AFFECTS_STMTS stmtIntVector = t_pkb->getAffectedByAnything();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g2);
    if (!stmtStrVector.empty()) {
      m_result[t_g2.getName()] = stmtStrVector;
    }
  }

  return m_result;
}

SET_OF_RESULTS AffectsEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    LIST_OF_AFFECTS_STMTS affectsStmts = t_pkb->getAffects(std::stoi(t_g2.getName()));
    if (affectsStmts.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, affectsStmts, t_g1);
    if (!stmtVector.empty()) {
      m_result[t_g1.getName()] = stmtVector;
    }
  } else if (StringUtil::isUnderscore(t_g2.getName())) {
    std::vector<int> stmtIntVector = t_pkb->getAffectsAnything();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
    if (!stmtStrVector.empty()) {
      m_result[t_g1.getName()] = stmtStrVector;
    }
  }

  return m_result;
}

SET_OF_RESULTS AffectsEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS allAffects = t_pkb->getAllAffects();
  if (t_g1.getName() == t_g2.getName()) {
    allAffects = EvaluatorUtil::filterSameResultsForSameSynonyms(allAffects);
  }

  if (allAffects.empty()) {
    return m_result;
  }

  for (auto& x : allAffects) {
    std::vector<std::string> stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_g2);
    if (!stmtStrVector.empty()) {
      std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.first, t_g1, stmtStrVector);
      if (!stmtVector.empty()) {
        m_result[std::to_string(x.first)] = stmtVector;
      }
    }
  }

  return m_result;
}