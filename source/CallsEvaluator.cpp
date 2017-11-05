#pragma once

#include "CallsEvaluator.h"

bool CallsEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (StringUtil::isUnderscore(t_g2.getName())) {
    if (t_pkb->isCallsAnything(t_g1.getName())) {
      //std::cout << "Calls Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Call Anything!\n";
      return false;
    }
  } else if (StringUtil::isUnderscore(t_g1.getName())) {
    if (t_pkb->isCalledByAnything(t_g2.getName())) {
      //std::cout << "Called By Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Called By Anything!\n";
      return false;
    }
  } else {
    if (t_pkb->isCalls(t_g1.getName(), t_g2.getName())) {
      //std::cout << "Calls: True\n";
      return true;
    } else {
      //std::cout << "Calls: False\n";
      return false;
    }
  }
}

bool CallsEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasCallsRelationship()) {
    //std::cout << "Has Calls Relationship!\n";
    return true;
  } else {
    //std::cout << "No Calls Relationship\n";
    return false;
  }
}

SET_OF_RESULTS CallsEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (StringUtil::isUnderscore(t_g1.getName())) {
    std::vector<std::string> procedures = t_pkb->getCalledByAnything();
    if (procedures.empty()) {
      return m_result;
    }

    m_result[t_g2.getName()] = procedures;
  } else if (t_g1.getType() == queryType::GType::STR) {
    std::vector<std::string> procedures = t_pkb->getCalls(t_g1.getName());
    if (procedures.empty()) {
      return m_result;
    }

    m_result[t_g2.getName()] = procedures;
  }

  return m_result;
}

SET_OF_RESULTS CallsEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (StringUtil::isUnderscore(t_g2.getName())) {
    std::vector<std::string> procedures = t_pkb->getCallsAnything();
    if (procedures.empty()) {
      return m_result;
    }

    m_result[t_g1.getName()] = procedures;
  } else if (t_g2.getType() == queryType::GType::STR) {
    std::vector<std::string> procedures = t_pkb->getCalledBy(t_g2.getName());
    if (procedures.empty()) {
      return m_result;
    }

    m_result[t_g1.getName()] = procedures;
  }

  return m_result;
}

SET_OF_RESULTS CallsEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  std::unordered_map<std::string, std::string> allCalls = t_pkb->getAllCalls();
  if (allCalls.empty()) {
    return m_result;
  }

  for (auto& x : allCalls) {
    m_result[x.first].push_back(x.second);
  }

  return m_result;
}