#pragma once

#include "FollowsStarEvaluator.h"

bool FollowsStarEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
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
    if (t_pkb->isFollowsStar(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      //std::cout << "Follows: True\n";
      return true;
    } else {
      //std::cout << "Follows: False\n";
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

SET_OF_RESULTS FollowsStarEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, Grammar::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == Grammar::GType::STMT_NO) {
    std::vector<int> stmtIntVector = t_pkb->getFollowsStar(std::stoi(t_g1.getName()));
    if (stmtIntVector.empty()) {
      return result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g2);
    if (!stmtStrVector.empty()) {
      result[t_g2.getName()] = stmtStrVector;
    }
  } else if (t_g1.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getFollowsAnything();
    if (stmtIntVector.empty()) {
      return result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g2);
    if (!stmtStrVector.empty()) {
      result[t_g2.getName()] = stmtStrVector;
    }
  }

  return result;
}

SET_OF_RESULTS FollowsStarEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, Grammar::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == Grammar::GType::STMT_NO) {
    std::vector<int> stmtIntVector = t_pkb->getFollowedByStar(std::stoi(t_g2.getName()));
    if (stmtIntVector.empty()) {
      return result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g1);
    result[t_g1.getName()] = stmtStrVector;
  } else if (t_g2.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getFollowedByAnything();
    if (stmtIntVector.empty()) {
      return result;
    }

    std::vector<std::string> stmtStrVector;
    for (auto& x : stmtIntVector) {
      stmtStrVector = filterStmts(typeOfStmts, x, t_g1);
    }

    result[t_g1.getName()] = stmtStrVector;
  }

  return result;
}

SET_OF_RESULTS FollowsStarEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, Grammar::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  std::unordered_map<int, std::vector<int>> allFollowsStar = t_pkb->getAllFollowsStar();
  if (allFollowsStar.empty()) {
    return result;
  }

  for (auto& x : allFollowsStar) {
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