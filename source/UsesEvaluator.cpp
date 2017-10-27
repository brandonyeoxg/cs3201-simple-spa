#pragma once

#include "UsesEvaluator.h"

bool UsesEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g1.getType() == queryType::GType::STMT_NO && t_g2.getName() == OPERATOR_UNDERSCORE) {
    if (t_pkb->isUsesAnything(std::stoi(t_g1.getName()))) {
      //std::cout << "Is Uses Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Uses Anything!\n";
      return false;
    }
  } else if (t_g1.getType() == queryType::GType::STR && t_g2.getName() == OPERATOR_UNDERSCORE) {
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

SET_OF_RESULTS UsesEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    std::vector<std::string> varUsedByStmt = t_pkb->getUses(std::stoi(t_g1.getName()));
    if (varUsedByStmt.empty()) {
      return m_result;
    }

    m_result[t_g2.getName()] = varUsedByStmt;
  } else if (t_g1.getType() == queryType::GType::STR) {
    std::vector<std::string> varUsedByStmt = t_pkb->getUsesPVarNamesWithProcIdx(t_g1.getName());
    if (varUsedByStmt.empty()) {
      return m_result;
    }

    m_result[t_g2.getName()] = varUsedByStmt;
  }

  return m_result;
}

SET_OF_RESULTS UsesEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getName() != OPERATOR_UNDERSCORE) {
    if (t_g1.getType() == queryType::GType::PROC) {
      std::vector<std::string> stmtVector = t_pkb->getUsesPProcNamesWithVarIdx(t_g2.getName());
      if (stmtVector.empty()) {
        return m_result;
      }

      m_result[t_g1.getName()] = stmtVector;
    } else {
      std::vector<int> stmtIntVector = t_pkb->getStmtUses(t_g2.getName());
      if (stmtIntVector.empty()) {
        return m_result;
      }

      std::vector<std::string> stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
      if (!stmtStrVector.empty()) {
        m_result[t_g1.getName()] = stmtStrVector;
      }
    }
  } else if (t_g2.getName() == OPERATOR_UNDERSCORE) {
    if (t_g1.getType() == queryType::GType::PROC) {
      std::vector<std::string> stmtVector = t_pkb->getUsesPAllProcNames();
      if (stmtVector.empty()) {
        return m_result;
      }

      m_result[t_g1.getName()] = stmtVector;
    } else {
      std::vector<int> stmtIntVector = t_pkb->getStmtUsesAnything();
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

SET_OF_RESULTS UsesEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::PROC) {
    std::multimap<std::string, std::string> procsAndVar = t_pkb->getUsesPAllProcToVar();
    if (procsAndVar.empty()) {
      return m_result;
    }

    for (auto& x : procsAndVar) {
      if (!x.second.empty()) {
        m_result[x.first].push_back(x.second);
      }
    }
  } else {
    std::unordered_map<std::string, std::vector<int>> stmtsAndVar = t_pkb->getAllStmtUses();
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