#pragma once

#include "NextEvaluator.h"

bool NextEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (StringUtil::isUnderscore(t_g2.getName())) {
    if (t_pkb->hasNextLine(std::stoi(t_g1.getName()))) {
      //std::cout << "Next By Anything!\n";
      return true;
    } else {
      //std::cout << "Is not Next By Anything!\n";
      return false;
    }
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    if (t_pkb->hasLineBefore(std::stoi(t_g2.getName()))) {
      //std::cout << "Next To Anything!\n";
      return true;
    } else {
      //std::cout << "Is not Next To Anything!\n";
      return false;
    }
  } else {
    if (t_pkb->isNext(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      //std::cout << "Next: True\n";
      return true;
    } else {
      //std::cout << "Next: False\n";
      return false;
    }
  }
}

bool NextEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasNextRelationship()) {
    //std::cout << "Has Next Relationship!\n";
    return true;
  } else {
    //std::cout << "No Next Relationship\n";
    return false;
  }
}

SET_OF_RESULTS_INDICES NextEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    LIST_OF_STMT_NUMS stmts = t_pkb->getLinesAfter(std::stoi(t_g1.getName()));
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

SET_OF_RESULTS_INDICES NextEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    LIST_OF_STMT_NUMS stmts = t_pkb->getLinesBefore(std::stoi(t_g2.getName()));
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

SET_OF_RESULTS_INDICES NextEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES allNext = t_pkb->getAllNext();
  if (allNext.empty()) {
    return m_result;
  }

  for (auto& x : allNext) {
    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_g2);
    m_result[x.first] = EvaluatorUtil::filterStmts(typeOfStmts, x.first, t_g1, stmtVector);
  }

  return m_result;
}