#pragma once

#include "QueryEvaluator.h"

/**
* A function that evaluates the query that has been pre-processed by the QueryPreprocessor.
*/
std::vector<std::string> QueryEvaluator::evaluateQuery() {
  //printDivider();
  //std::cout << "Evaluating Query...\n";
  bool hasResult = getResultFromPkb();
  if (hasResult) {
    return evaluateFinalResult();
  } else if (!hasResult) {
    std::vector<std::string> result;
    if (m_selects.front().getType() == queryType::GType::BOOLEAN) {
      result.push_back("false");
    }
    m_table->clearTable();
    return result;
  }
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
  
  //Loop through the Select Queue
  for (int i = 0; i < selectSize; ++i) {
    Grammar grammar = m_selects.front();
    m_selectedSynonym = grammar.getName();
    m_selectedType = grammar.getType();
    bool hasResult = getSelectResultFromPkb(grammar);
    if (!hasResult) {
      return false;
    }

    //Move the item to the back of the queue.
    m_selects.pop();
    m_selects.push(grammar);
  }

  //Loop through the Relation Queue
  for (int i = 0; i < relationSize; ++i) {
    m_isSelectOnly = false;
    Relation relation = m_relations.front();
    bool hasResult = getRelationResultFromPkb(relation);
    if (!hasResult) {
      return false;
    }

    m_relations.pop();
  }

  //Loop through the Pattern Queue
  for (int i = 0; i < patternSize; ++i) {
    m_isSelectOnly = false;
    Pattern pattern = m_patterns.front(); 
    bool hasResult = getPatternResultFromPkb(pattern);
    if (!hasResult) {
      return false;
    }

    m_patterns.pop();
  }

  return true;
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
    std::vector<std::string> allSelectedStmts = Formatter::formatVectorIntToVectorStr(allSelectedStmtsInInt);

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
    std::vector<std::string> allConstants = Formatter::formatListStrToVectorStr(constantsList);
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

    std::vector<std::string> allStmtList = Formatter::formatVectorIntToVectorStr(allStmtLst);
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
  if (QueryUtil::isAllUnderscores(g1, g2)) {
    bool result = eval->hasRelationship(m_pkb, g1, g2);
    return result;
  } else if (QueryUtil::hasNoSynonyms(g1, g2)) {
    bool result = eval->isRelationTrue(m_pkb, g1, g2);
    return result;
  } else if (QueryUtil::hasOneRightSynonym(g1, g2)) {
    result = eval->evaluateRightSynonym(m_pkb, g1, g2);
  } else if (QueryUtil::hasOneLeftSynonym(g1, g2)) {
    result = eval->evaluateLeftSynonym(m_pkb, g1, g2);
  } else if (QueryUtil::hasTwoSynonyms(g1, g2)) {
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
  if (QueryUtil::isAnythingWithAnyPattern(g1, g2)) {
    result = eval->getAllStmtsWithAnyPattern(m_pkb, stmt, g1, g2);
  } else if (QueryUtil::isAnythingWithExactPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsWithExactPattern(m_pkb, stmt, g1, g2);
  } else if (QueryUtil::isAnythingWithSubPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsWithSubPattern(m_pkb, stmt, g1, g2);
  } else if (QueryUtil::isVarWithAnyPattern(g1, g2)) {
    result = eval->getAllStmtsWithVarAndAnyPattern(m_pkb, stmt, g1, g2);
  } else if (QueryUtil::isSynonymWithAnyPattern(g1, g2)) {
    result = eval->getAllStmtsAndVarWithAnyPattern(m_pkb, stmt, g1, g2);
  } else if (QueryUtil::isVarWithExactPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsWithVarAndExactPattern(m_pkb, stmt, g1, g2);
  } else if (QueryUtil::isVarWithSubPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsWithVarAndSubPattern(m_pkb, stmt, g1, g2);
  } else if (QueryUtil::isSynonymWithExactPattern(g1, g2, isExact)) {
    result = eval->getAllStmtsAndVarWithExactPattern(m_pkb, stmt, g1, g2);
  } else if (QueryUtil::isSynonymWithSubPattern(g1, g2, isExact)) {
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

  //printDivider();
  m_table->clearTable();
  return finalResult;
}

std::vector<std::string> QueryEvaluator::getCommonResults(std::vector<std::string> t_resultVector1, std::vector<std::string> t_resultVector2) {
  std::vector<std::string> commonResultVector;

  sort(t_resultVector1.begin(), t_resultVector1.end());
  sort(t_resultVector2.begin(), t_resultVector2.end());

  set_intersection(t_resultVector1.begin(), t_resultVector1.end(), t_resultVector2.begin(), t_resultVector2.end(), back_inserter(commonResultVector));

  return commonResultVector;
}