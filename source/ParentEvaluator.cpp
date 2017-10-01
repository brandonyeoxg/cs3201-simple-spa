#pragma once

#include "ParentEvaluator.h"

bool ParentEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g2.getName() == "_") {
    if (t_pkb->isParentOfAnything(std::stoi(t_g1.getName()))) {
      //std::cout << "Is Parent of Anything!\n";
      return true;
    } else {
      //std::cout << "Is not Parent of Anything!\n";
      return false;
    }
  } else if (t_g1.getName() == "_") {
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

SET_OF_RESULTS ParentEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    std::vector<int> stmtIntVector = t_pkb->getChildrenOf(std::stoi(t_g1.getName()));
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g2);
    m_result[t_g2.getName()] = stmtStrVector;
  } else if (t_g1.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getChildrenOfAnything();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g2);
    m_result[t_g2.getName()] = stmtStrVector;
  }

  return m_result;
}

SET_OF_RESULTS ParentEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    int stmtNo;
    try {
      stmtNo = t_pkb->getParentOf(std::stoi(t_g2.getName()));
    } catch (const std::invalid_argument& ia) {
      //std::cout << "Invalid Argument Exception - No Results for getFollowedBy(s2)\n";
      return m_result;
    }

    std::vector<std::string> stmtVector = filterStmts(typeOfStmts, stmtNo, t_g1);
    m_result[t_g1.getName()] = stmtVector;
  } else if (t_g2.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getParentOfAnything();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g1);
    m_result[t_g1.getName()] = stmtStrVector;
  }

  return m_result;
}

SET_OF_RESULTS ParentEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  std::unordered_map<int, std::vector<int>> allParents = t_pkb->getAllParents();
  if (allParents.empty()) {
    return m_result;
  }

  for (auto& x : allParents) {
    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, x.second, t_g2);
    if (!stmtStrVector.empty()) {
      std::vector<std::string> stmtVector = filterStmts(typeOfStmts, x.first, t_g1, stmtStrVector);
      if (!stmtVector.empty()) {
        m_result[std::to_string(x.first)] = stmtVector;
      }
    }
  
  }

  return m_result;
}