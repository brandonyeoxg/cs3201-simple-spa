#pragma once

#include "ParentEvaluator.h"

bool ParentEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (StringUtil::isUnderscore(t_g2.getName())) {
    if (t_pkb->isParentOfAnything(std::stoi(t_g1.getName()))) {
      //std::cout << "Is Parent of Anything!\n";
      return true;
    } else {
      //std::cout << "Is not Parent of Anything!\n";
      return false;
    }
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    if (t_pkb->isChildrenOfAnything(std::stoi(t_g2.getName()))) {
      //std::cout << "Is Children of Anything!\n";
      return true;
    } else {
      //std::cout << "Is not Children of Anything!\n";
      return false;
    }
  } else {
    if (t_pkb->isParent(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      //std::cout << "Parent: True\n";
      return true;
    } else {
      //std::cout << "Parent: False\n";
      return false;
    }
  }
}

bool ParentEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasParentRelationship()) {
    //std::cout << "Has Parent Relationship!\n";
    return true;
  } else {
    //std::cout << "No Parent Relationship\n";
    return false;
  }
}

SET_OF_RESULTS_INDICES ParentEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    LIST_OF_STMT_NUMS stmtIntVector = t_pkb->getChildrenOf(std::stoi(t_g1.getName()));
    if (stmtIntVector.empty()) {
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g2);
    m_result[0] = stmtVector;
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    LIST_OF_STMT_NUMS stmtIntVector = t_pkb->getChildrenOfAnything();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g2);
    m_result[0] = stmtVector;
  }

  return m_result;
}

SET_OF_RESULTS_INDICES ParentEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    int stmtNo;
    try {
      stmtNo = t_pkb->getParentOf(std::stoi(t_g2.getName()));
    } catch (InvalidArgumentException ia) {
      //std::cout << "Invalid Argument Exception - No Results for getFollowedBy(s2)\n";
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtNo, t_g1);
    m_result[0] = stmtVector;
  } else if (StringUtil::isUnderscore(t_g2.getName())) {
    LIST_OF_STMT_NUMS stmtIntVector = t_pkb->getParentOfAnything();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
    m_result[0] = stmtVector;
  }

  return m_result;
}

SET_OF_RESULTS_INDICES ParentEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS allParents = t_pkb->getAllParents();
  if (allParents.empty()) {
    return m_result;
  }

  for (auto& x : allParents) {
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