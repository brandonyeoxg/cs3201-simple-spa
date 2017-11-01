#pragma once

#include "NextStarEvaluator.h"

bool NextStarEvaluator::isRelationTrue(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_g2.getName() == OPERATOR_UNDERSCORE) {
    if (t_pkb->hasNextLine(std::stoi(t_g1.getName()))) {
      //std::cout << "Next* By Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Next* By Anything!\n";
      return false;
    }
  } else if (t_g1.getName() == OPERATOR_UNDERSCORE) {
    if (t_pkb->hasLineBefore(std::stoi(t_g2.getName()))) {
      //std::cout << "Next* to Anything!\n";
      return true;
    } else {
      //std::cout << "Does not Next* to Anything!\n";
      return false;
    }
  } else {
    if (t_pkb->isNextStar(std::stoi(t_g1.getName()), std::stoi(t_g2.getName()))) {
      //std::cout << "Next*: True\n";
      return true;
    } else {
      //std::cout << "Next*: False\n";
      return false;
    }
  }
}

bool NextStarEvaluator::hasRelationship(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  if (t_pkb->hasNextRelationship()) {
    //std::cout << "Has Next Relationship!\n";
    return true;
  } else {
    //std::cout << "No Next Relationship\n";
    return false;
  }
}

SET_OF_RESULTS NextStarEvaluator::evaluateRightSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g1.getType() == queryType::GType::STMT_NO) {
    std::vector<int> stmts = t_pkb->getAllLinesAfter(std::stoi(t_g1.getName()));
    if (stmts.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmts, t_g2);
    m_result[t_g2.getName()] = stmtVector;
  } else if (t_g1.getName() == OPERATOR_UNDERSCORE) {
    std::vector<int> stmtIntVector = t_pkb->getAllLinesAfterAnyLine();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g2);
    m_result[t_g2.getName()] = stmtStrVector;
  }

  return m_result;
}

SET_OF_RESULTS NextStarEvaluator::evaluateLeftSynonym(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  if (t_g2.getType() == queryType::GType::STMT_NO) {
    std::vector<int> stmts = t_pkb->getAllLinesBefore(std::stoi(t_g2.getName()));
    if (stmts.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, stmts, t_g1);
    m_result[t_g1.getName()] = stmtVector;
  } else if (t_g2.getName() == OPERATOR_UNDERSCORE) {
    std::vector<int> stmtIntVector = t_pkb->getAllLinesBeforeAnyLine();
    if (stmtIntVector.empty()) {
      return m_result;
    }

    std::vector<std::string> stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, stmtIntVector, t_g1);
    m_result[t_g1.getName()] = stmtStrVector;
  }

  return m_result;
}

SET_OF_RESULTS NextStarEvaluator::evaluateBothSynonyms(PkbReadOnly *t_pkb, Grammar t_g1, Grammar t_g2) {
  std::unordered_map<int, queryType::GType> typeOfStmts = t_pkb->getTypeOfStatementTable();

  std::unordered_map<int, std::vector<int>> allNextStar = t_pkb->getAllNextStar();
  if (t_g1.getName() == t_g2.getName()) {
    allNextStar = EvaluatorUtil::filterSameResultsForSameSynonyms(allNextStar);
  }

  if (allNextStar.empty()) {
    return m_result;
  }

  for (auto& x : allNextStar) {
    std::vector<std::string> stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_g2);
    m_result[std::to_string(x.first)] = EvaluatorUtil::filterStmts(typeOfStmts, x.first, t_g1, stmtVector);
  }

  return m_result;
}