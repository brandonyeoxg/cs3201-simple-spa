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

SET_OF_RESULTS ModifiesEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    std::vector<std::string> varModifiedByStmt = t_pkb->getModifies(std::stoi(t_g1.getName()));
    if (varModifiedByStmt.empty()) {
      return m_result;
    }

    m_result[t_g2.getName()] = varModifiedByStmt;
  } else if (t_g1.getType() == queryType::GType::STR) {
    std::vector<std::string> varUsedByStmt = t_pkb->getModifiesPVarNamesWithProcIdx(t_g1.getName());
    if (varUsedByStmt.empty()) {
      return m_result;
    }

    m_result[t_g2.getName()] = varUsedByStmt;
  }

  return m_result;
}

SET_OF_RESULTS ModifiesEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (StringUtil::isUnderscore(t_g2.getName())) {
    if (t_g1.getType() == queryType::GType::PROC) {
      std::vector<std::string> stmtVector = t_pkb->getModifiesPProcNamesWithVarIdx(t_g2.getName());
      if (stmtVector.empty()) {
        return m_result;
      }

      m_result[t_g1.getName()] = stmtVector;
    } else {
      std::vector<int> stmtIntVector = t_pkb->getStmtModifies(t_g2.getName());
      if (stmtIntVector.empty()) {
        return m_result;
      }

      std::vector<std::string> stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
      if (!stmtStrVector.empty()) {
        m_result[t_g1.getName()] = stmtStrVector;
      }
    }
  } else if (StringUtil::isUnderscore(t_g2.getName())) {
    if (t_g1.getType() == queryType::GType::PROC) {
      std::vector<std::string> stmtVector = t_pkb->getModifiesPAllProcNames();
      if (stmtVector.empty()) {
        return m_result;
      }

      m_result[t_g1.getName()] = stmtVector;
    } else {
      std::vector<int> stmtIntVector = t_pkb->getStmtModifiesAnything();
      if (stmtIntVector.empty()) {
        return m_result;
      }

      std::vector<std::string> stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
      if (!stmtStrVector.empty()) {
        m_result[t_g1.getName()] = stmtStrVector;
      }
    }
  }

  return m_result;
}

SET_OF_RESULTS ModifiesEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::PROC) {
    std::multimap<std::string, std::string> procsAndVar = t_pkb->getModifiesPAllProcToVar();
    if (procsAndVar.empty()) {
      return m_result;
    }

    for (auto& x : procsAndVar) {
      if (!x.second.empty()) {
        m_result[x.first].push_back(x.second);
      }
    }
  } else {
    std::unordered_map<std::string, std::vector<int>> stmtsAndVar = t_pkb->getAllStmtModifies();
    if (stmtsAndVar.empty()) {
      return m_result;
    }

    for (auto& x : stmtsAndVar) {
      if (!x.second.empty()) {
        std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_g1);
        if (!stmtVector.empty()) {
          m_result[x.first] = stmtVector;
        }
      }
    }
  }

  return m_result;
}