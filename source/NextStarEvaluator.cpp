#pragma once

#include "NextStarEvaluator.h"

bool NextStarEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g2.getName() == "_") {
    if (t_pkb->isFollowedByAnything(std::stoi(t_g1.getName()))) {
      //std::cout << "Next* By Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Next* By Anything!\n";
      return false;
    }
  } else if (t_g1.getName() == "_") {
    if (t_pkb->isFollowsAnything(std::stoi(t_g2.getName()))) {
      //std::cout << "Next* to Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Next* to Anything!\n";
      return false;
    }
  } else {
    if (t_pkb->isFollows(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      //std::cout << "Next*: True\n";
      return true;
    } else {
      //std::cout << "Next*: False\n";
      return false;
    }
  }
}

bool NextStarEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasFollowRelationship()) {
    //std::cout << "Has Next Relationship!\n";
    return true;
  } else {
    //std::cout << "No Next Relationship\n";
    return false;
  }
}

SET_OF_RESULTS NextStarEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    int stmtNo;
    try {
      stmtNo = t_pkb->getFollows(std::stoi(t_g1.getName()));
      //std::cout << "getNext* - STMT NO: " << stmtNo << "\n";
    } catch (const std::invalid_argument& ia) {
      //std::cout << "Invalid Argument Exception - No Results for getNext*(s1)\n";
      return m_result;
    }

    std::vector<std::string> stmtVector = filterStmts(typeOfStmts, stmtNo, t_g2);
    m_result[t_g2.getName()] = stmtVector;
  } else if (t_g1.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getFollowsAnything();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector;
    for (auto& x : stmtIntVector) {
      stmtStrVector = filterStmts(typeOfStmts, x, t_g2);
    }

    m_result[t_g2.getName()] = stmtStrVector;
  }

  return m_result;
}

SET_OF_RESULTS NextStarEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    int stmtNo;
    try {
      stmtNo = t_pkb->getFollowedBy(std::stoi(t_g2.getName()));
      //std::cout << "getNextBy - STMT NO: " << stmtNo << "\n";
    } catch (const std::invalid_argument& ia) {
      //std::cout << "Invalid Argument Exception - No Results for getNextBy(s2)\n";
      return m_result;
    }

    std::vector<std::string> stmtVector = filterStmts(typeOfStmts, stmtNo, t_g1);
    m_result[t_g1.getName()] = stmtVector;
  } else if (t_g2.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getFollowedByAnything();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector;
    for (auto& x : stmtIntVector) {
      stmtStrVector = filterStmts(typeOfStmts, x, t_g1);
    }

    m_result[t_g1.getName()] = stmtStrVector;
  }

  return m_result;
}

SET_OF_RESULTS NextStarEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  std::unordered_map<int, int> allFollows = t_pkb->getAllFollows();
  if (allFollows.empty()) {
    return m_result;
  }

  for (auto& x : allFollows) {
    std::vector<std::string> stmtVector = filterStmts(typeOfStmts, x.second, t_g2);
    m_result[std::to_string(x.first)] = filterStmts(typeOfStmts, x.first, t_g1, stmtVector);
  }

  return m_result;
}