#pragma once

#include "ParentStarEvaluator.h"

bool ParentStarEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
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
    if (t_pkb->isParentStar(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      //std::cout << "Parent*: True\n";
      return true;
    } else {
      //std::cout << "Parent*: False\n";
      return false;
    }
  }
}

bool ParentStarEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasParentStarRelationship()) {
    //std::cout << "Has Parent* Relationship!\n";
    return true;
  } else {
    //std::cout << "No Parent* Relationship\n";
    return false;
  }
}

SET_OF_RESULTS ParentStarEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    std::vector<int> stmtIntVector = t_pkb->getChildrenStarOf(std::stoi(t_g1.getName()));
    if (stmtIntVector.empty()) {
      return result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g2);
    result[t_g2.getName()] = stmtStrVector;
  } else if (t_g1.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getChildrenStarOfAnything();
    if (stmtIntVector.empty()) {
      return result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g2);
    result[t_g2.getName()] = stmtStrVector;
  }

  return result;
}

SET_OF_RESULTS ParentStarEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    std::vector<int> stmtIntVector = t_pkb->getParentStarOf(std::stoi(t_g2.getName()));
    if (stmtIntVector.empty()) {
      return result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g1);
    result[t_g1.getName()] = stmtStrVector;
  } else if (t_g2.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getParentStarOfAnything();
    if (stmtIntVector.empty()) {
      return result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g1);
    result[t_g1.getName()] = stmtStrVector;
  }

  return result;
}

SET_OF_RESULTS ParentStarEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  std::unordered_map<int, std::vector<int>> allParentsStar = t_pkb->getAllParentsStar();
  if (allParentsStar.empty()) {
    return result;
  }

  for (auto& x : allParentsStar) {
    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, x.second, t_g2);
    if (!stmtStrVector.empty()) {
      std::vector<std::string> stmtVector = filterStmts(typeOfStmts, x.first, t_g1, stmtStrVector);
      if (!stmtVector.empty()) {
        result[std::to_string(x.first)] = stmtVector;
      }
    }

  }

  return result;
}