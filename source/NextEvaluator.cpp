#pragma once

#include "NextEvaluator.h"

bool NextEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g2.getName() == "_") {
    if (t_pkb->isFollowedByAnything(std::stoi(t_g1.getName()))) {
      //std::cout << "Next By Anything!\n";
      return true;
    } else {
      //std::cout << "Is not Next By Anything!\n";
      return false;
    }
  } else if (t_g1.getName() == "_") {
    if (t_pkb->isFollowsAnything(std::stoi(t_g2.getName()))) {
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
  if (t_pkb->hasFollowRelationship()) {
    //std::cout << "Has Next Relationship!\n";
    return true;
  } else {
    //std::cout << "No Next Relationship\n";
    return false;
  }
}

SET_OF_RESULTS NextEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    std::vector<int> stmts = t_pkb->getLinesAfter(std::stoi(t_g1.getName()));
    if (stmts.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmts, t_g2);
    m_result[t_g2.getName()] = stmtVector;
  } else if (t_g1.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getAllLinesAfterAnyLine();
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

SET_OF_RESULTS NextEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    std::vector<int> stmts = t_pkb->getLinesBefore(std::stoi(t_g2.getName()));
    if (stmts.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmts, t_g1);
    m_result[t_g1.getName()] = stmtVector;
  } else if (t_g2.getName() == "_") {
    std::vector<int> stmtIntVector;// = t_pkb->getAllLinesBeforeAnyLine();
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

SET_OF_RESULTS NextEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  std::unordered_map<int, std::vector<int>> allNext = t_pkb->getAllNext();
  if (allNext.empty()) {
    return m_result;
  }

  for (auto& x : allNext) {
    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_g2);
    m_result[std::to_string(x.first)] = EvaluatorUtil::filterStmts(typeOfStmts, x.first, t_g1, stmtVector);
  }

  return m_result;
}