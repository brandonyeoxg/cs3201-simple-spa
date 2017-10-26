#pragma once

#include "QueryEvaluator.h"

/**
* A function that evaluates the query that has been pre-processed by the QueryPreprocessor.
*/
LIST_OF_RESULTS QueryEvaluator::evaluateQuery() {  
  if (isDebugMode) {
    std::cout << "Evaluating Query...\n";
    MAP_OF_STMT_NUM_TO_GTYPE typeOfStmts = m_pkb->getTypeOfStatementTable();
    std::cout << "\nNUMBER OF STMTS: " << typeOfStmts.size() << "\n";
  }

  bool hasResult = getResultFromPkb();
  if (hasResult) {
    return evaluateFinalResult();
  } else if (!hasResult) {
    std::vector<std::string> result;
    m_table->clearTable();
    return result;
  }
}

/**
* A function that gets the result of the clauses by calling the API from PKB.
* @return true if there are results otherwise false
*/
BOOLEAN QueryEvaluator::getResultFromPkb() {
  if (isDebugMode) {
    std::cout << "Getting results from PKB...\n";
  }
  
  int relationSize = m_relations.size();
  int patternSize = m_patterns.size();
  int withSize = m_withs.size();

  Grammar grammar = m_selects.front();
  m_selectedSynonym = grammar.getName();
  m_selectedType = grammar.getType();

  //Loop through the With Queue
  for (int i = 0; i < withSize; ++i) {
    m_isSelectOnly = false;
    With with = m_withs.front();
    BOOLEAN hasResult = getWithResult(with);
    if (!hasResult) {
      return false;
    }

    m_withs.pop();
  }

  //Loop through the Relation Queue
  for (int i = 0; i < relationSize; ++i) {
    m_isSelectOnly = false;
    Relation relation = m_relations.front();
    std::unordered_map<SYNONYM_NAME, Grammar>::const_iterator got;
    got = m_synsToBeRewritten.find(relation.getG1().getName());
    if (got != m_synsToBeRewritten.end()) {
      relation.setG1(got->second);
    }
    got = m_synsToBeRewritten.find(relation.getG2().getName());
    if (got != m_synsToBeRewritten.end()) {
      relation.setG2(got->second);
    }

    BOOLEAN hasResult = getRelationResultFromPkb(relation);
    if (!hasResult) {
      return false;
    }

    m_relations.pop();
  }

  //Loop through the Pattern Queue
  for (int i = 0; i < patternSize; ++i) {
    m_isSelectOnly = false;
    Pattern pattern = m_patterns.front(); 
    BOOLEAN hasResult = getPatternResultFromPkb(pattern);
    if (!hasResult) {
      return false;
    }

    m_patterns.pop();
  }

  return true;
}

BOOLEAN QueryEvaluator::getSelectResultFromPkb(Grammar t_select) {
  std::unordered_map<SYNONYM_NAME, Grammar>::iterator got;
  got = m_synsToBeRewritten.find(t_select.getName());
  if (got != m_synsToBeRewritten.end()) {
    std::vector<std::string> results;
    results.push_back(got->second.getName());
    storeSelectResultFromPkb(results);
    return true;
  }

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
    LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
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

BOOLEAN QueryEvaluator::getRelationResultFromPkb(Relation t_relation) {
  SET_OF_RELATION_RESULTS result;
  MAP_OF_STMT_NUM_TO_GTYPE typeOfStmts = m_pkb->getTypeOfStatementTable();
  Evaluator *eval = Relationship::createEvaluator(t_relation.getType());
  Grammar g1 = t_relation.getG1();
  Grammar g2 = t_relation.getG2();

  // Get the respective evaluators to get the results of the relation clauses
  if (QueryUtil::isAllUnderscores(g1, g2)) {
    BOOLEAN result = eval->hasRelationship(m_pkb, g1, g2);
    delete eval;
    return result;
  } else if (QueryUtil::hasNoSynonyms(g1, g2)) {
    BOOLEAN result = eval->isRelationTrue(m_pkb, g1, g2);
    delete eval;
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

BOOLEAN QueryEvaluator::getPatternResultFromPkb(Pattern t_pattern) {
  SET_OF_PATTERN_RESULTS result;
  PatternEvaluator *eval = Patterns::createEvaluator(t_pattern.getStmt().getType());
  Grammar stmt = t_pattern.getStmt();
  Grammar g1 = t_pattern.getLeft();
  Grammar g2 = t_pattern.getRight();
  BOOLEAN isExact = !t_pattern.isSubtree();

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
void QueryEvaluator::storeSelectResultFromPkb(LIST_OF_SELECT_RESULTS t_result) {
  if (isDebugMode) {
    std::cout << "Storing the select result from PKB to the select result queue...\n";
  }
  
  m_selectResults.push(t_result);
}

BOOLEAN QueryEvaluator::storeRelationResultFromPkb(Relation t_relation, SET_OF_RELATION_RESULTS t_result) {
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

BOOLEAN QueryEvaluator::storePatternResultFromPkb(Pattern t_pattern, SET_OF_PATTERN_RESULTS t_result) {
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
LIST_OF_RESULTS QueryEvaluator::evaluateFinalResult() {
  if (isDebugMode) {
    std::cout << "Evaluating the final result...\n";
  }
  
  LIST_OF_RESULTS finalResult;
  LIST_OF_SYNONYMS selectedSynonyms;

  selectedSynonyms.push_back(m_selectedSynonym);
  if (m_selects.front().getType() == queryType::GType::BOOLEAN) {
    if (m_isSelectOnly) {
      finalResult.push_back("true");
    } else if (m_table->hasSynonyms() && m_table->isEmpty()) {
      finalResult.clear();
    } else {
      finalResult.push_back("true");
    }
  } else {
    finalResult = m_table->getResults(selectedSynonyms);
    if (finalResult.empty()) {
      BOOLEAN hasResult = getSelectResultFromPkb(m_selects.front());
      if (!hasResult) {
        return finalResult;
      } else {
        finalResult = m_selectResults.front();
      }   
    }  
  }

  m_table->clearTable();
  return finalResult;
}

BOOLEAN QueryEvaluator::getWithResult(With t_with) {
  Grammar left = t_with.getG1();
  Grammar right = t_with.getG2();

  if ((Grammar::isStmtNo(left.getType()) || Grammar::isString(left.getType())) && (Grammar::isStmtNo(right.getType()) || Grammar::isString(right.getType()))) {
    //Compare left and right (int = int, str = str)
    return true;
  } else if (Grammar::isProgLine(left.getType()) && Grammar::isProgLine(right.getType())) {
    //Rewrite syn = syn
    //With n = n
    if (left.getName() == right.getName()) {
      return true;
    }

    //With n1 = n2 but no other clauses uses n1 and n2
    if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, left.getName()) && !QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, right.getName())) {
      return true;
    }

    //With n1 = n2 with other clauses using n1 or n2
    if (m_synonymsUsedInQuery[left.getName()] >= m_synonymsUsedInQuery[right.getName()]) {
      m_synsToBeRewritten[right.getName()] = t_with.getG1();
    } else {
      m_synsToBeRewritten[left.getName()] = t_with.getG2();
    }
  } else if ((Grammar::isProgLine(left.getType()) || Grammar::isStmtNo(left.getType())) && (Grammar::isStmtNo(right.getType()) || Grammar::isProgLine(right.getType()))) {
    //Rewrite syn = int, int = syn
    MAP_OF_STMT_NUM_TO_GTYPE allStmts = m_pkb->getTypeOfStatementTable();
    INTEGER totalStmts = allStmts.size();
    
    if (Grammar::isStmtNo(left.getType())) {
      if (std::stoi(left.getName()) > totalStmts) {
        return false;
      }
      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, right.getName())) {
        return true;
      }
      m_synsToBeRewritten[right.getName()] = t_with.getG1();
    } else {
      if (std::stoi(right.getName()) > totalStmts) {
        return false;
      }
      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, left.getName())) {
        return true;
      }
      m_synsToBeRewritten[left.getName()] = t_with.getG2();
    }
  } else if (left.hasAttr() && right.hasAttr()) {
    //Todo: Evaluate attr = attr
    if (left.getName() == right.getName()) {
      return true;
    }

    if (left.getType() == right.getType()) {
      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, left.getName()) && !QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, right.getName())) {
        return true;
      }

      if (Grammar::isConst(left.getType())) {
        //Todo: Select <c1, c2>
      }

      if (m_synonymsUsedInQuery[left.getName()] >= m_synonymsUsedInQuery[right.getName()]) {
        m_synsToBeRewritten[right.getName()] = t_with.getG1();
        return true;
      } else {
        m_synsToBeRewritten[left.getName()] = t_with.getG2();
        return true;
      }
    }

    if (left.getAttr() == right.getAttr()) {
      if (Grammar::isStmtNum(left.getAttr())) {
        if (Grammar::isStmt(left.getType())) {
          m_synsToBeRewritten[left.getName()] = t_with.getG2();
        } else if (Grammar::isStmt(right.getType())) {
          m_synsToBeRewritten[right.getName()] = t_with.getG1();
        } else {
          return false;
        }
      } else if (Grammar::isProcName(left.getAttr())) {
        LIST_OF_PROC_NAMES allProcsCalled = m_pkb->getCalledByAnything();
        SET_OF_RESULTS results;
        for (auto& procName : allProcsCalled) {
          LIST_OF_STMT_NUMS callStmts = m_pkb->getStmtNumsFromProcName(procName);
          if (callStmts.empty()) {
            return false;
          }
          
          results[procName] = Formatter::formatVectorIntToVectorStr(callStmts);
        }

        if (Grammar::isProc(left.getType())) {
          return m_table->insertTwoSynonym(left.getName(), right.getName(), results);
        } else {
          return m_table->insertTwoSynonym(right.getName(), left.getName(), results);
        }
      }
    }

    if (left.getAttr() != right.getAttr()) {
      if ((Grammar::isStmtNum(left.getAttr()) && Grammar::isValue(right.getAttr())) || (Grammar::isStmtNum(right.getAttr()) && Grammar::isValue(left.getAttr()))) {
        if (Grammar::isStmt(left.getType()) || Grammar::isStmt(right.getType())) {
          MAP_OF_STMT_NUM_TO_GTYPE allStmts = m_pkb->getTypeOfStatementTable();
          LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
          SET_OF_RESULTS results = EvaluatorUtil::getCommonProgLineAndConstant(allConstants, allStmts.size());
          if (results.empty()) {
            return false;
          }

          return m_table->insertTwoSynonym(left.getName(), right.getName(), results);
        } else if (Grammar::isAssign(left.getType()) || Grammar::isAssign(right.getType())) {
          LIST_OF_STMT_NUMS allAssignStmts = m_pkb->getAllAssignStmts();
          LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
          LIST_OF_RESULTS allAssignStmtsInStr = Formatter::formatVectorIntToVectorStr(allAssignStmts);
          LIST_OF_RESULTS commonResults = EvaluatorUtil::getCommonResults(allConstants, allAssignStmtsInStr);
          if (commonResults.empty()) {
            return false;
          }

          SET_OF_RESULTS results = Formatter::formatVectorStrToMapStrVectorStr(commonResults);
          return m_table->insertTwoSynonym(left.getName(), right.getName(), results);
        } else if (Grammar::isWhile(left.getType()) || Grammar::isWhile(right.getType())) {
          LIST_OF_STMT_NUMS allWhileStmts = m_pkb->getAllWhileStmts();
          LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
          LIST_OF_RESULTS allWhileStmtsInStr = Formatter::formatVectorIntToVectorStr(allWhileStmts);
          LIST_OF_RESULTS commonResults = EvaluatorUtil::getCommonResults(allConstants, allWhileStmtsInStr);
          if (commonResults.empty()) {
            return false;
          }

          SET_OF_RESULTS results = Formatter::formatVectorStrToMapStrVectorStr(commonResults);
          return m_table->insertTwoSynonym(left.getName(), right.getName(), results);
        } else if (Grammar::isIf(left.getType()) || Grammar::isIf(right.getType())) {
          LIST_OF_STMT_NUMS allIfStmts = m_pkb->getAllIfStmts();
          LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
          LIST_OF_RESULTS allIfStmtsInStr = Formatter::formatVectorIntToVectorStr(allIfStmts);
          LIST_OF_RESULTS commonResults = EvaluatorUtil::getCommonResults(allConstants, allIfStmtsInStr);
          if (commonResults.empty()) {
            return false;
          }

          SET_OF_RESULTS results = Formatter::formatVectorStrToMapStrVectorStr(commonResults);
          return m_table->insertTwoSynonym(left.getName(), right.getName(), results);
        } else if (Grammar::isCall(left.getType()) || Grammar::isCall(right.getType())) {
          MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS allStmts = m_pkb->getStatementTypeTable();
          LIST_OF_STMT_NUMS allCallStmts = allStmts[queryType::GType::CALL];
          LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
          LIST_OF_RESULTS allCallStmtsInStr = Formatter::formatVectorIntToVectorStr(allCallStmts);
          LIST_OF_RESULTS commonResults = EvaluatorUtil::getCommonResults(allConstants, allCallStmtsInStr);
          if (commonResults.empty()) {
            return false;
          }

          SET_OF_RESULTS results = Formatter::formatVectorStrToMapStrVectorStr(commonResults);
          return m_table->insertTwoSynonym(left.getName(), right.getName(), results);
        }
      } else if ((Grammar::isProcName(left.getAttr()) && Grammar::isVarName(right.getAttr())) || (Grammar::isProcName(right.getAttr()) && Grammar::isVarName(left.getAttr()))) {
        if (Grammar::isProc(left.getType()) || Grammar::isProc(right.getType())) {
          LIST_OF_PROC_NAMES allProcNames = m_pkb->getAllProcsName();
          LIST_OF_VAR_NAMES allVarNames = m_pkb->getAllVarNames();
          LIST_OF_RESULTS commonNames = EvaluatorUtil::getCommonResults(allProcNames, allVarNames);
          if (commonNames.empty()) {
            return false;
          }

          SET_OF_RESULTS results = Formatter::formatVectorStrToMapStrVectorStr(commonNames);
          return m_table->insertTwoSynonym(left.getName(), right.getName(), results);
        } else if (Grammar::isCall(left.getType()) || Grammar::isCall(right.getType())) {
          LIST_OF_PROC_NAMES allProcsCalled = m_pkb->getCalledByAnything();
          LIST_OF_VAR_NAMES allVarNames = m_pkb->getAllVarNames();
          LIST_OF_RESULTS commonNames = EvaluatorUtil::getCommonResults(allProcsCalled, allVarNames);
          SET_OF_RESULTS results;
          for (auto& procName : commonNames) {
            LIST_OF_STMT_NUMS callStmts = m_pkb->getStmtNumsFromProcName(procName);
            if (callStmts.empty()) {
              return false;
            }

            results[procName] = Formatter::formatVectorIntToVectorStr(callStmts);
          }

          if (Grammar::isVar(left.getType())) {
            return m_table->insertTwoSynonym(left.getName(), right.getName(), results);
          } else {
            return m_table->insertTwoSynonym(right.getName(), left.getName(), results);
          }
        }
      } else {
        return false;
      }
    }
  } else if ((left.hasAttr() || Grammar::isProgLine(left.getType())) 
    && (Grammar::isProgLine(right.getType()) || right.hasAttr())) {
    //Evaluate attr = syn, syn = attr
    if (Grammar::isValue(left.getAttr()) || Grammar::isValue(right.getAttr())) {   
      MAP_OF_STMT_NUM_TO_GTYPE allStmts = m_pkb->getTypeOfStatementTable();
      LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
      SET_OF_RESULTS results = EvaluatorUtil::getCommonProgLineAndConstant(allConstants, allStmts.size());
      if (results.empty()) {
        return false;
      }

      return m_table->insertTwoSynonym(left.getName(), right.getName(), results);
    } else if (Grammar::isStmtNum(left.getAttr()) || Grammar::isStmtNum(right.getAttr())) {
      if (Grammar::isStmt(left.getType()) || Grammar::isStmt(right.getType())) {
        return true;
      } else if (Grammar::isAssign(left.getType())) {
        m_synsToBeRewritten[right.getName()] = t_with.getG1();
      } else if (Grammar::isAssign(right.getType())) {
        m_synsToBeRewritten[left.getName()] = t_with.getG2();
      } else if (Grammar::isWhile(left.getType())) {
        m_synsToBeRewritten[right.getName()] = t_with.getG1();
      } else if (Grammar::isWhile(right.getType())) {
        m_synsToBeRewritten[left.getName()] = t_with.getG2();
      } else if (Grammar::isIf(left.getType())) {
        m_synsToBeRewritten[right.getName()] = t_with.getG1();
      } else if (Grammar::isIf(right.getType())) {
        m_synsToBeRewritten[left.getName()] = t_with.getG2();
      } else if (Grammar::isCall(left.getType())) {
        m_synsToBeRewritten[right.getName()] = t_with.getG1();
      } else if (Grammar::isCall(right.getType())) {
        m_synsToBeRewritten[left.getName()] = t_with.getG2();
      }
    } else {
      return false;
    }
  } else if ((left.hasAttr() || Grammar::isString(left.getType())) 
    && (Grammar::isString(right.getType()) || right.hasAttr())) {
    //Rewrite attr = str, str = attr
    if (Grammar::isCall(left.getType())) {
      LIST_OF_STMT_NUMS callStmts = m_pkb->getStmtNumsFromProcName(right.getName());
      if (callStmts.empty()) {
        return false;
      }
      return m_table->insertOneSynonym(left.getName(), Formatter::formatVectorIntToVectorStr(callStmts));
    } else if (Grammar::isCall(right.getType())) {
      LIST_OF_STMT_NUMS callStmts = m_pkb->getStmtNumsFromProcName(left.getName());
      if (callStmts.empty()) {
        return false;
      }
      return m_table->insertOneSynonym(right.getName(), Formatter::formatVectorIntToVectorStr(callStmts));
    }

    if (Grammar::isString(left.getType())) {
      if (Grammar::isProc(right.getType())) {
        LIST_OF_RESULTS allProcNames = m_pkb->getAllProcsName();
        if (std::find(allProcNames.begin(), allProcNames.end(), left.getName()) == allProcNames.end()) {
          return false;
        }
      } else if (Grammar::isVar(right.getType())) {
        LIST_OF_RESULTS allVarNames = m_pkb->getAllVarNames();
        if (std::find(allVarNames.begin(), allVarNames.end(), left.getName()) == allVarNames.end()) {
          return false;
        }
      }

      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, right.getName())) {
        return true;
      }
      m_synsToBeRewritten[right.getName()] = t_with.getG1();
    } else {
      if (Grammar::isProc(left.getType())) {
        LIST_OF_RESULTS allProcNames = m_pkb->getAllProcsName();
        if (std::find(allProcNames.begin(), allProcNames.end(), right.getName()) == allProcNames.end()) {
          return false;
        }
      } else if (Grammar::isVar(left.getType())) {
        LIST_OF_RESULTS allVarNames = m_pkb->getAllVarNames();
        if (std::find(allVarNames.begin(), allVarNames.end(), right.getName()) == allVarNames.end()) {
          return false;
        }
      }

      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, left.getName())) {
        return true;
      }
      m_synsToBeRewritten[left.getName()] = t_with.getG2();
    }
  } else if ((left.hasAttr() || Grammar::isStmtNo(left.getType())) 
    && (Grammar::isStmtNo(right.getType()) || right.hasAttr())) {
    //Rewrite attr = int, int = attr
    if (Grammar::isValue(left.getAttr())) {
      LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
      if (std::find(allConstants.begin(), allConstants.end(), right.getName()) != allConstants.end()) {
        LIST_OF_RESULTS results;
        results.push_back(right.getName());
        return m_table->insertOneSynonym(left.getName(), results);
      } else {
        return false;
      }
    } else if (Grammar::isValue(right.getAttr())) {
      LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
      if (std::find(allConstants.begin(), allConstants.end(), left.getName()) != allConstants.end()) {
        LIST_OF_RESULTS results;
        results.push_back(left.getName());
        return m_table->insertOneSynonym(right.getName(), results);
      } else {
        return false;
      }
    }

    if (Grammar::isStmtNum(left.getAttr())) {
      MAP_OF_STMT_NUM_TO_GTYPE allStmts = m_pkb->getTypeOfStatementTable();
      int maxStmtNo = allStmts.size();
      if (std::stoi(right.getName()) > maxStmtNo) {
        return false;
      }

      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, left.getName())) {
        return true;
      }

      m_synsToBeRewritten[left.getName()] = t_with.getG2();
    } else if (Grammar::isStmtNum(right.getAttr())) {
      MAP_OF_STMT_NUM_TO_GTYPE allStmts = m_pkb->getTypeOfStatementTable();
      int maxStmtNo = allStmts.size();
      if (std::stoi(right.getName()) > maxStmtNo) {
        return false;
      }

      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, right.getName())) {
        return true;
      }

      m_synsToBeRewritten[right.getName()] = t_with.getG1();
    } else {
      return false;
    }
  } else {
    return false;
  }

  return true;
}