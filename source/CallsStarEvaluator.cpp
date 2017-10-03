#pragma once

#include "CallsStarEvaluator.h"

bool CallsStarEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g2.getName() == "_") {
    if (t_pkb->isCallsAnything(t_g1.getName())) {
      //std::cout << "Calls Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Call Anything!\n";
      return false;
    }
  } else if (t_g1.getName() == "_") {
    if (t_pkb->isCalledByAnything(t_g2.getName())) {
      //std::cout << "Called By Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Called By Anything!\n";
      return false;
    }
  } else {
    if (t_pkb->isCallsStar(t_g1.getName(), t_g2.getName())) {
      //std::cout << "Calls: True\n";
      return true;
    } else {
      //std::cout << "Calls: False\n";
      return false;
    }
  }
}

bool CallsStarEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasCallsRelationship()) {
    //std::cout << "Has Calls Relationship!\n";
    return true;
  } else {
    //std::cout << "No Calls Relationship\n";
    return false;
  }
}

SET_OF_RESULTS CallsStarEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getName() == "_") {
    std::vector<std::string> procedures = t_pkb->getCalledByStarAnything();
    if (procedures.empty()) {
      return m_result;
    }

    m_result[t_g2.getName()] = procedures;
  } else if (t_g1.getType() == queryType::GType::STR) {
    std::vector<std::string> procedures = t_pkb->getCallsStar(t_g1.getName());
    if (procedures.empty()) {
      return m_result;
    }

    m_result[t_g2.getName()] = procedures;
  }

  return m_result;
}

SET_OF_RESULTS CallsStarEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getName() == "_") {
    std::vector<std::string> procedures = t_pkb->getCallsStarAnything();
    if (procedures.empty()) {
      return m_result;
    }

    m_result[t_g1.getName()] = procedures;
  } else if (t_g2.getType() == queryType::GType::STR) {
    std::vector<std::string> procedures = t_pkb->getCalledByStar(t_g2.getName());
    if (procedures.empty()) {
      return m_result;
    }

    m_result[t_g1.getName()] = procedures;
  }

  return m_result;
}

SET_OF_RESULTS CallsStarEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  std::unordered_map<std::string, std::vector<std::string>> allCallsStar = t_pkb->getAllCallsStar();
  if (allCallsStar.empty()) {
    return m_result;
  }

  m_result = allCallsStar;
  return m_result;
}