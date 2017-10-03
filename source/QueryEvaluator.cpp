#pragma once

#include "QueryEvaluator.h"

/**
* A function that evaluates the query that has been pre-processed by the QueryPreprocessor.
*/
std::vector<std::string> QueryEvaluator::evaluateQuery() {
  //printDivider();
  //std::cout << "Evaluating Query...\n";

  // Debug: Check the queues
  /*printSelectQueue();
  printRelationQueue();
  printPatternQueue();*/

  bool hasResult = getResultFromPkb();
  if (hasResult) {
    return evaluateFinalResult();
  } else if (!hasResult) {
    std::vector<std::string> result;
    /*printDivider();
    std::cout << "No Query Result: \n";
    for (auto& x : result) {
      std::cout << x << ", ";
    }
    printDivider();*/
    return result;
  }
}

std::vector<std::string> QueryEvaluator::formatVectorIntToVectorString(std::vector<int> t_vectorInt) {
  std::vector<std::string> vectorStr;
  for (auto& x : t_vectorInt) {
    vectorStr.push_back(std::to_string(x));
  }

  return vectorStr;
}

std::vector<std::string> QueryEvaluator::formatListStringToVectorString(std::list<std::string> t_listStr) {
  std::vector<std::string> vectorStr;
  for (auto& x : t_listStr) {
    vectorStr.push_back(x);
  }

  return vectorStr;
}

std::vector<std::string> QueryEvaluator::getCommonResults(std::vector<std::string> t_resultVector1, std::vector<std::string> t_resultVector2) {
  std::vector<std::string> commonResultVector;

  sort(t_resultVector1.begin(), t_resultVector1.end());
  sort(t_resultVector2.begin(), t_resultVector2.end());

  set_intersection(t_resultVector1.begin(), t_resultVector1.end(), t_resultVector2.begin(), t_resultVector2.end(), back_inserter(commonResultVector));

  return commonResultVector;
}

bool QueryEvaluator::isAllUnderscores(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() == "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() == "_";
}

bool QueryEvaluator::hasNoSynonyms(Grammar t_g1, Grammar t_g2) {
  return (t_g1.getType() == queryType::GType::STMT_NO || t_g1.getType() == queryType::GType::STR) && (t_g2.getType() == queryType::GType::STMT_NO || t_g2.getType() == queryType::GType::STR);
}

bool QueryEvaluator::hasOneRightSynonym(Grammar t_g1, Grammar t_g2) {
  return (t_g1.getType() == queryType::GType::STMT_NO || t_g1.getType() == queryType::GType::STR) && t_g2.getType() != queryType::GType::STMT_NO && t_g2.getType() != queryType::GType::STR;
}

bool QueryEvaluator::hasOneLeftSynonym(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() != queryType::GType::STMT_NO && t_g1.getType() != queryType::GType::STR && (t_g2.getType() == queryType::GType::STMT_NO || t_g2.getType() == queryType::GType::STR);
}

bool QueryEvaluator::hasTwoSynonyms(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() != queryType::GType::STMT_NO && t_g1.getType() != queryType::GType::STR && t_g2.getType() != queryType::GType::STMT_NO && t_g2.getType() != queryType::GType::STR;
}

bool QueryEvaluator::getSelectResultFromPkb(Grammar t_select) {
  if (t_select.getType() != queryType::GType::PROC && t_select.getType() != queryType::GType::ST_LST && t_select.getType() != queryType::GType::VAR && t_select.getType() != queryType::GType::CONST) {
    // Call the PKB API getStatementTypeTable().
    std::unordered_map<queryType::GType, std::vector<int>> allStmts = m_pkb->getStatementTypeTable();

    // Check if there are results else return false.
    if (allStmts.empty()) {
      return false;
    }

    // Get all the statements of the type of statements that has been selected.
    std::vector<int> allSelectedStmtsInInt;
    if (t_select.getType() == queryType::GType::STMT || t_select.getType() == queryType::GType::PROG_LINE) {
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[queryType::GType::ASGN].begin(), allStmts[queryType::GType::ASGN].end());
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[queryType::GType::WHILE].begin(), allStmts[queryType::GType::WHILE].end());
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[queryType::GType::IF].begin(), allStmts[queryType::GType::IF].end());
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[queryType::GType::CALL].begin(), allStmts[queryType::GType::CALL].end());
    } else if (t_select.getType() == queryType::GType::ASGN) {
      allSelectedStmtsInInt = allStmts[queryType::GType::ASGN];
    } else if (t_select.getType() == queryType::GType::WHILE) {
      allSelectedStmtsInInt = allStmts[queryType::GType::WHILE];
    } else if (t_select.getType() == queryType::GType::IF) {
      allSelectedStmtsInInt = allStmts[queryType::GType::IF];
    } else if (t_select.getType() == queryType::GType::CALL) {
      allSelectedStmtsInInt = allStmts[queryType::GType::CALL];
    }

    // Change from vector<int> to vector<string>.
    std::vector<std::string> allSelectedStmts = formatVectorIntToVectorString(allSelectedStmtsInInt);

    // Push into the selectResults queue.
    storeSelectResultFromPkb(allSelectedStmts);
  } else if (t_select.getType() == queryType::GType::VAR) {
    std::vector<std::string> allVariables = m_pkb->getAllVariables();
    if (allVariables.empty()) {
      return false;
    }

    storeSelectResultFromPkb(allVariables);
  } else if (t_select.getType() == queryType::GType::CONST) {
    std::list<std::string> constantsList = m_pkb->getAllConstants();
    std::vector<std::string> allConstants = formatListStringToVectorString(constantsList);
    if (allConstants.empty()) {
      return false;
    }

    storeSelectResultFromPkb(allConstants);
  } else if (t_select.getType() == queryType::GType::PROC) {
    std::vector<std::string> allProcedures = m_pkb->getAllProcsName();
    if (allProcedures.empty()) {
      return false;
    }

    storeSelectResultFromPkb(allProcedures);
  } else if (t_select.getType() == queryType::GType::ST_LST) {
    std::vector<int> allStmtLst = m_pkb->getStmtList();
    if (allStmtLst.empty()) {
      return false;
    }

    std::vector<std::string> allStmtList = formatVectorIntToVectorString(allStmtLst);
    storeSelectResultFromPkb(allStmtList);
  }

  return true;
}

bool QueryEvaluator::getRelationResultFromPkb(Relation t_relation) {
  std::unordered_map<std::string, std::vector<std::string>> result;
  std::unordered_map<int, queryType::GType> typeOfStmts = m_pkb->getTypeOfStatementTable();
  Evaluator *eval = Relationship::createEvaluator(t_relation.getType());
  Grammar g1 = t_relation.getG1();
  Grammar g2 = t_relation.getG2();

  // Get the respective evaluators to get the results of the relation clauses
  if (isAllUnderscores(g1, g2)) {
    bool result = eval->hasRelationship(m_pkb, g1, g2);
    return result;
  } else if (hasNoSynonyms(g1, g2)) {
    bool result = eval->isRelationTrue(m_pkb, g1, g2);
    return result;
  } else if (hasOneRightSynonym(g1, g2)) {
    result = eval->evaluateRightSynonym(m_pkb, g1, g2);
  } else if (hasOneLeftSynonym(g1, g2)) {
    result = eval->evaluateLeftSynonym(m_pkb, g1, g2);
  } else if (hasTwoSynonyms(g1, g2)) {
    result = eval->evaluateBothSynonyms(m_pkb, g1, g2);
  } 

  delete eval;

  if (result.empty()) {
    return false;
  }

  // Store the result
  std::unordered_map<std::string, int>::const_iterator got;
  if ((t_relation.getG1().getType() != queryType::GType::STMT_NO || t_relation.getG1().getType() != queryType::GType::STR) && (t_relation.getG2().getType() == queryType::GType::STMT_NO || t_relation.getG2().getType() == queryType::GType::STR)) {
    got = m_synonymsUsedInQuery.find(t_relation.getG1().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::RELATION);
        m_relations.push(t_relation);
      }
    }
  } else if ((t_relation.getG1().getType() == queryType::GType::STMT_NO || t_relation.getG1().getType() == queryType::GType::STR) && t_relation.getG2().getType() != queryType::GType::STMT_NO && t_relation.getG2().getType() != queryType::GType::STR) {
    got = m_synonymsUsedInQuery.find(t_relation.getG2().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::RELATION);
        m_relations.push(t_relation);
      }
    }
  } else if (t_relation.getG1().getType() != queryType::GType::STMT_NO && t_relation.getG1().getType() != queryType::GType::STR && t_relation.getG2().getType() != queryType::GType::STMT_NO && t_relation.getG2().getType() != queryType::GType::STR) {
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
  PatternEvaluator *eval = Patterns::createEvaluator(t_pattern.getStmt().getType());
  Grammar stmt = t_pattern.getStmt();
  Grammar g1 = t_pattern.getLeft();
  Grammar g2 = t_pattern.getRight();
  bool isExact = !t_pattern.isSubtree();

  // Get the respective evaluators to get the results of the pattern clauses
  if (g1.getType() == queryType::GType::STR && g1.getName() == "_" && g2.getType() == queryType::GType::STR && g2.getName() == "_") {
    result = eval->getAllStmtsWithAnyPattern(m_pkb, stmt, g1, g2); //Only underscores
  } else if (g1.getType() == queryType::GType::STR && g1.getName() == "_" && g2.getType() == queryType::GType::STR && isExact) {
    result = eval->getAllStmtsWithExactPattern(m_pkb, stmt, g1, g2); //underscore + STR
  } else if (g1.getType() == queryType::GType::STR && g1.getName() == "_" && g2.getType() == queryType::GType::STR && !isExact) {
    result = eval->getAllStmtsWithSubPattern(m_pkb, stmt, g1, g2); //underscore + _STR_
  } else if (g1.getType() == queryType::GType::STR && g2.getType() == queryType::GType::STR && g2.getName() == "_") {
    result = eval->getAllStmtsWithVarAndAnyPattern(m_pkb, stmt, g1, g2); //STR + underscore
  } else if (g1.getType() == queryType::GType::VAR && g2.getType() == queryType::GType::STR && g2.getName() == "_") {
    result = eval->getAllStmtsAndVarWithAnyPattern(m_pkb, stmt, g1, g2); //VAR + underscore
  } else if (g1.getType() == queryType::GType::STR && g2.getType() == queryType::GType::STR && isExact) {
    result = eval->getAllStmtsWithVarAndExactPattern(m_pkb, stmt, g1, g2); //STR + STR
  } else if (g1.getType() == queryType::GType::STR && g2.getType() == queryType::GType::STR && !isExact) {
    result = eval->getAllStmtsWithVarAndSubPattern(m_pkb, stmt, g1, g2); //STR + _STR_
  } else if (g1.getType() == queryType::GType::VAR && g2.getType() == queryType::GType::STR && isExact) {
    result = eval->getAllStmtsAndVarWithExactPattern(m_pkb, stmt, g1, g2); //VAR + STR
  } else if (g1.getType() == queryType::GType::VAR && g2.getType() == queryType::GType::STR && !isExact) {
    result = eval->getAllStmtsAndVarWithSubPattern(m_pkb, stmt, g1, g2); //VAR + _STR_
  }

  delete eval;

  if (result.empty()) {
    return false;
  }

  // Store the result
  std::unordered_map<std::string, int>::const_iterator got;
  if (t_pattern.getLeft().getType() != queryType::GType::VAR) {
    got = m_synonymsUsedInQuery.find(t_pattern.getStmt().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::PATTERN);
        m_patterns.push(t_pattern);
      }
    }
  } else if (t_pattern.getLeft().getType() == queryType::GType::VAR) {
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
  //printDivider();
  //std::cout << "Getting results from PKB...\n";
  int selectSize = m_selects.size();
  int relationSize = m_relations.size();
  int patternSize = m_patterns.size();

  //std::cout << "\nSelect Queue";
  for (int i = 0; i < selectSize; ++i) {
    Grammar grammar = m_selects.front();
    m_selectedSynonym = grammar.getName();
    m_selectedType = grammar.getType();
    //std::cout << "\n" << i + 1 << ": " << grammar.getName() << "\n";
    
    // Get Select Results from PKB.
    bool hasResult = getSelectResultFromPkb(grammar);

    // Check if there are results else return false.
    if (!hasResult) {
      return false;
    }

    // Print Select Results Queue.
    //printSelectResultQueue();

    // Move the item to the back of the queue.
    m_selects.pop();
    m_selects.push(grammar);
  }

  //std::cout << "\nRelation Queue";
  for (int i = 0; i < relationSize; ++i) {
    Relation relation = m_relations.front();
    /*std::cout << "\n" << i + 1 << ": " << relation.getType() << " ";
    std::cout << relation.getG1().getName() << " ";
    std::cout << relation.getG2().getName() << "\n";*/

    bool hasResult = getRelationResultFromPkb(relation);
    if (!hasResult) {
      return false;
    }

    //printRelationResultQueue();
    m_relations.pop();
  }

  //std::cout << "\nPattern Queue";
  for (int i = 0; i < patternSize; ++i) {
    Pattern pattern = m_patterns.front();
    /*std::cout << "\n" << i + 1 << ": " << pattern.getStmt().getName() << " ";
    std::cout << pattern.getLeft().getName() << " ";
    std::cout << pattern.getRight().getName() << " ";
    std::cout << pattern.isSubtree() << "\n";*/
    
    bool hasResult = getPatternResultFromPkb(pattern);
    if (!hasResult) {
      return false;
    }

    //printPatternResultQueue();
    m_patterns.pop();
  }

  return true;
}

/**
* A function that stores the result in a data structure.
* @param t_result a vector<string> argument
*/
void QueryEvaluator::storeSelectResultFromPkb(std::vector<std::string> t_result) {
  //printDivider();
  //std::cout << "Storing the select result from PKB to the select result queue...\n";
  m_selectResults.push(t_result);
  //printDivider();
}

/**
* A function that stores the result in a data structure.
* @param t_result an unordered_map<string, vector<string>> argument
* @param t_type an enum queryType argument
*/
void QueryEvaluator::storeResultFromPkb(std::unordered_map<std::string, std::vector<std::string>> t_result, queryType::clauseType t_type) {
  //printDivider();
  //std::cout << "Storing the result from PKB to different queues...\n";

  if (t_type == queryType::RELATION) {
    m_relationResults.push(t_result);
  } else if (t_type == queryType::PATTERN) {
    m_patternResults.push(t_result);
  } else {
    //std::cout << "Result Type: " << t_type << "\n";
  }
  //printDivider();
}

/**
* A function that evaluates the final result of the query by comparing the results based on what the query wants.
* @return The query results
*/
std::vector<std::string> QueryEvaluator::evaluateFinalResult() {
  //printDivider();
  //std::cout << "Evaluating the final result...\n";
  std::vector<std::string> finalResult;
  std::unordered_map<int, queryType::GType> typeOfStmts = m_pkb->getTypeOfStatementTable();

  if (m_relationResults.empty() && m_patternResults.empty()) {
    //std::cout << "CASE 1\n";
    if (!m_selectResults.empty()) {
      finalResult = m_selectResults.front();
    } 
  } else if (!m_relationResults.empty() && m_patternResults.empty()) {
    //std::cout << "CASE 2\n";
    if ((m_relations.front().getG1().getType() == queryType::GType::STMT_NO || m_relations.front().getG1().getType() == queryType::GType::STR) && m_relations.front().getG2().getType() != queryType::GType::STMT_NO && m_relations.front().getG2().getType() != queryType::GType::STR) {
      //std::cout << "STMT_NO/_ SYNONYM\n";
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          if (m_selectedType == queryType::GType::STMT) {
            if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == queryType::GType::ASGN || typeOfStmts[stoi(y)] == queryType::GType::WHILE) {
              finalResult.push_back(y);
            }
          } else if (m_selectedType == queryType::GType::ASGN || m_selectedType == queryType::GType::WHILE) {
            if (typeOfStmts[stoi(y)] == m_selectedType) {
              finalResult.push_back(y);
            }
          } else {
            finalResult.push_back(y);
          }
        }
      }
    } else if (m_relations.front().getG1().getType() != queryType::GType::STMT_NO && m_relations.front().getG1().getType() != queryType::GType::STR && (m_relations.front().getG2().getType() == queryType::GType::STMT_NO || m_relations.front().getG2().getType() == queryType::GType::STR)) {
      //std::cout << "SYNONYM STMT_NO/STR\n";
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          if (m_selectedType == queryType::GType::STMT) {
            if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == queryType::GType::ASGN || typeOfStmts[stoi(y)] == queryType::GType::WHILE) {
              finalResult.push_back(y);
            }
          } else if (m_selectedType == queryType::GType::ASGN || m_selectedType == queryType::GType::WHILE) {
            if (typeOfStmts[stoi(y)] == m_selectedType) {
              finalResult.push_back(y);
            }
          } else {
            finalResult.push_back(y);
          }
        }
      }
    } else if (m_relations.front().getG1().getType() != queryType::GType::STMT_NO && m_relations.front().getG1().getType() != queryType::GType::STR && m_relations.front().getG2().getType() != queryType::GType::STMT_NO && m_relations.front().getG2().getType() != queryType::GType::STR) {
      //std::cout << "SYNONYM SYNONYM\n";
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      
      if (m_relations.front().getG1().getName() == m_selectedSynonym) {
        //std::cout << "Selected Synonym 1: " << m_relations.front().getG1().getName() << "\n";
        if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
          for (auto& x : results) {
            for (auto& y : x.second) {
              if (m_selectedType == queryType::GType::STMT) {
                if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == queryType::GType::ASGN || typeOfStmts[stoi(y)] == queryType::GType::WHILE) {
                  finalResult.push_back(y);
                }
              } else if (m_selectedType == queryType::GType::ASGN || m_selectedType == queryType::GType::WHILE) {
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
              if (m_selectedType == queryType::GType::STMT) {
                if (typeOfStmts[stoi(x.first)] == m_selectedType || typeOfStmts[stoi(x.first)] == queryType::GType::ASGN || typeOfStmts[stoi(x.first)] == queryType::GType::WHILE) {
                  finalResult.push_back(x.first);
                }
              } else if (m_selectedType == queryType::GType::ASGN || m_selectedType == queryType::GType::WHILE) {
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
        //std::cout << "Selected Synonym 2: " << m_relations.front().getG2().getName() << "\n";
        if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
          for (auto& x : results) {
            if (!x.second.empty()) {
              if (m_selectedType == queryType::GType::STMT) {
                if (typeOfStmts[stoi(x.first)] == m_selectedType || typeOfStmts[stoi(x.first)] == queryType::GType::ASGN || typeOfStmts[stoi(x.first)] == queryType::GType::WHILE) {
                  finalResult.push_back(x.first);
                }
              } else if (m_selectedType == queryType::GType::ASGN || m_selectedType == queryType::GType::WHILE) {
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
              if (m_selectedType == queryType::GType::STMT) {
                if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == queryType::GType::ASGN || typeOfStmts[stoi(y)] == queryType::GType::WHILE) {
                  finalResult.push_back(y);
                }
              } else if (m_selectedType == queryType::GType::ASGN || m_selectedType == queryType::GType::WHILE) {
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
    //std::cout << "CASE 3\n";
    if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
      std::unordered_map<std::string, std::vector<std::string>> results = m_patternResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          if (m_selectedType == queryType::GType::STMT) {
            if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == queryType::GType::ASGN || typeOfStmts[stoi(y)] == queryType::GType::WHILE) {
              finalResult.push_back(y);
            }
          } else if (m_selectedType == queryType::GType::ASGN || m_selectedType == queryType::GType::WHILE) {
            if (typeOfStmts[stoi(y)] == m_selectedType) {
              finalResult.push_back(y);
            }
          } else {
            finalResult.push_back(y);
          }
        }
      }
    } else if (m_patterns.front().getLeft().getType() == queryType::GType::VAR) {
      if (m_patterns.front().getLeft().getName() == m_selectedSynonym) {
        std::unordered_map<std::string, std::vector<std::string>> results = m_patternResults.front();
        for (auto& x : results) {
          for (auto& y : x.second) {
            if (m_selectedType == queryType::GType::STMT) {
              if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == queryType::GType::ASGN || typeOfStmts[stoi(y)] == queryType::GType::WHILE) {
                finalResult.push_back(y);
              }
            } else if (m_selectedType == queryType::GType::ASGN || m_selectedType == queryType::GType::WHILE) {
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
          if (m_selectedType == queryType::GType::STMT) {
            if (typeOfStmts[stoi(x.first)] == m_selectedType || typeOfStmts[stoi(x.first)] == queryType::GType::ASGN || typeOfStmts[stoi(x.first)] == queryType::GType::WHILE) {
              finalResult.push_back(x.first);
            }
          } else if (m_selectedType == queryType::GType::ASGN || m_selectedType == queryType::GType::WHILE) {
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
        //std::cout << "CASE 4\n";
        std::vector<std::string> commonResults;
        if ((m_relations.front().getG1().getType() == queryType::GType::STMT_NO || m_relations.front().getG1().getType() == queryType::GType::STR) && m_relations.front().getG2().getType() != queryType::GType::STMT_NO && m_relations.front().getG2().getType() != queryType::GType::STR) {
          if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;
            
            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            } 

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = m_selectResults.front();
            }      
          } else {
            if (m_patterns.front().getLeft().getType() == queryType::GType::VAR) {
              if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
                if (!commonResults.empty()) {
                  finalResult = m_selectResults.front();
                }
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != queryType::GType::STMT_NO && m_relations.front().getG1().getType() != queryType::GType::STR && (m_relations.front().getG2().getType() == queryType::GType::STMT_NO || m_relations.front().getG2().getType() == queryType::GType::STR)) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = m_selectResults.front();
            } 
          } else {
            if (m_patterns.front().getLeft().getType() == queryType::GType::VAR) {
              if (m_relations.front().getG1().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
                if (!commonResults.empty()) {
                  finalResult = m_selectResults.front();
                }
              } 
            }
          }
        } else if (m_relations.front().getG1().getType() != queryType::GType::STMT_NO && m_relations.front().getG1().getType() != queryType::GType::STR && m_relations.front().getG2().getType() != queryType::GType::STMT_NO && m_relations.front().getG2().getType() != queryType::GType::STR) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : relationResults) {
                result1.push_back(x.first);
              }
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }          
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = m_selectResults.front();
            }
          } else if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.push_back(x.first);      
              }
            } else {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = m_selectResults.front();
            }
          } else if (m_patterns.front().getLeft().getType() == queryType::GType::VAR) {
            if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
              std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
              std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
              std::vector<std::string> result1;
              std::vector<std::string> result2;

              if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
                for (auto& x : relationResults) {
                  result1.push_back(x.first);
                }
              } else {
                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }
              }

              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }

              commonResults = getCommonResults(result1, result2);
              if (!commonResults.empty()) {
                finalResult = m_selectResults.front();
              }
            }
          }
        }
      } else if (got->second == 2) { // Common Synonym between Select + Relation or Select + Pattern and Relation + Pattern Clause
        //std::cout << "CASE 5\n";
        std::vector<std::string> commonResults;
        if ((m_relations.front().getG1().getType() == queryType::GType::STMT_NO || m_relations.front().getG1().getType() == queryType::GType::STR) && m_relations.front().getG2().getType() != queryType::GType::STMT_NO && m_relations.front().getG2().getType() != queryType::GType::STR) {
          if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
          } else {
            if (m_patterns.front().getLeft().getType() == queryType::GType::VAR) {
              if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != queryType::GType::STMT_NO && m_relations.front().getG1().getType() != queryType::GType::STR && (m_relations.front().getG2().getType() == queryType::GType::STMT_NO || m_relations.front().getG2().getType() == queryType::GType::STR)) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
          } else {
            if (m_patterns.front().getLeft().getType() == queryType::GType::VAR) {
              if (m_relations.front().getG1().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != queryType::GType::STMT_NO && m_relations.front().getG1().getType() != queryType::GType::STR && m_relations.front().getG2().getType() != queryType::GType::STMT_NO && m_relations.front().getG2().getType() != queryType::GType::STR) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : relationResults) {
                result1.push_back(x.first);
              }
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
          } else if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.push_back(x.first);
              }
            } else {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
          } else if (m_patterns.front().getLeft().getType() == queryType::GType::VAR) {
            if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
              std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
              std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
              std::vector<std::string> result1;
              std::vector<std::string> result2;

              if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
                for (auto& x : relationResults) {
                  result1.push_back(x.first);
                }
              } else {
                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }
              }

              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }

              commonResults = getCommonResults(result1, result2);
            }
          }
        }

        if (!commonResults.empty()) {
          if (m_selectedSynonym == m_relations.front().getG1().getName() && m_relations.front().getG1().getType() == m_selectedType) {
            if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
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
            if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
              for (auto& x : m_relationResults.front()) {
                if (!getCommonResults(x.second, commonResults).empty()) {
                  finalResult.push_back(x.first);
                }
              }
            } else {
              for (auto& x : m_relationResults.front()) {
                if (find(commonResults.begin(), commonResults.end(), x.first) != commonResults.end()) {
                  finalResult.insert(finalResult.end(), x.second.begin(), x.second.end());
                }
              }
            }
          } else if (m_selectedSynonym == m_patterns.front().getStmt().getName() && m_patterns.front().getStmt().getType() == m_selectedType) {
            for (auto& x : m_patternResults.front()) {
              if (!getCommonResults(x.second, commonResults).empty()) {
                finalResult.push_back(x.first);
              }
            }
          } else if (m_selectedSynonym == m_patterns.front().getLeft().getName() && m_patterns.front().getLeft().getType() == m_selectedType) {
            for (auto& x : m_patternResults.front()) {
              if (find(commonResults.begin(), commonResults.end(), x.first) != commonResults.end()) {
                finalResult.insert(finalResult.end(), x.second.begin(), x.second.end());
              }
            }
          }
        }
      } else if (got->second > 2) { // Common Synonym between Select, Relation and Pattern Clause
        //std::cout << "CASE 6\n";
        std::vector<std::string> commonResults;
        if ((m_relations.front().getG1().getType() == queryType::GType::STMT_NO || m_relations.front().getG1().getType() == queryType::GType::STR) && m_relations.front().getG2().getType() != queryType::GType::STMT_NO && m_relations.front().getG2().getType() != queryType::GType::STR) {
          if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = commonResults;
            }
          } else {
            if (m_patterns.front().getLeft().getType() == queryType::GType::VAR) {
              if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
                if (!commonResults.empty()) {
                  finalResult = commonResults;
                }
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != queryType::GType::STMT_NO && m_relations.front().getG1().getType() != queryType::GType::STR && (m_relations.front().getG2().getType() == queryType::GType::STMT_NO || m_relations.front().getG2().getType() == queryType::GType::STR)) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = commonResults;
            }
          } else {
            if (m_patterns.front().getLeft().getType() == queryType::GType::VAR) {
              if (m_relations.front().getG1().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
                if (!commonResults.empty()) {
                  finalResult = commonResults;
                }
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != queryType::GType::STMT_NO && m_relations.front().getG1().getType() != queryType::GType::STR && m_relations.front().getG2().getType() != queryType::GType::STMT_NO && m_relations.front().getG2().getType() != queryType::GType::STR) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : relationResults) {
                result1.push_back(x.first);
              }
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = commonResults;
            }
          } else if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.push_back(x.first);
              }
            } else {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            }

            if (m_patterns.front().getLeft().getType() != queryType::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = commonResults;
            }
          } else if (m_patterns.front().getLeft().getType() == queryType::GType::VAR) {
            if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
              std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
              std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
              std::vector<std::string> result1;
              std::vector<std::string> result2;

              if (m_relations.front().getType() == queryType::RType::USES || m_relations.front().getType() == queryType::RType::MODIFIES) {
                for (auto& x : relationResults) {
                  result1.push_back(x.first);
                }
              } else {
                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }
              }

              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }

              commonResults = getCommonResults(result1, result2);
              if (!commonResults.empty()) {
                finalResult = commonResults;
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

  /*std::cout << "Query Result: \n";
  for (auto& x : finalResult) {
    std::cout << x << ", ";
  }*/

  //printDivider();
  return finalResult;
}