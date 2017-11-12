#pragma once

#include "ModifiesEvaluator.h"

bool ModifiesEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g1.getType() == queryType::GType::STMT_NO && t_g2.getName() == "_") {
    if (t_pkb->isModifiesAnything(std::stoi(t_g1.getName()))) {
      //std::cout << "Is Modifies Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Modifies Anything!\n";
      return false;
    }
  } else if (t_g1.getType() == queryType::GType::STR && StringUtil::isUnderscore(t_g2.getName())) {
    if (t_pkb->isModifiesInProc(t_g1.getName())) {
      //std::cout << "Is Modifies Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Modifies Anything!\n";
      return false;
    }
  } else if (t_g1.getType() == queryType::GType::STMT_NO && t_g2.getType() == queryType::GType::STR) {
    if (t_pkb->isModifies(std::stoi(t_g1.getName()), t_g2.getName())) {
      //std::cout << "Modifies: True\n";
      return true;
    } else {
      //std::cout << "Modifies: False\n";
      return false;
    }
  } else if (t_g1.getType() == queryType::GType::STR && t_g2.getType() == queryType::GType::STR) {
    if (t_pkb->isModifiesP(t_g1.getName(), t_g2.getName())) {
      //std::cout << "Is Modifies Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Modifies Anything!\n";
      return false;
    }
  }

  return false;
}

bool ModifiesEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  return false;
}

SET_OF_RESULTS_INDICES ModifiesEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    LIST_OF_VAR_INDICES varModifiedByStmt = t_pkb->getModifiesByIdx(std::stoi(t_g1.getName()));
    if (varModifiedByStmt.empty()) {
      return m_result;
    }

    m_result[0] = varModifiedByStmt;
  } else if (t_g1.getType() == queryType::GType::STR) {
    LIST_OF_VAR_INDICES varUsedByStmt = t_pkb->getModifiesPVarIndicesWithProcIdx(t_g1.getName());
    if (varUsedByStmt.empty()) {
      return m_result;
    }

    m_result[0] = varUsedByStmt;
  }

  return m_result;
}

SET_OF_RESULTS_INDICES ModifiesEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (!StringUtil::isUnderscore(t_g2.getName())) {
    if (t_g1.getType() == queryType::GType::PROC) {
      LIST_OF_STMT_NUMS stmtVector = t_pkb->getModifiesPProcIndicesWithVarIdx(t_g2.getName());
      if (stmtVector.empty()) {
        return m_result;
      }

      m_result[0] = stmtVector;
    } else {
      std::vector<int> stmtIntVector = t_pkb->getStmtModifies(t_g2.getName());
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
      LIST_OF_STMT_NUMS stmtVector = t_pkb->getModifiesPAllProcIndices();
      if (stmtVector.empty()) {
        return m_result;
      }

      m_result[0] = stmtVector;
    } else {
      std::vector<int> stmtIntVector = t_pkb->getStmtModifiesAnything();
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

SET_OF_RESULTS_INDICES ModifiesEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::PROC) {
    MAP_OF_PROC_INDEX_TO_VAR_INDEX procsAndVar = t_pkb->getModifiesPAllProcToVarByIdx();
    if (procsAndVar.empty()) {
      return m_result;
    }

    for (auto& x : procsAndVar) {
      m_result[x.first].push_back(x.second); 
    }
  } else {
    MAP_OF_VAR_INDEX_TO_LIST_OF_STMT_NUMS stmtsAndVar = t_pkb->getAllStmtModifiesByIdx();
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