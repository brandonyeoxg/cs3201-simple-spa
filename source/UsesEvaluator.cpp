#pragma once

#include "UsesEvaluator.h"

bool UsesEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g2.getName() == "_") {
    if (t_pkb->isUsesAnything(std::stoi(t_g1.getName()))) {
      //std::cout << "Is Uses Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Uses Anything!\n";
      return false;
    }
  } else {
    if (t_pkb->isUses(std::stoi(t_g1.getName()), t_g2.getName())) {
      //std::cout << "Uses: True\n";
      return true;
    } else {
      //std::cout << "Uses: False\n";
      return false;
    }
  }
}

bool UsesEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  return false;
}

SET_OF_RESULTS UsesEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, Grammar::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == Grammar::GType::STMT_NO) {
    std::vector<std::string> varUsedByStmt = t_pkb->getUses(std::stoi(t_g1.getName()));
    if (varUsedByStmt.empty()) {
      return result;
    }

    result[t_g2.getName()] = varUsedByStmt;
  } else if (t_g1.getType() == Grammar::GType::STR) {
    
  }

  return result;
}

SET_OF_RESULTS UsesEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, Grammar::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getName() != "_") {
    std::vector<int> stmtIntVector = t_pkb->getStmtUses(t_g2.getName());
    if (stmtIntVector.empty()) {
      return result;
    }
    
    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g1);
    if (!stmtStrVector.empty()) {
      result[t_g1.getName()] = stmtStrVector;
    }
  } else if (t_g2.getName() == "_") {
    std::vector<int> stmtIntVector = t_pkb->getStmtUsesAnything();
    if (stmtIntVector.empty()) {
      return result;
    }

    std::vector<std::string> stmtStrVector = filterStmts(typeOfStmts, stmtIntVector, t_g1);
    if (!stmtStrVector.empty()) {
      result[t_g1.getName()] = stmtStrVector;
    }
  }

  return result;
}

SET_OF_RESULTS UsesEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, Grammar::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  std::unordered_map<std::string, std::vector<int>> stmtsAndVar = t_pkb->getAllStmtUses();
  if (stmtsAndVar.empty()) {
    return result;
  }

  for (auto& x : stmtsAndVar) {
    if (!x.second.empty()) {
      std::vector<std::string> stmtVector = filterStmts(typeOfStmts, x.second, t_g1);
      if (!stmtVector.empty()) {
        result[x.first] = stmtVector;
      }
    } 
  }

  return result;
}