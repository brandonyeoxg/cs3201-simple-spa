#pragma once

#include "FollowsEvaluator.h"

bool FollowsEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
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
    if (t_pkb->isFollows(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      //std::cout << "Follows: True\n";
      return true;
    } else {
      //std::cout << "Follows: False\n";
      return false;
    }
  }
}

bool FollowsEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasFollowRelationship()) {
    //std::cout << "Has Follows Relationship!\n";
    return true;
  } else {
    //std::cout << "No Follows Relationship\n";
    return false;
  }
}

SET_OF_RESULTS_INDICES FollowsEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    int stmtNo;
    try {
      stmtNo = t_pkb->getFollows(std::stoi(t_g1.getName()));
      //std::cout << "getFollows - STMT NO: " << stmtNo << "\n";
    } catch (InvalidArgumentException ia) {
      //std::cout << "Invalid Argument Exception - No Results for getFollows(s1)\n";
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtNo, t_g2);
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

SET_OF_RESULTS_INDICES FollowsEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    int stmtNo;
    try {
      stmtNo = t_pkb->getFollowedBy(std::stoi(t_g2.getName()));
      //std::cout << "getFollowedBy - STMT NO: " << stmtNo << "\n";
    } catch (InvalidArgumentException ia) {
      //std::cout << "Invalid Argument Exception - No Results for getFollowedBy(s2)\n";
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtNo, t_g1);
    if (!stmtVector.empty()) {
      m_result[0] = stmtVector;
    }
  } else if (StringUtil::isUnderscore(t_g2.getName())) {
    LIST_OF_STMT_NUMS stmtIntVector = t_pkb->getFollowedByAnything();
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

SET_OF_RESULTS_INDICES FollowsEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  MAP_OF_STMT_NUMS allFollows = t_pkb->getAllFollows();
  if (allFollows.empty()) {
    return m_result;
  }

  for (auto& x : allFollows) {
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