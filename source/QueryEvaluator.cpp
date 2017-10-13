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
    if (m_selects.front().getType() == queryType::GType::BOOLEAN) {
      result.push_back("false");
    }
    /*printDivider();
    std::cout << "No Query Result: \n";
    for (auto& x : result) {
      std::cout << x << ", ";
    }
    printDivider();*/
    m_table->clearTable();
    return result;
  }
}

std::vector<std::string> QueryEvaluator::filterKeyResults(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::unordered_map<std::string, std::vector<std::string>> t_results) {
  std::vector<std::string> stmtVector;

  for (auto& x : t_results) {
    if (!x.second.empty()) {
      if (Grammar::isStmt(m_selectedType) || Grammar::isProgLine(m_selectedType)) {
        if (t_typeOfStmts[std::stoi(x.first)] == m_selectedType || Grammar::isAssign(t_typeOfStmts[std::stoi(x.first)]) || Grammar::isWhile(t_typeOfStmts[std::stoi(x.first)]) || Grammar::isIf(t_typeOfStmts[std::stoi(x.first)]) || Grammar::isCall(t_typeOfStmts[std::stoi(x.first)])) {
          stmtVector.push_back(x.first);
        }
      } else if (Grammar::isAssign(m_selectedType) || Grammar::isWhile(m_selectedType) || Grammar::isIf(m_selectedType) || Grammar::isCall(m_selectedType)) {
        if (t_typeOfStmts[std::stoi(x.first)] == m_selectedType) {
          stmtVector.push_back(x.first);
        }
      } else {
        stmtVector.push_back(x.first);
      }
    }
  }

  return stmtVector;
}

std::vector<std::string> QueryEvaluator::filterValueResults(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::unordered_map <std::string, std::vector<std::string>> t_results) {
  std::vector<std::string> stmtVector;

  for (auto& x : t_results) {
    for (auto& stmtNo : x.second) {
      if (Grammar::isStmt(m_selectedType) || Grammar::isProgLine(m_selectedType)) {
        if (t_typeOfStmts[std::stoi(stmtNo)] == m_selectedType || Grammar::isAssign(t_typeOfStmts[std::stoi(stmtNo)]) || Grammar::isWhile(t_typeOfStmts[std::stoi(stmtNo)]) || Grammar::isIf(t_typeOfStmts[std::stoi(stmtNo)]) || Grammar::isCall(t_typeOfStmts[std::stoi(stmtNo)])) {
          stmtVector.push_back(stmtNo);
        }
      } else if (Grammar::isAssign(m_selectedType) || Grammar::isWhile(m_selectedType) || Grammar::isIf(m_selectedType) || Grammar::isCall(m_selectedType)) {
        if (t_typeOfStmts[std::stoi(stmtNo)] == m_selectedType) {
          stmtVector.push_back(stmtNo);
        }
      } else {
        stmtVector.push_back(stmtNo);
      }
    }
  }

  return stmtVector;
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

bool QueryEvaluator::isAnythingWithAnyPattern(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() == "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() == "_";
}

bool QueryEvaluator::isAnythingWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() == "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && t_isExact;
}

bool QueryEvaluator::isAnythingWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() == "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && !t_isExact;
}

bool QueryEvaluator::isVarWithAnyPattern(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() != "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() == "_";
}

bool QueryEvaluator::isVarWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() != "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && t_isExact;
}

bool QueryEvaluator::isVarWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::STR && t_g1.getName() != "_" && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && !t_isExact;
}

bool QueryEvaluator::isSynonymWithAnyPattern(Grammar t_g1, Grammar t_g2) {
  return t_g1.getType() == queryType::GType::VAR && t_g2.getType() == queryType::GType::STR && t_g2.getName() == "_";
}

bool QueryEvaluator::isSynonymWithExactPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::VAR && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && t_isExact;
}

bool QueryEvaluator::isSynonymWithSubPattern(Grammar t_g1, Grammar t_g2, bool t_isExact) {
  return t_g1.getType() == queryType::GType::VAR && t_g2.getType() == queryType::GType::STR && t_g2.getName() != "_" && !t_isExact;
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
    std::vector<std::string> allVariables = m_pkb->getAllVarNames();
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
  return storeRelationResultFromPkb(t_relation, result);
}

bool QueryEvaluator::getPatternResultFromPkb(Pattern t_pattern) {
  std::unordered_map<std::string, std::vector<std::string>> result;
  PatternEvaluator *eval = Patterns::createEvaluator(t_pattern.getStmt().getType());
  Grammar stmt = t_pattern.getStmt();
  Grammar g1 = t_pattern.getLeft();
  Grammar g2 = t_pattern.getRight();
  bool isExact = !t_pattern.isSubtree();

  // Get the respective evaluators to get the results of the pattern clauses
  if (isAnythingWithAnyPattern(g1, g2)) {
    result = eval->getAllStmtsWithAnyPattern(m_pkb, stmt, g1, g2);
  } else if (isAnythingWithExactPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsWithExactPattern(m_pkb, stmt, g1, g2);
  } else if (isAnythingWithSubPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsWithSubPattern(m_pkb, stmt, g1, g2);
  } else if (isVarWithAnyPattern(g1, g2)) {
    result = eval->getAllStmtsWithVarAndAnyPattern(m_pkb, stmt, g1, g2);
  } else if (isSynonymWithAnyPattern(g1, g2)) {
    result = eval->getAllStmtsAndVarWithAnyPattern(m_pkb, stmt, g1, g2);
  } else if (isVarWithExactPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsWithVarAndExactPattern(m_pkb, stmt, g1, g2);
  } else if (isVarWithSubPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsWithVarAndSubPattern(m_pkb, stmt, g1, g2);
  } else if (isSynonymWithExactPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsAndVarWithExactPattern(m_pkb, stmt, g1, g2);
  } else if (isSynonymWithSubPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsAndVarWithSubPattern(m_pkb, stmt, g1, g2);
  }

  delete eval;
  if (result.empty()) {
    return false;
  }

  // Store the result
  return storePatternResultFromPkb(t_pattern, result);
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
    m_isSelectOnly = false;
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
    m_isSelectOnly = false;
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

bool QueryEvaluator::storeRelationResultFromPkb(Relation t_relation, std::unordered_map<std::string, std::vector<std::string>> t_result) {
  std::unordered_map<std::string, int>::const_iterator got;
  if ((t_relation.getG1().getType() != queryType::GType::STMT_NO || t_relation.getG1().getType() != queryType::GType::STR) && (t_relation.getG2().getType() == queryType::GType::STMT_NO || t_relation.getG2().getType() == queryType::GType::STR)) {
    got = m_synonymsUsedInQuery.find(t_relation.getG1().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {   
        m_relations.push(t_relation);
        return m_table->insertOneSynonym(t_relation.getG1().getName(), t_result[t_relation.getG1().getName()]);
      }
    }
  } else if ((t_relation.getG1().getType() == queryType::GType::STMT_NO || t_relation.getG1().getType() == queryType::GType::STR) && t_relation.getG2().getType() != queryType::GType::STMT_NO && t_relation.getG2().getType() != queryType::GType::STR) {
    got = m_synonymsUsedInQuery.find(t_relation.getG2().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        m_relations.push(t_relation);
        return m_table->insertOneSynonym(t_relation.getG2().getName(), t_result[t_relation.getG2().getName()]);
      }
    }
  } else if (t_relation.getG1().getType() != queryType::GType::STMT_NO && t_relation.getG1().getType() != queryType::GType::STR && t_relation.getG2().getType() != queryType::GType::STMT_NO && t_relation.getG2().getType() != queryType::GType::STR) {
    got = m_synonymsUsedInQuery.find(t_relation.getG1().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        m_relations.push(t_relation);
        if ((Relation::isUses(t_relation.getType()) || Relation::isModifies(t_relation.getType())) && !Grammar::isProc(t_relation.getG1().getType())) {
          return m_table->insertTwoSynonym(t_relation.getG2().getName(), t_relation.getG1().getName(), t_result);
        } else {
          return m_table->insertTwoSynonym(t_relation.getG1().getName(), t_relation.getG2().getName(), t_result);
        }
      } else {
        got = m_synonymsUsedInQuery.find(t_relation.getG2().getName());
        if (got != m_synonymsUsedInQuery.end()) {
          if (got->second > 1) {
            m_relations.push(t_relation);
            if ((Relation::isUses(t_relation.getType()) || Relation::isModifies(t_relation.getType())) && !Grammar::isProc(t_relation.getG1().getType())) {
              return m_table->insertTwoSynonym(t_relation.getG2().getName(), t_relation.getG1().getName(), t_result);
            } else {
              return m_table->insertTwoSynonym(t_relation.getG1().getName(), t_relation.getG2().getName(), t_result);
            }        
          }
        }
      }
    }
  }

  return true;
}

bool QueryEvaluator::storePatternResultFromPkb(Pattern t_pattern, std::unordered_map<std::string, std::vector<std::string>> t_result) {
  std::unordered_map<std::string, int>::const_iterator got;
  if (t_pattern.getLeft().getType() != queryType::GType::VAR) {
    got = m_synonymsUsedInQuery.find(t_pattern.getStmt().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        m_patterns.push(t_pattern);
        return m_table->insertOneSynonym(t_pattern.getStmt().getName(), t_result[t_pattern.getStmt().getName()]);       
      }
    }
  } else if (t_pattern.getLeft().getType() == queryType::GType::VAR) {
    got = m_synonymsUsedInQuery.find(t_pattern.getStmt().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        m_patterns.push(t_pattern);
        return m_table->insertTwoSynonym(t_pattern.getStmt().getName(), t_pattern.getLeft().getName(), t_result);        
      } else if (got->second == 1) {
        got = m_synonymsUsedInQuery.find(t_pattern.getLeft().getName());
        if (got != m_synonymsUsedInQuery.end()) {
          if (got->second > 1) {
            m_patterns.push(t_pattern);
            return m_table->insertTwoSynonym(t_pattern.getStmt().getName(), t_pattern.getLeft().getName(), t_result);        
          }
        }
      }
    }
  }

  return true;
}

/**
* A function that evaluates the final result of the query by comparing the results based on what the query wants.
* @return The query results
*/
std::vector<std::string> QueryEvaluator::evaluateFinalResult() {
  //printDivider();
  //std::cout << "Evaluating the final result...\n";
  LIST_OF_RESULTS finalResult;
  LIST_OF_SYNONYMS selectedSynonyms;

  selectedSynonyms.push_back(m_selectedSynonym);
  if (m_selects.front().getType() == queryType::GType::BOOLEAN) {
    if (m_isSelectOnly) {
      finalResult.push_back("true");
    } else if (m_table->hasSynonyms() && m_table->isEmpty()) {
      finalResult.push_back("false");
    } else {
      finalResult.push_back("true");
    }
  } else {
    finalResult = m_table->getResults(selectedSynonyms);
    if (finalResult.empty()) {
      finalResult = m_selectResults.front();
    }  
  }

  /*std::cout << "Query Result: \n";
  for (auto& x : finalResult) {
    std::cout << x << ", ";
  }*/

  //printDivider();
  m_table->clearTable();
  return finalResult;
}