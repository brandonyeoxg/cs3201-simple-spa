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
      std::cout << "\n" << j << ". " << x.first << " ";
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
      std::cout << "\n" << j << ". " << x.first << " ";
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

  std::cout << "\nSelect Queue\n";
  for (int i = 0; i < selectSize; ++i) {
    Grammar grammar = m_selects.front();
    std::cout << i + 1 << ": " << grammar.getName() << "\n";
    m_selectedSynonym = grammar.getName();

    if (grammar.getType() != Grammar::GType::VAR && grammar.getType() != Grammar::GType::CONST) {
      // Call the PKB API getStatementTypeTable().
      std::unordered_map<Grammar::GType, std::vector<int>> allStmts = m_pkb->getStatementTypeTable();

      // Check if there are results else return false.
      if (allStmts.empty()) {
        return false;
      }

      // Get all the statements of the type of statements that has been selected.
      std::vector<int> allSelectedStmtsInInt;
      if (grammar.getType() == Grammar::GType::STMT || grammar.getType() == Grammar::GType::PROG_LINE) {
        allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[Grammar::GType::ASGN].begin(), allStmts[Grammar::GType::ASGN].end());
        allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[Grammar::GType::WHILE].begin(), allStmts[Grammar::GType::WHILE].end());
      } else if (grammar.getType() == Grammar::GType::ASGN) {
        allSelectedStmtsInInt = allStmts[Grammar::GType::ASGN];
      } else if (grammar.getType() == Grammar::GType::WHILE) {
        allSelectedStmtsInInt = allStmts[Grammar::GType::WHILE];
      }

      // Change from vector<int> to vector<string>
      std::vector<std::string> allSelectedStmts;
      for (std::vector<int>::iterator getStmts = allSelectedStmtsInInt.begin(); getStmts != allSelectedStmtsInInt.end(); ++getStmts) {
        allSelectedStmts.push_back(std::to_string(*getStmts));
      }

      // Push into the selectResults queue.
      m_selectResults.push(allSelectedStmts);
    } else if (grammar.getType() == Grammar::GType::VAR) {
      std::vector<std::string> allVariables; //= m_pkb->getAllVariables();
      m_selectResults.push(allVariables);
    } else if (grammar.getType() == Grammar::GType::CONST) {
      std::vector<std::string> allConstants; //= m_pkb->getAllConstants();
      m_selectResults.push(allConstants);
    }

    // Print Select Results Queue
    printSelectResultQueue();

    // Move the item to the back of the queue.
    m_selects.pop();
    m_selects.push(grammar);
  }

  std::cout << "\nRelation Queue\n";
  for (int i = 0; i < relationSize; ++i) {
    std::unordered_map<std::string, std::vector<std::string>> result;
    Relation relation = m_relations.front();
    std::cout << i + 1 << ": " << relation.getType() << " ";
    std::cout << relation.getG1().getName() << " ";
    std::cout << relation.getG2().getName() << "\n";

    // Call the PKB API to get the results of the relation clauses
    if (relation.getType() == Relation::RType::FOLLOWS) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        if (m_pkb->isFollows(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()))) {
          std::cout << "Follows: True\n";
          continue;
        } else if (!m_pkb->isFollows(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()))) {
          std::cout << "Follows: False\n";
          return false;
        } else {
          std::cout << "Exception in QueryEvaluator.cpp for isFollows(s1, s2)\n";
          return false;
        }
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        int stmtNo;
        try {
          stmtNo = m_pkb->getFollows(std::stoi(relation.getG1().getName()));
          std::cout << "getFollows - STMT NO: " << stmtNo << "\n";
        } catch (const std::invalid_argument& ia) {
          std::cout << "Invalid Argument Exception - No Results for getFollows(s1)\n";
          return false;
        }

        std::vector<std::string> stmtVector;
        stmtVector.push_back(std::to_string(stmtNo));
        result[relation.getG2().getName()] = stmtVector;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        int stmtNo;
        try {
          stmtNo = m_pkb->getFollowedBy(std::stoi(relation.getG2().getName()));
          std::cout << "getFollowedBy - STMT NO: " << stmtNo << "\n";
        } catch (const std::invalid_argument& ia) {
          std::cout << "Invalid Argument Exception - No Results for getFollowedBy(s2)\n";
          return false;
        }

        std::vector<std::string> stmtVector;
        stmtVector.push_back(std::to_string(stmtNo));
        result[relation.getG1().getName()] = stmtVector;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        // Todo: Get the whole Follows Table.
        //result = m_pkb->getAllFollows();
      }
    } else if (relation.getType() == Relation::RType::FOLLOWS_) {
      std::cout << "FOLLOWS STAR\n";
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        if (m_pkb->isFollowsStar(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()))) {
          std::cout << "Follows*: True\n";
          continue;
        } else if (!m_pkb->isFollows(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()))) {
          std::cout << "Follows*: False\n";
          return false;
        } else {
          std::cout << "Exception in QueryEvaluator.cpp Line 93\n";
          return false;
        }
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        std::vector<int> stmtIntVector = m_pkb->getFollowsStar(std::stoi(relation.getG1().getName()));
        if (stmtIntVector.empty()) {
          return false;
        }
        std::vector<std::string> stmtStrVector;
        for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
          stmtStrVector.push_back(std::to_string(*getStmts));
          std::cout << "Follows STMTS: " << *getStmts << " ";
        }
        std::cout << "\n";
        result[relation.getG2().getName()] = stmtStrVector;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        std::vector<int> stmtIntVector = m_pkb->getFollowedByStar(std::stoi(relation.getG2().getName()));
        if (stmtIntVector.empty()) {
          return false;
        }
        std::vector<std::string> stmtStrVector;
        for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
          stmtStrVector.push_back(std::to_string(*getStmts));
          std::cout << "Followed By STMTS: " << *getStmts << " ";
        }
        std::cout << "\n";
        result[relation.getG1().getName()] = stmtStrVector;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        // Todo: Get the whole Follows Table.
        //result = m_pkb->getAllFollows();
      }
    } else if (relation.getType() == Relation::RType::PARENT) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        if (m_pkb->isParent(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()))) {
          std::cout << "Parent: True\n";
          continue;
        } else if (!m_pkb->isParent(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()))) {
          std::cout << "Parent: False\n";
          return false;
        } else {
          std::cout << "Exception in QueryEvaluator.cpp Line 93\n";
          return false;
        }
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        std::vector<int> stmtIntVector = m_pkb->getChildrenOf(std::stoi(relation.getG1().getName()));
        if (stmtIntVector.empty()) {
          return false;
        }
        std::vector<std::string> stmtStrVector;
        for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
          stmtStrVector.push_back(std::to_string(*getStmts));
        }
        result[relation.getG2().getName()] = stmtStrVector;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        int stmtNo;
        try {
          stmtNo = m_pkb->getParentOf(std::stoi(relation.getG2().getName()));
        } catch (const std::invalid_argument& ia) {
          std::cout << "Invalid Argument Exception - No Results for getParentOf(s2)\n";
          return false;
        }
        std::vector<std::string> stmtVector;
        stmtVector.push_back(std::to_string(stmtNo));
        result[relation.getG1().getName()] = stmtVector;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        // Todo: Get the whole Parent Table.
        //result = m_pkb->getAllParent();
      }
    } else if (relation.getType() == Relation::RType::PARENT_) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        if (m_pkb->isParentStar(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()))) {
          std::cout << "Parent*: True\n";
          continue;
        } else if (!m_pkb->isParentStar(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()))) {
          std::cout << "Parent*: False\n";
          return false;
        } else {
          std::cout << "Exception in QueryEvaluator.cpp Line 93\n";
          return false;
        }
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        std::vector<int> stmtIntVector = m_pkb->getChildrenStarOf(std::stoi(relation.getG1().getName()));
        if (stmtIntVector.empty()) {
          return false;
        }
        std::vector<std::string> stmtStrVector;
        for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
          stmtStrVector.push_back(std::to_string(*getStmts));
        }
        result[relation.getG2().getName()] = stmtStrVector;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        std::vector<int> stmtIntVector = m_pkb->getParentStarOf(std::stoi(relation.getG2().getName()));
        if (stmtIntVector.empty()) {
          return false;
        }
        std::vector<std::string> stmtStrVector;
        for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
          stmtStrVector.push_back(std::to_string(*getStmts));
        }
        result[relation.getG1().getName()] = stmtStrVector;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        // Todo: Get the whole Parent Table.
        //result = m_pkb->getAllParent();
      }
    } else if (relation.getType() == Relation::RType::USES) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::EXPR) {
        if (m_pkb->isUses(std::stoi(relation.getG1().getName()), relation.getG2().getName())) {
          std::cout << "Uses: True\n";
          continue;
        } else if (!m_pkb->isUses(std::stoi(relation.getG1().getName()), relation.getG2().getName())) {
          std::cout << "Uses: False\n";
          return false;
        } else {
          std::cout << "Exception in QueryEvaluator.cpp for isUses(s, v)\n";
          return false;
        }
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::VAR) {
        std::cout << "USES (CONST, VAR) WORKS!\n";
        std::vector<std::string> varUsedByStmt = m_pkb->getUses((std::stoi(relation.getG1().getName())));
        if (varUsedByStmt.empty()) {
          return false;
        }
        
        result[relation.getG2().getName()] = varUsedByStmt;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::EXPR) {
        std::cout << "USES (STMT, EXPR) WORKS!\n";
        std::vector<int> stmtIntVector = m_pkb->getStmtUses(relation.getG2().getName());
        if (stmtIntVector.empty()) {
          return false;
        }

        std::vector<std::string> stmtStrVector;
        for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
          stmtStrVector.push_back(std::to_string(*getStmts));
        }

        result[relation.getG1().getName()] = stmtStrVector;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::VAR) {
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
    } else if (relation.getType() == Relation::RType::MODIFIES) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::EXPR) {
        if (m_pkb->isModifies(std::stoi(relation.getG1().getName()), relation.getG2().getName())) {
          std::cout << "Modifies: True\n";
          continue;
        } else if (!m_pkb->isModifies(std::stoi(relation.getG1().getName()), relation.getG2().getName())) {
          std::cout << "Modifies: False\n";
          return false;
        } else {
          std::cout << "Exception in QueryEvaluator.cpp for Modifies(s, v)\n";
          return false;
        }
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::VAR) {
        std::cout << "Modifies (CONST, VAR) WORKS!\n";
        std::vector<std::string> varModifiedByStmt = m_pkb->getModifies((std::stoi(relation.getG1().getName())));
        if (varModifiedByStmt.empty()) {
          return false;
        }

        result[relation.getG2().getName()] = varModifiedByStmt;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::EXPR) {
        std::cout << "Modifies (STMT, EXPR) WORKS!\n";
        std::vector<int> stmtIntVector = m_pkb->getStmtModifies(relation.getG2().getName());
        if (stmtIntVector.empty()) {
          return false;
        }

        std::vector<std::string> stmtStrVector;
        for (std::vector<int>::iterator getStmts = stmtIntVector.begin(); getStmts != stmtIntVector.end(); ++getStmts) {
          stmtStrVector.push_back(std::to_string(*getStmts));
        }

        result[relation.getG1().getName()] = stmtStrVector;
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::VAR) {
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
      std::cout << "Relation Type: " + relation.getType() << "\n";
    }
  

    if (result.empty()) {
      return false;
    }

    // Print the result
    std::cout << "\nRelation Result: \n";
    for (auto& x : result) {
      std::cout << x.first << ": ";
      for (std::vector<std::string>::iterator getStmts = x.second.begin(); getStmts != x.second.end(); ++getStmts) {
        std::cout << *getStmts << " ";
      }
      std::cout << "\n";
    }

    std::unordered_map<std::string, int>::const_iterator got;
    if (relation.getG1().getType() != Grammar::GType::CONST) {
      got = m_synonymsUsedInQuery.find(relation.getG1().getName());
    } else if (relation.getG2().getType() != Grammar::GType::CONST && relation.getG2().getType() != Grammar::GType::EXPR) {
      got = m_synonymsUsedInQuery.find(relation.getG2().getName());
    }

    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::RELATION);
        m_relations.push(relation);
      }
    }
    
    m_relations.pop();
  }

  std::cout << "\nPattern Queue\n";
  for (int i = 0; i < patternSize; ++i) {
    Pattern pattern = m_patterns.front();
    std::cout << i + 1 << ": " << pattern.getStmt().getName() << " ";
    std::cout << pattern.getLeft().getName() << " ";
    std::cout << pattern.getRight().getName() << " ";
    std::cout << pattern.isSubtree() << "\n";
    
    // Todo: Get results for pattern clauses by calling the API from PKB
    if (pattern.getStmt().getType() == Grammar::GType::ASGN) {
      if (pattern.getLeft().getName() != "_" && pattern.getRight().getName() != "_") {
        if (!pattern.isSubtree()) {

        } else if (pattern.isSubtree()) {

        }
      }
    }

    m_patterns.pop();
    m_patterns.push(pattern);
  }

  return true;
}

/**
* A function that stores the result in a data structure.
* @param t_result a vector<string> argument
* @param t_type a string argument
*/
void QueryEvaluator::storeResultFromPkb(std::unordered_map<std::string, std::vector<std::string>> t_result, queryType t_type) {
  //This part also needs to be re-implement after discussing with group...
  std::cout << "\n*************************************\n";
  std::cout << "Storing the result from PKB to different queues...\n";

  if (t_type == queryType::SELECT) {
    //m_selectResults.push(t_result);
  } else if (t_type == queryType::RELATION) {
    m_relationResults.push(t_result);
  } else if (t_type == queryType::PATTERN) {
    m_patternResults.push(t_result);
  } else {
    std::cout << "Result Type: " << t_type << "\n";
  }
}

/**
* A function that evaluates the final result of the query by comparing the results based on what the query wants.
* @return The query results
*/
std::vector<std::string> QueryEvaluator::evaluateFinalResult() {
  std::cout << "\n*************************************\n";
  std::cout << "Evaluating the final result...\n";
  std::vector<std::string> finalResult;

  if (m_relationResults.empty() && m_patternResults.empty()) {
    if (!m_selectResults.empty()) {
      finalResult = m_selectResults.front();
    } 
  } else if (!m_relationResults.empty() && m_patternResults.empty()) {
    if (m_relations.front().getG1().getType() == Grammar::GType::CONST && m_relations.front().getG2().getType() != Grammar::GType::CONST && m_relations.front().getG2().getType() != Grammar::GType::EXPR) {
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          finalResult.push_back(y);
        }
      }
    } else if (m_relations.front().getG1().getType() != Grammar::GType::CONST && (m_relations.front().getG2().getType() == Grammar::GType::CONST || m_relations.front().getG2().getType() == Grammar::GType::EXPR)) {
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          finalResult.push_back(y);
        }
      }
    } else if (m_relations.front().getG1().getType() != Grammar::GType::CONST && m_relations.front().getG2().getType() != Grammar::GType::CONST && m_relations.front().getG2().getType() != Grammar::GType::EXPR) {
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      
      if (m_relations.front().getG1().getName() == m_selectedSynonym) {
        std::cout << "Selected Synonym 1: " << m_relations.front().getG1().getName() << "\n";
        if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
          for (auto& x : results) {
            for (auto& y : x.second) {
              finalResult.push_back(y);
            }
          }
        } else {
          for (auto& x : results) {
            finalResult.push_back(x.first);
          }
        }   
      } else if (m_relations.front().getG2().getName() == m_selectedSynonym) {
        std::cout << "Selected Synonym 2: " << m_relations.front().getG2().getName() << "\n";
        if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
          for (auto& x : results) {
            finalResult.push_back(x.first);
          }
        } else {
          for (auto& x : results) {
            for (auto& y : x.second) {
              finalResult.push_back(y);
            }
          }
        }   
      }
    }

    m_relations.pop();
    m_relationResults.pop();
  } else if (m_relationResults.empty() && !m_patternResults.empty()) {
    if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          finalResult.push_back(y);
        }
      }
    } else if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
      if (m_patterns.front().getLeft().getName() == m_selectedSynonym) {
        std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
        for (auto& x : results) {
          for (auto& y : x.second) {
            finalResult.push_back(y);
          }
        }
      } else if (m_patterns.front().getStmt().getName() == m_selectedSynonym) {
        std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
        for (auto& x : results) {
          finalResult.push_back(x.first);
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
        if (m_relations.front().getG1().getType() == Grammar::GType::CONST && m_relations.front().getG2().getType() != Grammar::GType::CONST) {

        }
      } else if (got->second > 1) { // Common Synonym between Select, Relation and Pattern Clause
          
      }
    }

    m_relations.pop();
    m_patterns.pop();
    m_relationResults.pop();
    m_patternResults.pop();
  }

  while (!m_relationResults.empty()) {
    //Todo: format result to vector<string>
    std::unordered_map<std::string, std::vector<std::string>> resultMap = m_relationResults.front();
    Relation relation = m_relations.front();
    if (relation.getType() == Relation::RType::FOLLOWS || relation.getType() == Relation::RType::FOLLOWS_) {
      if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        std::unordered_map<std::string, std::vector<std::string>>::const_iterator getVector = resultMap.find(relation.getG1().getName());
        std::vector<std::string> stmtVector = getVector->second;
        for (std::vector<std::string>::iterator getStmts = stmtVector.begin(); getStmts != stmtVector.end(); ++getStmts) {
          finalResult.push_back(*getStmts);
        }
      } else if (relation.getG2().getType() == Grammar::GType::STMT && relation.getG1().getType() == Grammar::GType::CONST) {
        std::unordered_map<std::string, std::vector<std::string>>::const_iterator getVector = resultMap.find(relation.getG2().getName());
        std::vector<std::string> stmtVector = getVector->second;
        for (std::vector<std::string>::iterator getStmts = stmtVector.begin(); getStmts != stmtVector.end(); ++getStmts) {
          finalResult.push_back(*getStmts);
        }
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        //Todo: Get the results for query Select s1 such that Follows (s1, s2) or Select s2 such that Follows (s1, s2)
      }
    }
    else if (relation.getType() == Relation::RType::PARENT || relation.getType() == Relation::RType::PARENT_) {
      if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        std::unordered_map<std::string, std::vector<std::string>>::const_iterator getVector = resultMap.find(relation.getG1().getName());
        std::vector<std::string> stmtVector = getVector->second;
        for (std::vector<std::string>::iterator getStmts = stmtVector.begin(); getStmts != stmtVector.end(); ++getStmts) {
          finalResult.push_back(*getStmts);
        }
      }
      else if (relation.getG2().getType() == Grammar::GType::STMT && relation.getG1().getType() == Grammar::GType::CONST) {
        std::unordered_map<std::string, std::vector<std::string>>::const_iterator getVector = resultMap.find(relation.getG2().getName());
        std::vector<std::string> stmtVector = getVector->second;
        for (std::vector<std::string>::iterator getStmts = stmtVector.begin(); getStmts != stmtVector.end(); ++getStmts) {
          finalResult.push_back(*getStmts);
        }
      }
      else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        //Todo: Get the results for query Select s1 such that Follows (s1, s2) or Select s2 such that Follows (s1, s2)
      }
    } else if (relation.getType() == Relation::RType::USES) {
      //Todo: Get the results for Uses
    } else if (relation.getType() == Relation::RType::MODIFIES) {
      //Todo: Get ther results for Modifies
    } else {
      std::cout << "Relation Type: " << relation.getType() << "\n";
    }

    m_relations.pop();
    m_relationResults.pop();
  }

  while (!m_patternResults.empty()) {
    //Todo: format result to vector<string>
  }

  std::cout << "Query Result: \n";
  for (std::vector<std::string>::iterator getStmts = finalResult.begin(); getStmts != finalResult.end(); ++getStmts) {
    std::cout << *getStmts << ", ";
  }

  printDivider();
  return finalResult;
}