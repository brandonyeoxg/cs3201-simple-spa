#pragma once

#include "NextStarEvaluator.h"

bool NextStarEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (StringUtil::isUnderscore(t_g2.getName())) {
    if (t_pkb->hasNextLine(std::stoi(t_g1.getName()))) {
      //std::cout << "Next* By Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Next* By Anything!\n";
      return false;
    }
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    if (t_pkb->hasLineBefore(std::stoi(t_g2.getName()))) {
      //std::cout << "Next* to Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Next* to Anything!\n";
      return false;
    }
  } else {
    if (t_pkb->isNextStar(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      //std::cout << "Next*: True\n";
      return true;
    } else {
      //std::cout << "Next*: False\n";
      return false;
    }
  }
}

bool NextStarEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasNextRelationship()) {
    //std::cout << "Has Next Relationship!\n";
    return true;
  } else {
    //std::cout << "No Next Relationship\n";
    return false;
  }
}

SET_OF_RESULTS_INDICES NextStarEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    LIST_OF_STMT_NUMS stmts = t_pkb->getAllLinesAfter(std::stoi(t_g1.getName()));
    if (stmts.empty()) {
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmts, t_g2);
    m_result[0] = stmtVector;
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    LIST_OF_STMT_NUMS stmtIntVector = t_pkb->getAllLinesAfterAnyLine();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g2);
    m_result[0] = stmtVector;
  }

  return m_result;
}

SET_OF_RESULTS_INDICES NextStarEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    LIST_OF_STMT_NUMS stmts = t_pkb->getAllLinesBefore(std::stoi(t_g2.getName()));
    if (stmts.empty()) {
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmts, t_g1);
    m_result[0] = stmtVector;
  } else if (StringUtil::isUnderscore(t_g2.getName())) {
    LIST_OF_STMT_NUMS stmtIntVector = t_pkb->getAllLinesBeforeAnyLine();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
    m_result[0] = stmtVector;
  }

  return m_result;
}

SET_OF_RESULTS_INDICES NextStarEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES allNextStar = t_pkb->getAllNextStar();
  if (t_g1.getName() == t_g2.getName()) {
    allNextStar = EvaluatorUtil::filterSameResultsForSameSynonyms(allNextStar);
  }

  if (allNextStar.empty()) {
    return m_result;
  }

  for (auto& x : allNextStar) {
    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_g2);
    m_result[x.first] = EvaluatorUtil::filterStmts(typeOfStmts, x.first, t_g1, stmtVector);
  }

  return m_result;
}