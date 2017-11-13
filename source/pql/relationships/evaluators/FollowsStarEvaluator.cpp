#pragma once

#include "FollowsStarEvaluator.h"

bool FollowsStarEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (StringUtil::isUnderscore(t_g2.getName())) {
    if (t_pkb->isFollowedByAnything(std::stoi(t_g1.getName()))) {
      //std::cout << "Followed By Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Follow By Anything!\n";
      return false;
    }
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    if (t_pkb->isFollowsAnything(std::stoi(t_g2.getName()))) {
      //std::cout << "Follows Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Follow Anything!\n";
      return false;
    }
  } else {
    if (t_pkb->isFollowsStar(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      //std::cout << "Follows*: True\n";
      return true;
    } else {
      //std::cout << "Follows*: False\n";
      return false;
    }
  }
}

bool FollowsStarEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasFollowRelationship()) {
    //std::cout << "Has Follows Relationship!\n";
    return true;
  } else {
    //std::cout << "No Follows Relationship\n";
    return false;
  }
}

SET_OF_RESULTS_INDICES FollowsStarEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    std::vector<int> stmtIntVector = t_pkb->getFollowsStar(std::stoi(t_g1.getName()));
    if (stmtIntVector.empty()) {
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g2);
    if (!stmtVector.empty()) {
      m_result[0] = stmtVector;
    }
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    std::vector<int> stmtIntVector = t_pkb->getFollowsAnything();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g2);
    if (!stmtVector.empty()) {
      m_result[0] = stmtVector;
    }
  }

  return m_result;
}

SET_OF_RESULTS_INDICES FollowsStarEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    LIST_OF_STMT_NUMS stmtIntVector = t_pkb->getFollowedByStar(std::stoi(t_g2.getName()));
    if (stmtIntVector.empty()) {
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
    m_result[0] = stmtVector;
  } else if (StringUtil::isUnderscore(t_g2.getName())) {
    std::vector<int> stmtIntVector = t_pkb->getFollowedByAnything();
    if (stmtIntVector.empty()) {
      return m_result;
    }
    
    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
    if (!stmtVector.empty()) {
      m_result[0] = stmtVector;
    }
  }

  return m_result;
}

SET_OF_RESULTS_INDICES FollowsStarEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  std::unordered_map<int, std::vector<int>> allFollowsStar = t_pkb->getAllFollowsStar();
  if (allFollowsStar.empty()) {
    return m_result;
  }

  for (auto& x : allFollowsStar) {
    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_g2);
    if (!stmtVector.empty()) {
      LIST_OF_STMT_NUMS stmts = EvaluatorUtil::filterStmts(typeOfStmts, x.first, t_g1, stmtVector);
      if (!stmts.empty()) {
        m_result[x.first] = stmts;
      }
    }
  }

  return m_result;
}