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

SET_OF_RESULTS_INDICES CallsEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (StringUtil::isUnderscore(t_g1.getName())) {
    LIST_OF_PROC_INDICES procedures = t_pkb->getCalledByAnythingByIdx();
    if (procedures.empty()) {
      return m_result;
    }

    m_result[0] = procedures;
  } else if (t_g1.getType() == queryType::GType::STR) {
    PROC_INDEX procIdx = t_pkb->getProcIdxFromName(t_g1.getName());
    LIST_OF_PROC_INDICES procedures = t_pkb->getCallsByIdx(procIdx);
    if (procedures.empty()) {
      return m_result;
    }

    m_result[0] = procedures;
  }

  return m_result;
}

SET_OF_RESULTS_INDICES CallsEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (StringUtil::isUnderscore(t_g2.getName())) {
    LIST_OF_PROC_INDICES procedures = t_pkb->getCallsAnythingByIdx();
    if (procedures.empty()) {
      return m_result;
    }

    m_result[0] = procedures;
  } else if (t_g2.getType() == queryType::GType::STR) {
    PROC_INDEX procIdx = t_pkb->getProcIdxFromName(t_g2.getName());
    LIST_OF_PROC_INDICES procedures = t_pkb->getCalledByByIdx(procIdx);
    if (procedures.empty()) {
      return m_result;
    }

    m_result[0] = procedures;
  }

  return m_result;
}

SET_OF_RESULTS_INDICES CallsEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES allCalls = t_pkb->getAllCallsByIdx();
  if (allCalls.empty()) {
    return m_result;
  }

  for (auto& x : allCalls) {
    m_result[x.first] = x.second;
  }

  return m_result;
}