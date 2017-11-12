#pragma once

#include "UsesEvaluator.h"

bool UsesEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g1.getType() == queryType::GType::STMT_NO && StringUtil::isUnderscore(t_g2.getName())) {
    if (t_pkb->isUsesAnything(std::stoi(t_g1.getName()))) {
      //std::cout << "Is Uses Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Uses Anything!\n";
      return false;
    }
  } else if (t_g1.getType() == queryType::GType::STR && StringUtil::isUnderscore(t_g2.getName())) {
    if (t_pkb->isUsesInProc(t_g1.getName())) {
      //std::cout << "Is Uses Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Uses Anything!\n";
      return false;
    }
  } else if (t_g1.getType() == queryType::GType::STMT_NO && t_g2.getType() == queryType::GType::STR) {
    if (t_pkb->isUses(std::stoi(t_g1.getName()), t_g2.getName())) {
      //std::cout << "Uses: True\n";
      return true;
    } else {
      //std::cout << "Uses: False\n";
      return false;
    }
  } else if (t_g1.getType() == queryType::GType::STR && t_g2.getType() == queryType::GType::STR) {
    if (t_pkb->isUsesP(t_g1.getName(), t_g2.getName())) {
      //std::cout << "Is Uses Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Uses Anything!\n";
      return false;
    }
  }

  return false;
}

bool UsesEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  return false;
}

SET_OF_RESULTS_INDICES UsesEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    LIST_OF_VAR_INDICES varUsedByStmt = t_pkb->getUsesByIdx(std::stoi(t_g1.getName()));
    if (varUsedByStmt.empty()) {
      return m_result;
    }

    m_result[0] = varUsedByStmt;
  } else if (t_g1.getType() == queryType::GType::STR) {
    LIST_OF_VAR_INDICES varUsedByStmt = t_pkb->getUsesPVarIndicesWithProcIdx(t_g1.getName());
    if (varUsedByStmt.empty()) {
      return m_result;
    }

    m_result[0] = varUsedByStmt;
  }

  return m_result;
}

SET_OF_RESULTS_INDICES UsesEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (!StringUtil::isUnderscore(t_g2.getName())) {
    if (t_g1.getType() == queryType::GType::PROC) {
      LIST_OF_PROC_INDICES procIndices = t_pkb->getUsesPProcIndicesWithVarIdx(t_g2.getName());
      if (procIndices.empty()) {
        return m_result;
      }

      m_result[0] = procIndices;
    } else {
      LIST_OF_STMT_NUMS stmtIntVector = t_pkb->getStmtUses(t_g2.getName());
      if (stmtIntVector.empty()) {
        return m_result;
      }

      LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
      if (!stmtVector.empty()) {
        m_result[0] = stmtVector;
      }
    }
  } else if (StringUtil::isUnderscore(t_g2.getName())) {
    if (t_g1.getType() == queryType::GType::PROC) {
      LIST_OF_PROC_INDICES procIndices = t_pkb->getUsesPAllProcIndices();
      if (procIndices.empty()) {
        return m_result;
      }

      m_result[0] = procIndices;
    } else {
      LIST_OF_STMT_NUMS stmtIntVector = t_pkb->getStmtUsesAnything();
      if (stmtIntVector.empty()) {
        return m_result;
      }

      LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
      if (!stmtVector.empty()) {
        m_result[0] = stmtVector;
      }
    }
  }

  return m_result;
}

SET_OF_RESULTS_INDICES UsesEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::PROC) {
    MAP_OF_PROC_INDEX_TO_VAR_INDEX procsAndVar = t_pkb->getUsesPAllProcToVarByIdx();
    if (procsAndVar.empty()) {
      return m_result;
    }

    for (auto& x : procsAndVar) {
      m_result[x.first].push_back(x.second);
    }
  } else {
    MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS stmtsAndVar = t_pkb->getAllStmtUsesByIdx();
    if (stmtsAndVar.empty()) {
      return m_result;
    }

    for (auto& x : stmtsAndVar) {
      if (!x.second.empty()) {
        LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_g1);
        if (!stmtVector.empty()) {
          m_result[x.first] = stmtVector;
        }
      }
    }
  }

  return m_result;
}