#pragma once

#include "AffectsStarEvaluator.h"

bool AffectsStarEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g2.getName() == "_") {
    if (t_pkb->isFollowedByAnything(std::stoi(t_g1.getName()))) {
      //std::cout << "Followed By Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Follow By Anything!\n";
      return false;
    }
  } else if (t_g1.getName() == "_") {
    if (t_pkb->isFollowsAnything(std::stoi(t_g2.getName()))) {
      //std::cout << "Follows Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Follow Anything!\n";
      return false;
    }
  } else {
    if (t_pkb->isFollows(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      //std::cout << "Follows: True\n";
      return true;
    } else {
      //std::cout << "Follows: False\n";
      return false;
    }
  }
}

bool AffectsStarEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasFollowRelationship()) {
    //std::cout << "Has Follows Relationship!\n";
    return true;
  } else {
    //std::cout << "No Follows Relationship\n";
    return false;
  }
}

SET_OF_RESULTS AffectsStarEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    int stmtNo;
    try {
      stmtNo = t_pkb->getFollows(std::stoi(t_g1.getName()));
      //std::cout << "getFollows - STMT NO: " << stmtNo << "\n";
    } catch (const std::invalid_argument& ia) {
      //std::cout << "Invalid Argument Exception - No Results for getFollows(s1)\n";
      return m_result;
    }

    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtNo, t_g2);
    m_result[t_g2.getName()] = stmtVector;
  } else if (t_g1.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getFollowsAnything();
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
    int stmtNo;
    try {
      stmtNo = t_pkb->getFollowedBy(std::stoi(t_g2.getName()));
      //std::cout << "getFollowedBy - STMT NO: " << stmtNo << "\n";
    } catch (const std::invalid_argument& ia) {
      //std::cout << "Invalid Argument Exception - No Results for getFollowedBy(s2)\n";
      return m_result;
    }

    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtNo, t_g1);
    m_result[t_g1.getName()] = stmtVector;
  } else if (t_g2.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getFollowedByAnything();
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

  std::unordered_map<int, int> allFollows = t_pkb->getAllFollows();
  if (allFollows.empty()) {
    return m_result;
  }

  for (auto& x : allFollows) {
    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_g2);
    m_result[std::to_string(x.first)] = EvaluatorUtil::filterStmts(typeOfStmts, x.first, t_g1, stmtVector);
  }

  return m_result;
}