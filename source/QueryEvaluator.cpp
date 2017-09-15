#pragma once

#include "QueryEvaluator.h"

void QueryEvaluator::printDivider() {
  std::cout << "\n*************************************\n";
}

void QueryEvaluator::printSelectQueue() {
  std::cout << "\nSelect Queue\n";
  for (int i = 0; i < m_selects.size(); ++i) {
    std::cout << i + 1 << ": " << m_selects.front().getName() << "\n";
    m_selects.push(m_selects.front());
    m_selects.pop();
  }
}

void QueryEvaluator::printRelationQueue() {
  std::cout << "\nRelation Queue\n";
  for (int i = 0; i < m_relations.size(); ++i) {
    std::cout << i + 1 << ": " << m_relations.front().getType() << " ";
    std::cout << m_relations.front().getG1().getName() << " ";
    std::cout << m_relations.front().getG2().getName() << "\n";
    m_relations.push(m_relations.front());
    m_relations.pop();
  }
}

void QueryEvaluator::printPatternQueue() {
  std::cout << "\nPattern Queue\n";
  for (int i = 0; i < m_patterns.size(); ++i) {
    std::cout << i + 1 << ": " << m_patterns.front().getStmt().getName() << " ";
    std::cout << m_patterns.front().getLeft().getName() << " ";
    std::cout << m_patterns.front().getRight().getName() << " ";
    std::cout << m_patterns.front().isSubtree() << "\n";
    m_patterns.push(m_patterns.front());
    m_patterns.pop();
  }
}

void QueryEvaluator::printSelectResultQueue() {
  std::cout << "\nSelect Result Queue";
  for (int i = 0; i < m_selectResults.size(); ++i) {
    std::cout << "\n" << i + 1 << ". " << m_selectedSynonym << ": ";
    for (std::vector<std::string>::iterator getStmts = m_selectResults.front().begin(); getStmts != m_selectResults.front().end(); ++getStmts) {
      std::cout << *getStmts << ", ";
    }

    m_selectResults.push(m_selectResults.front());
    m_selectResults.pop();
  }
}

void QueryEvaluator::printRelationResultQueue() {
  std::cout << "\nRelation Result Queue";
  for (int i = 0; i < m_relationResults.size(); ++i) {
    std::cout << "\nQueue " << i + 1 << ". ";
    int j = 1;
    for (auto& x: m_relationResults.front()) {
      std::cout << "\n" << j << ". " << x.first << ": ";
      for (std::vector<std::string>::iterator getStmts = x.second.begin(); getStmts != x.second.end(); ++getStmts) {
        std::cout << *getStmts << ", ";
      }
      j++;
    }

    m_relationResults.push(m_relationResults.front());
    m_relationResults.pop();
  }
}

void QueryEvaluator::printPatternResultQueue() {
  std::cout << "\nPattern Result Queue\n";
  for (int i = 0; i < m_patternResults.size(); ++i) {
    std::cout << "\nQueue " << i + 1 << ". ";
    int j = 1;
    for (auto& x : m_patternResults.front()) {
      std::cout << "\n" << j << ". " << x.first << ": ";
      for (std::vector<std::string>::iterator getStmts = x.second.begin(); getStmts != x.second.end(); ++getStmts) {
        std::cout << *getStmts << ", ";
      }
      j++;
    }

    m_patterns.push(m_patterns.front());
    m_patterns.pop();
  }
}

/**
* A function that evaluates the query that has been pre-processed by the QueryPreprocessor.
*/
std::vector<std::string> QueryEvaluator::evaluateQuery() {
  printDivider();
  std::cout << "Evaluating Query...\n";

  // Debug: Check the queues
  printSelectQueue();
  printRelationQueue();
  printPatternQueue();

  bool hasResult = getResultFromPkb();
  if (hasResult) {
    return evaluateFinalResult();
  } else if (!hasResult) {
    std::vector<std::string> result;
    printDivider();
    std::cout << "No Query Result: \n";
    for (std::vector<std::string>::iterator getStmts = result.begin(); getStmts != result.end(); ++getStmts) {
      std::cout << *getStmts << ", ";
    }
    printDivider();
    return result;
  }
}

vector<string> QueryEvaluator::formatVectorIntToVectorString(vector<int> t_vectorInt) {
  std::vector<std::string> vectorStr;
  for (std::vector<int>::iterator getStmts = t_vectorInt.begin(); getStmts != t_vectorInt.end(); ++getStmts) {
    vectorStr.push_back(std::to_string(*getStmts));
  }

  return vectorStr;
}

vector<string> QueryEvaluator::getCommonResults(vector<string> t_v1, vector<string> t_v2) {
  vector<string> v;

  sort(t_v1.begin(), t_v1.end());
  sort(t_v2.begin(), t_v2.end());

  set_intersection(t_v1.begin(), t_v1.end(), t_v2.begin(), t_v2.end(), back_inserter(v));

  return v;
}

bool QueryEvaluator::getSelectResultFromPkb(Grammar t_select) {
  if (t_select.getType() != Grammar::GType::VAR && t_select.getType() != Grammar::GType::CONST) {
    // Call the PKB API getStatementTypeTable().
    std::unordered_map<Grammar::GType, std::vector<int>> allStmts = m_pkb->getStatementTypeTable();

    // Check if there are results else return false.
    if (allStmts.empty()) {
      return false;
    }

    // Get all the statements of the type of statements that has been selected.
    std::vector<int> allSelectedStmtsInInt;
    if (t_select.getType() == Grammar::GType::STMT || t_select.getType() == Grammar::GType::PROG_LINE) {
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[Grammar::GType::ASGN].begin(), allStmts[Grammar::GType::ASGN].end());
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[Grammar::GType::WHILE].begin(), allStmts[Grammar::GType::WHILE].end());
    } else if (t_select.getType() == Grammar::GType::ASGN) {
      allSelectedStmtsInInt = allStmts[Grammar::GType::ASGN];
    } else if (t_select.getType() == Grammar::GType::WHILE) {
      allSelectedStmtsInInt = allStmts[Grammar::GType::WHILE];
    }

    // Change from vector<int> to vector<string>.
    std::vector<std::string> allSelectedStmts = formatVectorIntToVectorString(allSelectedStmtsInInt);

    // Push into the selectResults queue.
    storeSelectResultFromPkb(allSelectedStmts);
  } else if (t_select.getType() == Grammar::GType::VAR) {
    std::vector<std::string> allVariables = m_pkb->getAllVariables();
    if (allVariables.empty()) {
      return false;
    }
    storeSelectResultFromPkb(allVariables);
  } else if (t_select.getType() == Grammar::GType::CONST) {
    std::list<std::string> constantsList = m_pkb->getAllConstants();
    vector<string> allConstants;
    for (auto& x : constantsList) {
      allConstants.push_back(x);
    }

    if (allConstants.empty()) {
      return false;
    }
    storeSelectResultFromPkb(allConstants);
  }

  return true;
}

bool QueryEvaluator::getRelationResultFromPkb(Relation t_relation) {
  std::unordered_map<std::string, std::vector<std::string>> result;
  unordered_map<int, Grammar::GType> typeOfStmts = m_pkb->getTypeOfStatementTable();

  // Call the PKB API to get the results of the relation clauses
  if (t_relation.getType() == Relation::RType::FOLLOWS) {
    if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      if (m_pkb->isFollows(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        std::cout << "Follows: True\n";
        return true;
      } else if (!m_pkb->isFollows(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        std::cout << "Follows: False\n";
        return false;
      } else {
        std::cout << "Exception in QueryEvaluator.cpp for isFollows(s1, s2)\n";
        return false;
      }
    } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      int stmtNo;
      try {
        stmtNo = m_pkb->getFollows(std::stoi(t_relation.getG1().getName()));
        std::cout << "getFollows - STMT NO: " << stmtNo << "\n";
      } catch (const std::invalid_argument& ia) {
        std::cout << "Invalid Argument Exception - No Results for getFollows(s1)\n";
        return false;
      }

      std::vector<std::string> stmtVector;
      stmtVector.push_back(std::to_string(stmtNo));
      result[t_relation.getG2().getName()] = stmtVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      int stmtNo;
      try {
        stmtNo = m_pkb->getFollowedBy(std::stoi(t_relation.getG2().getName()));
        std::cout << "getFollowedBy - STMT NO: " << stmtNo << "\n";
      } catch (const std::invalid_argument& ia) {
        std::cout << "Invalid Argument Exception - No Results for getFollowedBy(s2)\n";
        return false;
      }

      std::vector<std::string> stmtVector;
      stmtVector.push_back(std::to_string(stmtNo));
      result[t_relation.getG1().getName()] = stmtVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      std::cout << "FOLLOWS (s1, s2) WORKS!\n";
      std::unordered_map<int, int> allFollows = m_pkb->getAllFollows();
      if (allFollows.empty()) {
        return false;
      }

      for (auto& x : allFollows) {
        std::vector<std::string> stmtVector;
        stmtVector.push_back(std::to_string(x.second));
        result[std::to_string(x.first)] = stmtVector;
      }
    }
  } else if (t_relation.getType() == Relation::RType::FOLLOWS_) {
    std::cout << "FOLLOWS STAR\n";
    if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      if (m_pkb->isFollowsStar(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        std::cout << "Follows*: True\n";
        return true;
      } else if (!m_pkb->isFollowsStar(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        std::cout << "Follows*: False\n";
        return false;
      } else {
        std::cout << "Exception in QueryEvaluator.cpp Line 93\n";
        return false;
      }
    } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      std::vector<int> stmtIntVector = m_pkb->getFollowsStar(std::stoi(t_relation.getG1().getName()));
      if (stmtIntVector.empty()) {
        return false;
      }
      std::vector<std::string> stmtStrVector;
      for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
        stmtStrVector.push_back(std::to_string(*getStmts));
        std::cout << "Follows STMTS: " << *getStmts << " ";
      }
      std::cout << "\n";
      result[t_relation.getG2().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      std::vector<int> stmtIntVector = m_pkb->getFollowedByStar(std::stoi(t_relation.getG2().getName()));
      if (stmtIntVector.empty()) {
        return false;
      }
      std::vector<std::string> stmtStrVector;
      for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
        stmtStrVector.push_back(std::to_string(*getStmts));
        std::cout << "Followed By STMTS: " << *getStmts << " ";
      }
      std::cout << "\n";
      result[t_relation.getG1().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      std::cout << "FOLLOWS* (s1, s2) WORKS!\n";
      std::unordered_map<int, std::vector<int>> allFollows = m_pkb->getAllFollowsStar();
      if (allFollows.empty()) {
        return false;
      }

      for (auto& x : allFollows) {
        std::vector<std::string> stmtVector = formatVectorIntToVectorString(x.second);
        /*for (auto& y : x.second) {
          stmtVector.push_back(std::to_string(y));
        }*/

        result[std::to_string(x.first)] = stmtVector;
      }
    }
  } else if (t_relation.getType() == Relation::RType::PARENT) {
    if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      if (m_pkb->isParent(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        std::cout << "Parent: True\n";
        return true;
      } else if (!m_pkb->isParent(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        std::cout << "Parent: False\n";
        return false;
      } else {
        std::cout << "Exception in QueryEvaluator.cpp Line 93\n";
        return false;
      }
    } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      std::vector<int> stmtIntVector = m_pkb->getChildrenOf(std::stoi(t_relation.getG1().getName()));
      if (stmtIntVector.empty()) {
        return false;
      }
      std::vector<std::string> stmtStrVector;
      for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
        stmtStrVector.push_back(std::to_string(*getStmts));
      }
      result[t_relation.getG2().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      int stmtNo;
      try {
        stmtNo = m_pkb->getParentOf(std::stoi(t_relation.getG2().getName()));
      } catch (const std::invalid_argument& ia) {
        std::cout << "Invalid Argument Exception - No Results for getParentOf(s2)\n";
        return false;
      }
      std::vector<std::string> stmtVector;
      stmtVector.push_back(std::to_string(stmtNo));
      result[t_relation.getG1().getName()] = stmtVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      std::cout << "PARENT (s1, s2) WORKS!\n";
      std::unordered_map<int, std::vector<int>> allParents = m_pkb->getAllParents();
      if (allParents.empty()) {
        return false;
      }

      for (auto& x : allParents) {
        std::vector<std::string> stmtVector;
        for (auto& y : x.second) {
          stmtVector.push_back(std::to_string(y));
        }

        result[std::to_string(x.first)] = stmtVector;
      }
    }
  } else if (t_relation.getType() == Relation::RType::PARENT_) {
    if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      if (m_pkb->isParentStar(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        std::cout << "Parent*: True\n";
        return true;
      } else if (!m_pkb->isParentStar(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        std::cout << "Parent*: False\n";
        return false;
      } else {
        std::cout << "Exception in QueryEvaluator.cpp Line 93\n";
        return false;
      }
    } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      std::vector<int> stmtIntVector = m_pkb->getChildrenStarOf(std::stoi(t_relation.getG1().getName()));
      if (stmtIntVector.empty()) {
        return false;
      }
      std::vector<std::string> stmtStrVector;
      for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
        stmtStrVector.push_back(std::to_string(*getStmts));
      }
      result[t_relation.getG2().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      std::vector<int> stmtIntVector = m_pkb->getParentStarOf(std::stoi(t_relation.getG2().getName()));
      if (stmtIntVector.empty()) {
        return false;
      }
      std::vector<std::string> stmtStrVector;
      for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
        stmtStrVector.push_back(std::to_string(*getStmts));
      }
      result[t_relation.getG1().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      std::cout << "PARENT* (s1, s2) WORKS!\n";
      std::unordered_map<int, std::vector<int>> allParents = m_pkb->getAllParentsStar();
      if (allParents.empty()) {
        return false;
      }

      for (auto& x : allParents) {
        std::vector<std::string> stmtVector;
        for (auto& y : x.second) {
          stmtVector.push_back(std::to_string(y));
        }

        result[std::to_string(x.first)] = stmtVector;
      }
    }
  } else if (t_relation.getType() == Relation::RType::USES) {
    if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::STR) {
      if (m_pkb->isUses(std::stoi(t_relation.getG1().getName()), t_relation.getG2().getName())) {
        std::cout << "Uses: True\n";
        return true;
      } else if (!m_pkb->isUses(std::stoi(t_relation.getG1().getName()), t_relation.getG2().getName())) {
        std::cout << "Uses: False\n";
        return false;
      } else {
        std::cout << "Exception in QueryEvaluator.cpp for isUses(s, v)\n";
        return false;
      }
    } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::VAR) {
      std::cout << "USES (STMT_NO, VAR) WORKS!\n";
      std::vector<std::string> varUsedByStmt = m_pkb->getUses((std::stoi(t_relation.getG1().getName())));
      if (varUsedByStmt.empty()) {
        return false;
      }

      result[t_relation.getG2().getName()] = varUsedByStmt;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::STR) {
      std::cout << "USES (STMT, STR) WORKS!\n";
      std::vector<int> stmtIntVector = m_pkb->getStmtUses(t_relation.getG2().getName());
      if (stmtIntVector.empty()) {
        return false;
      }

      std::vector<std::string> stmtStrVector;
      for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
        stmtStrVector.push_back(std::to_string(*getStmts));
      }

      result[t_relation.getG1().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::VAR) {
      std::cout << "USES (STMT, VAR) WORKS!\n";
      std::unordered_map<std::string, std::vector<int>> stmtsUsedVar = m_pkb->getAllStmtUses();
      for (auto& x : stmtsUsedVar) {
        std::vector<std::string> stmtStrVector;
        for (auto& y : x.second) {
          stmtStrVector.push_back(std::to_string(y));
        }
        result[x.first] = stmtStrVector;
      }
    }
  } else if (t_relation.getType() == Relation::RType::MODIFIES) {
    if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::STR) {
      if (m_pkb->isModifies(std::stoi(t_relation.getG1().getName()), t_relation.getG2().getName())) {
        std::cout << "Modifies: True\n";
        return true;
      } else if (!m_pkb->isModifies(std::stoi(t_relation.getG1().getName()), t_relation.getG2().getName())) {
        std::cout << "Modifies: False\n";
        return false;
      } else {
        std::cout << "Exception in QueryEvaluator.cpp for Modifies(s, v)\n";
        return false;
      }
    } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::VAR) {
      std::cout << "Modifies (STMT_NO, VAR) WORKS!\n";
      std::vector<std::string> varModifiedByStmt = m_pkb->getModifies((std::stoi(t_relation.getG1().getName())));
      if (varModifiedByStmt.empty()) {
        return false;
      }

      result[t_relation.getG2().getName()] = varModifiedByStmt;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::STR) {
      std::cout << "Modifies (STMT, STR) WORKS!\n";
      std::vector<int> stmtIntVector = m_pkb->getStmtModifies(t_relation.getG2().getName());
      if (stmtIntVector.empty()) {
        return false;
      }

      std::vector<std::string> stmtStrVector;
      for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
        stmtStrVector.push_back(std::to_string(*getStmts));
      }

      result[t_relation.getG1().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::VAR) {
      std::cout << "Modifies (STMT, VAR) WORKS!\n";
      std::unordered_map<std::string, std::vector<int>> stmtsUsedVar = m_pkb->getAllStmtModifies();
      for (auto& x : stmtsUsedVar) {
        std::vector<std::string> stmtStrVector;
        for (auto& y : x.second) {
          stmtStrVector.push_back(std::to_string(y));
        }
        result[x.first] = stmtStrVector;
      }
    }
  } else {
    std::cout << "Relation Type: " + t_relation.getType() << "\n";
  }

  if (result.empty()) {
    return false;
  }

  // Store the result
  std::unordered_map<std::string, int>::const_iterator got;
  if (t_relation.getG1().getType() != Grammar::GType::STMT_NO && (t_relation.getG2().getType() == Grammar::GType::STMT_NO || t_relation.getG2().getType() == Grammar::GType::STR)) {
    got = m_synonymsUsedInQuery.find(t_relation.getG1().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::RELATION);
        m_relations.push(t_relation);
      }
    }
  } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() != Grammar::GType::STMT_NO && t_relation.getG2().getType() != Grammar::GType::STR) {
    got = m_synonymsUsedInQuery.find(t_relation.getG2().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::RELATION);
        m_relations.push(t_relation);
      }
    }
  } else if (t_relation.getG1().getType() != Grammar::GType::STMT_NO && t_relation.getG2().getType() != Grammar::GType::STMT_NO && t_relation.getG2().getType() != Grammar::GType::STR) {
    got = m_synonymsUsedInQuery.find(t_relation.getG1().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::RELATION);
        m_relations.push(t_relation);
      } else {
        got = m_synonymsUsedInQuery.find(t_relation.getG2().getName());
        if (got != m_synonymsUsedInQuery.end()) {
          if (got->second > 1) {
            storeResultFromPkb(result, queryType::RELATION);
            m_relations.push(t_relation);
          }
        }
      }
    }
  }

  return true;
}

bool QueryEvaluator::getPatternResultFromPkb(Pattern t_pattern) {
  std::unordered_map<std::string, std::vector<std::string>> result;

  // Todo: Get results for pattern clauses by calling the API from PKB
  if (t_pattern.getStmt().getType() == Grammar::GType::ASGN) {
    if (t_pattern.getLeft().getName() != "_" && t_pattern.getRight().getName() != "_") {
      if (t_pattern.getLeft().getType() == Grammar::GType::VAR) {
        unordered_map<int, string> assignStmtsWithVar = m_pkb->getAllAssignStmtAndVarByPattern(t_pattern.getRight().getName(), !t_pattern.isSubtree());
        if (assignStmtsWithVar.empty()) {
          return false;
        }

        for (auto& x : assignStmtsWithVar) {
          vector<string> varVector;
          varVector.push_back(x.second);
          result[to_string(x.first)] = varVector;
        }
      } else {
        list<int> assignStmts = m_pkb->getAssignStmtByVarPattern(t_pattern.getLeft().getName(), t_pattern.getRight().getName(), !t_pattern.isSubtree());
        if (assignStmts.empty()) {
          return false;
        }

        vector<string> stmtVector;
        for (auto& x : assignStmts) {
          stmtVector.push_back(to_string(x));  
        }
        result["a"] = stmtVector;
      }
    } else if (t_pattern.getLeft().getName() == "_" && t_pattern.getRight().getName() == "_") {
      list<int> allAssignStmts = m_pkb->getAllAssignStmtList();
      if (allAssignStmts.empty()) {
        return false;
      }

      vector<string> stmtVector;
      for (auto& x : allAssignStmts) {
        stmtVector.push_back(to_string(x));
      }
      result["a"] = stmtVector;
    } else if (t_pattern.getRight().getName() == "_") {
      if (t_pattern.getLeft().getType() == Grammar::GType::VAR) {
        unordered_map<int, string> assignStmtsWithVar = m_pkb->getAllAssignStmtWithVarName();
        if (assignStmtsWithVar.empty()) {
          return false;
        }

        for (auto& x : assignStmtsWithVar) {
          vector<string> varVector;
          varVector.push_back(x.second);
          result[to_string(x.first)] = varVector;
        }      
      } else {
        list<int> assignStmts = m_pkb->getAllAssignStmtListByVar(t_pattern.getLeft().getName());
        if (assignStmts.empty()) {
          return false;
        }

        vector<string> stmtVector;
        for (auto& x : assignStmts) {
          stmtVector.push_back(to_string(x));
        }
        result["a"] = stmtVector;
      }
    } else if (t_pattern.getLeft().getName() == "_") {
      list<int> assignStmts;
      if (!t_pattern.isSubtree()) {
        assignStmts = m_pkb->getAllAssignStmtByExactPattern(t_pattern.getRight().getName());
      } else {
        assignStmts = m_pkb->getAllAssignStmtBySubtreePattern(t_pattern.getRight().getName());
      }
      
      if (assignStmts.empty()) {
        return false;
      }

      vector<string> stmtVector;
      for (auto& x : assignStmts) {
        cout << "X: x\n";
        stmtVector.push_back(to_string(x));
      }
      result["a"] = stmtVector;
    }
  }

  // Store the result
  std::unordered_map<std::string, int>::const_iterator got;
  if (t_pattern.getLeft().getType() != Grammar::GType::VAR) {
    got = m_synonymsUsedInQuery.find(t_pattern.getStmt().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::PATTERN);
        m_patterns.push(t_pattern);
      }
    }
  } else if (t_pattern.getLeft().getType() == Grammar::GType::VAR) {
    got = m_synonymsUsedInQuery.find(t_pattern.getStmt().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::PATTERN);
        m_patterns.push(t_pattern);
      } else if (got->second == 1) {
        got = m_synonymsUsedInQuery.find(t_pattern.getLeft().getName());
        if (got != m_synonymsUsedInQuery.end()) {
          if (got->second > 1) {
            storeResultFromPkb(result, queryType::PATTERN);
            m_patterns.push(t_pattern);
          }
        }
      }
    }
  }

  return true;
}

/**
* A function that gets the result of the clauses by calling the API from PKB.
* @return true if there are results otherwise false
*/
bool QueryEvaluator::getResultFromPkb() {
  printDivider();
  std::cout << "Getting results from PKB...\n";
  int selectSize = m_selects.size();
  int relationSize = m_relations.size();
  int patternSize = m_patterns.size();

  std::cout << "\nSelect Queue";
  for (int i = 0; i < selectSize; ++i) {
    Grammar grammar = m_selects.front();
    m_selectedSynonym = grammar.getName();
    m_selectedType = grammar.getType();
    std::cout << "\n" << i + 1 << ": " << grammar.getName() << "\n";
    
    // Get Select Results from PKB.
    bool hasResult = getSelectResultFromPkb(grammar);

    // Check if there are results else return false.
    if (!hasResult) {
      return false;
    }

    // Print Select Results Queue.
    printSelectResultQueue();

    // Move the item to the back of the queue.
    m_selects.pop();
    m_selects.push(grammar);
  }

  std::cout << "\nRelation Queue";
  for (int i = 0; i < relationSize; ++i) {
    Relation relation = m_relations.front();
    std::cout << "\n" << i + 1 << ": " << relation.getType() << " ";
    std::cout << relation.getG1().getName() << " ";
    std::cout << relation.getG2().getName() << "\n";

    bool hasResult = getRelationResultFromPkb(relation);
    if (!hasResult) {
      return false;
    }

    printRelationResultQueue();
    m_relations.pop();
  }

  std::cout << "\nPattern Queue";
  for (int i = 0; i < patternSize; ++i) {
    Pattern pattern = m_patterns.front();
    std::cout << "\n" << i + 1 << ": " << pattern.getStmt().getName() << " ";
    std::cout << pattern.getLeft().getName() << " ";
    std::cout << pattern.getRight().getName() << " ";
    std::cout << pattern.isSubtree() << "\n";
    
    bool hasResult = getPatternResultFromPkb(pattern);
    if (!hasResult) {
      return false;
    }

    printPatternResultQueue();
    m_patterns.pop();
  }

  return true;
}

/**
* A function that stores the result in a data structure.
* @param t_result a vector<string> argument
*/
void QueryEvaluator::storeSelectResultFromPkb(vector<string> t_result) {
  printDivider();
  std::cout << "Storing the select result from PKB to the select result queue...\n";
  m_selectResults.push(t_result);
  printDivider();
}

/**
* A function that stores the result in a data structure.
* @param t_result an unordered_map<string, vector<string>> argument
* @param t_type an enum queryType argument
*/
void QueryEvaluator::storeResultFromPkb(std::unordered_map<std::string, std::vector<std::string>> t_result, queryType t_type) {
  //This part also needs to be re-implement after discussing with group...
  printDivider();
  std::cout << "Storing the result from PKB to different queues...\n";

  if (t_type == queryType::RELATION) {
    m_relationResults.push(t_result);
  } else if (t_type == queryType::PATTERN) {
    m_patternResults.push(t_result);
  } else {
    std::cout << "Result Type: " << t_type << "\n";
  }
  printDivider();
}

/**
* A function that evaluates the final result of the query by comparing the results based on what the query wants.
* @return The query results
*/
std::vector<std::string> QueryEvaluator::evaluateFinalResult() {
  printDivider();
  std::cout << "Evaluating the final result...\n";
  std::vector<std::string> finalResult;
  unordered_map<int, Grammar::GType> typeOfStmts = m_pkb->getTypeOfStatementTable();

  if (m_relationResults.empty() && m_patternResults.empty()) {
    std::cout << "CASE 1\n";
    if (!m_selectResults.empty()) {
      finalResult = m_selectResults.front();
    } 
  } else if (!m_relationResults.empty() && m_patternResults.empty()) {
    std::cout << "CASE 2\n";
    if (m_relations.front().getG1().getType() == Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
      std::cout << "STMT_NO SYNONYM\n";
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          if (m_selectedType == Grammar::GType::STMT) {
            if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
              finalResult.push_back(y);
            }
          } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
            if (typeOfStmts[stoi(y)] == m_selectedType) {
              finalResult.push_back(y);
            }
          } else {
            finalResult.push_back(y);
          }
        }
      }
    } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && (m_relations.front().getG2().getType() == Grammar::GType::STMT_NO || m_relations.front().getG2().getType() == Grammar::GType::STR)) {
      std::cout << "SYNONYM STMT_NO/STR\n";
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          if (m_selectedType == Grammar::GType::STMT) {
            if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
              finalResult.push_back(y);
            }
          } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
            if (typeOfStmts[stoi(y)] == m_selectedType) {
              finalResult.push_back(y);
            }
          } else {
            finalResult.push_back(y);
          }
        }
      }
    } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
      std::cout << "SYNONYM SYNONYM\n";
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      
      if (m_relations.front().getG1().getName() == m_selectedSynonym) {
        std::cout << "Selected Synonym 1: " << m_relations.front().getG1().getName() << "\n";
        if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
          for (auto& x : results) {
            for (auto& y : x.second) {
              if (m_selectedType == Grammar::GType::STMT) {
                if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
                  finalResult.push_back(y);
                }
              } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
                if (typeOfStmts[stoi(y)] == m_selectedType) {
                  finalResult.push_back(y);
                }
              } else {
                finalResult.push_back(y);
              }
            }
          }
        } else {
          for (auto& x : results) {
            if (!x.second.empty()) {
              if (m_selectedType == Grammar::GType::STMT) {
                if (typeOfStmts[stoi(x.first)] == m_selectedType || typeOfStmts[stoi(x.first)] == Grammar::GType::ASGN || typeOfStmts[stoi(x.first)] == Grammar::GType::WHILE) {
                  finalResult.push_back(x.first);
                }
              } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
                if (typeOfStmts[stoi(x.first)] == m_selectedType) {
                  finalResult.push_back(x.first);
                }
              } else {
                finalResult.push_back(x.first);
              }
            }
          }
        }   
      } else if (m_relations.front().getG2().getName() == m_selectedSynonym) {
        std::cout << "Selected Synonym 2: " << m_relations.front().getG2().getName() << "\n";
        if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
          for (auto& x : results) {
            if (!x.second.empty()) {
              if (m_selectedType == Grammar::GType::STMT) {
                if (typeOfStmts[stoi(x.first)] == m_selectedType || typeOfStmts[stoi(x.first)] == Grammar::GType::ASGN || typeOfStmts[stoi(x.first)] == Grammar::GType::WHILE) {
                  finalResult.push_back(x.first);
                }
              } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
                if (typeOfStmts[stoi(x.first)] == m_selectedType) {
                  finalResult.push_back(x.first);
                }
              } else {
                finalResult.push_back(x.first);
              }
            }
          }
        } else {
          for (auto& x : results) {
            for (auto& y : x.second) {
              if (m_selectedType == Grammar::GType::STMT) {
                if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
                  finalResult.push_back(y);
                }
              } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
                if (typeOfStmts[stoi(y)] == m_selectedType) {
                  finalResult.push_back(y);
                }
              } else {
                finalResult.push_back(y);
              }
            }
          }
        }   
      }
    }

    m_relations.pop();
    m_relationResults.pop();
  } else if (m_relationResults.empty() && !m_patternResults.empty()) {
    std::cout << "CASE 3\n";
    if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
      std::unordered_map<std::string, std::vector<std::string>> results = m_patternResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          if (m_selectedType == Grammar::GType::STMT) {
            if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
              finalResult.push_back(y);
            }
          } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
            if (typeOfStmts[stoi(y)] == m_selectedType) {
              finalResult.push_back(y);
            }
          } else {
            finalResult.push_back(y);
          }
        }
      }
    } else if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
      if (m_patterns.front().getLeft().getName() == m_selectedSynonym) {
        std::unordered_map<std::string, std::vector<std::string>> results = m_patternResults.front();
        for (auto& x : results) {
          for (auto& y : x.second) {
            if (m_selectedType == Grammar::GType::STMT) {
              if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
                finalResult.push_back(y);
              }
            } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
              if (typeOfStmts[stoi(y)] == m_selectedType) {
                finalResult.push_back(y);
              }
            } else {
              finalResult.push_back(y);
            }
          }
        }
      } else if (m_patterns.front().getStmt().getName() == m_selectedSynonym) {
        std::unordered_map<std::string, std::vector<std::string>> results = m_patternResults.front();
        for (auto& x : results) {
          if (m_selectedType == Grammar::GType::STMT) {
            if (typeOfStmts[stoi(x.first)] == m_selectedType || typeOfStmts[stoi(x.first)] == Grammar::GType::ASGN || typeOfStmts[stoi(x.first)] == Grammar::GType::WHILE) {
              finalResult.push_back(x.first);
            }
          } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
            if (typeOfStmts[stoi(x.first)] == m_selectedType) {
              finalResult.push_back(x.first);
            }
          } else {
            finalResult.push_back(x.first);
          }
        }
      }
    }

    m_patterns.pop();
    m_patternResults.pop();
  } else if (!m_relationResults.empty() && !m_patternResults.empty()) {
    std::unordered_map<std::string, int>::const_iterator got;
    got = m_synonymsUsedInQuery.find(m_selectedSynonym);

    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second == 1) { // Common Synonym between Relation and Pattern Clause
        std::cout << "CASE 4\n";
        if (m_relations.front().getG1().getType() == Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
          if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            unordered_map<string, vector<string>> relationResults = m_relationResults.front();
            unordered_map<string, vector<string>> patternResults = m_patternResults.front();
            for (auto& x : relationResults) {
              for (auto& y : patternResults) {
                vector<string> commonResults = getCommonResults(x.second, y.second);
                if (!commonResults.empty()) {
                  finalResult = m_selectResults.front();
                }
              }
            }
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
                unordered_map<string, vector<string>> relationResults = m_relationResults.front();
                unordered_map<string, vector<string>> patternResults = m_patternResults.front();
                for (auto& x : relationResults) {
                  for (auto& y : patternResults) {
                    vector<string> commonResults = getCommonResults(x.second, y.second);
                    if (!commonResults.empty()) {
                      finalResult = m_selectResults.front();
                    }
                  }
                }
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && (m_relations.front().getG2().getType() == Grammar::GType::STMT_NO || m_relations.front().getG2().getType() == Grammar::GType::STR)) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            unordered_map<string, vector<string>> relationResults = m_relationResults.front();
            unordered_map<string, vector<string>> patternResults = m_patternResults.front();
            for (auto& x : relationResults) {
              for (auto& y : patternResults) {
                vector<string> commonResults = getCommonResults(x.second, y.second);
                if (!commonResults.empty()) {
                  finalResult = m_selectResults.front();
                }
              }
            }
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG1().getName() == m_patterns.front().getLeft().getName()) {
                unordered_map<string, vector<string>> relationResults = m_relationResults.front();
                unordered_map<string, vector<string>> patternResults = m_patternResults.front();
                for (auto& x : relationResults) {
                  for (auto& y : patternResults) {
                    vector<string> commonResults = getCommonResults(x.second, y.second);
                    if (!commonResults.empty()) {
                      finalResult = m_selectResults.front();
                    }
                  }
                }
              }
            }
          }
        }
      } else if (got->second == 2) { // Common Synonym between Select + Relation or Select + Pattern and Relation + Pattern Clause
        std::cout << "CASE 5\n";
        vector<string> commonResults;
        if (m_relations.front().getG1().getType() == Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
          if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            unordered_map<string, vector<string>> relationResults = m_relationResults.front();
            unordered_map<string, vector<string>> patternResults = m_patternResults.front();
            for (auto& x : relationResults) {
              for (auto& y : patternResults) {
                commonResults = getCommonResults(x.second, y.second);
              }
            }
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
                unordered_map<string, vector<string>> relationResults = m_relationResults.front();
                unordered_map<string, vector<string>> patternResults = m_patternResults.front();
                for (auto& x : relationResults) {
                  for (auto& y : patternResults) {
                    commonResults = getCommonResults(x.second, y.second);
                  }
                }
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && (m_relations.front().getG2().getType() == Grammar::GType::STMT_NO || m_relations.front().getG2().getType() == Grammar::GType::STR)) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            unordered_map<string, vector<string>> relationResults = m_relationResults.front();
            unordered_map<string, vector<string>> patternResults = m_patternResults.front();
            for (auto& x : relationResults) {
              for (auto& y : patternResults) {
                commonResults = getCommonResults(x.second, y.second);
              }
            }
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG1().getName() == m_patterns.front().getLeft().getName()) {
                unordered_map<string, vector<string>> relationResults = m_relationResults.front();
                unordered_map<string, vector<string>> patternResults = m_patternResults.front();
                for (auto& x : relationResults) {
                  for (auto& y : patternResults) {
                    commonResults = getCommonResults(x.second, y.second);
                  }
                }
              }
            }
          }
        }

        if (!commonResults.empty()) {
          if (m_selectedSynonym == m_relations.front().getG1().getName() && m_relations.front().getG1().getType() == m_selectedType) {
            if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
              for (auto& x : m_relationResults.front()) {
                if (find(commonResults.begin(), commonResults.end(), x.first) != commonResults.end()) {
                  finalResult.insert(finalResult.end(), x.second.begin(), x.second.end());
                }
              }
            } else {
              for (auto& x : m_relationResults.front()) {
                if (!getCommonResults(x.second, commonResults).empty()) {
                  finalResult.push_back(x.first);
                }
              }
            }
          } else if (m_selectedSynonym == m_relations.front().getG2().getName() && m_relations.front().getG2().getType() == m_selectedType) {
            if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
              for (auto& x : m_relationResults.front()) {
                if (!getCommonResults(x.second, commonResults).empty()) {
                  finalResult.push_back(x.first);
                }
              }
            } else {
              for (auto& x : m_relationResults.front()) {

              }
            }
          } else if (m_selectedSynonym == m_patterns.front().getStmt().getName() && m_patterns.front().getStmt().getType() == m_selectedType) {
            for (auto& x : m_relationResults.front()) {

            }
          } else if (m_selectedSynonym == m_patterns.front().getLeft().getName() && m_patterns.front().getLeft().getType() == m_selectedType) {
            for (auto& x : m_relationResults.front()) {

            }
          }
        }
      } else if (got->second > 2) { // Common Synonym between Select, Relation and Pattern Clause
        std::cout << "CASE 6\n";
        if (m_relations.front().getG1().getType() == Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
          if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            unordered_map<string, vector<string>> relationResults = m_relationResults.front();
            unordered_map<string, vector<string>> patternResults = m_patternResults.front();
            for (auto& x : relationResults) {
              for (auto& y : patternResults) {
                vector<string> commonResults = getCommonResults(x.second, y.second);
                if (!commonResults.empty()) {
                  finalResult = commonResults;
                }
              }
            }
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
                unordered_map<string, vector<string>> relationResults = m_relationResults.front();
                unordered_map<string, vector<string>> patternResults = m_patternResults.front();
                for (auto& x : relationResults) {
                  for (auto& y : patternResults) {
                    vector<string> commonResults = getCommonResults(x.second, y.second);
                    if (!commonResults.empty()) {
                      finalResult = commonResults;
                    }
                  }
                }
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && (m_relations.front().getG2().getType() == Grammar::GType::STMT_NO || m_relations.front().getG2().getType() == Grammar::GType::STR)) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            unordered_map<string, vector<string>> relationResults = m_relationResults.front();
            unordered_map<string, vector<string>> patternResults = m_patternResults.front();
            for (auto& x : relationResults) {
              for (auto& y : patternResults) {
                vector<string> commonResults = getCommonResults(x.second, y.second);
                if (!commonResults.empty()) {
                  finalResult = commonResults;
                }
              }
            }
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG1().getName() == m_patterns.front().getLeft().getName()) {
                unordered_map<string, vector<string>> relationResults = m_relationResults.front();
                unordered_map<string, vector<string>> patternResults = m_patternResults.front();
                for (auto& x : relationResults) {
                  for (auto& y : patternResults) {
                    vector<string> commonResults = getCommonResults(x.second, y.second);
                    if (!commonResults.empty()) {
                      finalResult = commonResults;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    m_relations.pop();
    m_patterns.pop();
    m_relationResults.pop();
    m_patternResults.pop();
  }

  std::cout << "Query Result: \n";
  for (std::vector<std::string>::iterator getStmts = finalResult.begin(); getStmts != finalResult.end(); ++getStmts) {
    std::cout << *getStmts << ", ";
  }

  printDivider();
  return finalResult;
}