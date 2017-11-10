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

  QueryOptimiser opt = QueryOptimiser(m_selects, m_relations, m_patterns, m_withs);
  opt.divideClausesIntoGroups(m_noSyns, m_withSyns);

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

  BOOLEAN hasResult;
  int numOfClauses = m_noSyns.size();
  for (int i = 0; i < numOfClauses; ++i) {
    m_isSelectOnly = false;
    Clause *clause = m_noSyns.top();
    if (clause->getClauseType() == queryType::clauseType::WITH) {
      With *with = (With*)clause;
      hasResult = getWithResult(with, 0);
    } else if (clause->getClauseType() == queryType::clauseType::RELATION) {
      Relation *relation = (Relation*)clause;

      if (!Grammar::isStmtNo(relation->getG1().getType()) && !Grammar::isString(relation->getG1().getType())) {
        relation->setG1(EvaluatorUtil::rewriteSynonym(relation->getG1(), m_synsToBeRewritten));
      }

      if (!Grammar::isStmtNo(relation->getG2().getType()) && !Grammar::isString(relation->getG2().getType())) {
        relation->setG2(EvaluatorUtil::rewriteSynonym(relation->getG2(), m_synsToBeRewritten));
      }

      hasResult = getRelationResultFromPkb(relation, 0);
    } else if (clause->getClauseType() == queryType::clauseType::PATTERN) {
      Pattern *pattern = (Pattern*)clause;

      if (!Grammar::isStmtNo(pattern->getLeft().getType()) && !Grammar::isString(pattern->getLeft().getType())) {
        pattern->setLeft(EvaluatorUtil::rewriteSynonym(pattern->getLeft(), m_synsToBeRewritten));
      }

      hasResult = getPatternResultFromPkb(pattern, 0);
    }

    if (!hasResult) {
      return false;
    }

    m_noSyns.pop();
  }

  INTEGER tableIdx;
  if (m_tables.empty()) {
    tableIdx = 0;
  } else {
    tableIdx = 1;
  }

  int numOfGroups = m_withSyns.size();
  for (int i = 0; i < numOfGroups; ++i) {
    m_isSelectOnly = false;
    std::pair<std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses>*, int> group = m_withSyns.top();
    numOfClauses = group.first->size();
    m_tables.push_back(new IntermediateTable());
    for (int j = 0; j < numOfClauses; ++j) {
      Clause *clause = group.first->top();
      if (clause->getClauseType() == queryType::clauseType::WITH) {
        With *with = (With*)clause;
        hasResult = getWithResult(with, tableIdx+i);
      } else if (clause->getClauseType() == queryType::clauseType::RELATION) {
        Relation *relation = (Relation*)clause;

        if (!Grammar::isStmtNo(relation->getG1().getType()) && !Grammar::isString(relation->getG1().getType())) {
          relation->setG1(EvaluatorUtil::rewriteSynonym(relation->getG1(), m_synsToBeRewritten));
        }

        if (!Grammar::isStmtNo(relation->getG2().getType()) && !Grammar::isString(relation->getG2().getType())) {
          relation->setG2(EvaluatorUtil::rewriteSynonym(relation->getG2(), m_synsToBeRewritten));
        }

        hasResult = getRelationResultFromPkb(relation, tableIdx+i);
      } else if (clause->getClauseType() == queryType::clauseType::PATTERN) {
        Pattern *pattern = (Pattern*)clause;

        if (!Grammar::isStmtNo(pattern->getLeft().getType()) && !Grammar::isString(pattern->getLeft().getType())) {
          pattern->setLeft(EvaluatorUtil::rewriteSynonym(pattern->getLeft(), m_synsToBeRewritten));
        }

        hasResult = getPatternResultFromPkb(pattern, tableIdx+i);
      }

      if (!hasResult) {
        return false;
      }

      group.first->pop();
    }

    m_withSyns.pop();
  }

  return true;
}

BOOLEAN QueryEvaluator::getSelectResultFromPkb(Grammar t_select) {
  if (Grammar::isStmtNo(t_select.getType()) || Grammar::isString(t_select.getType())) {
    return true;
  }

  if (Grammar::isCall(t_select.getType()) && t_select.hasAttr()
    && Grammar::isProcName(t_select.getAttr())) {
    std::unordered_map<SYNONYM_NAME, Grammar>::iterator got;
    got = m_synsToBeRewritten.find(t_select.getName());
    if (got != m_synsToBeRewritten.end()) {
      LIST_OF_RESULTS_INDICES results;
      PROC_INDEX procIdx = m_pkb->getProcIdxFromName(got->second.getName());
      results.push_back(procIdx);
      return storeSelectResultFromPkb(t_select, results);
    }
  }

  if (!Grammar::isProc(t_select.getType()) && !Grammar::isStmtLst(t_select.getType()) 
    && !Grammar::isVar(t_select.getType()) && !Grammar::isConst(t_select.getType())) {
    // Call the PKB API getStatementTypeTable().
    std::unordered_map<queryType::GType, std::vector<int>> allStmts = m_pkb->getStatementTypeTable();

    // Check if there are results else return false.
    if (allStmts.empty()) {
      return false;
    }

    // Get all the statements of the type of statements that has been selected.
    std::vector<int> allSelectedStmtsInInt;
    if (Grammar::isStmt(t_select.getType()) || Grammar::isProgLine(t_select.getType())) {
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[queryType::GType::ASGN].begin(), allStmts[queryType::GType::ASGN].end());
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[queryType::GType::WHILE].begin(), allStmts[queryType::GType::WHILE].end());
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[queryType::GType::IF].begin(), allStmts[queryType::GType::IF].end());
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[queryType::GType::CALL].begin(), allStmts[queryType::GType::CALL].end());
    } else if (Grammar::isAssign(t_select.getType())) {
      allSelectedStmtsInInt = allStmts[queryType::GType::ASGN];
    } else if (Grammar::isWhile(t_select.getType())) {
      allSelectedStmtsInInt = allStmts[queryType::GType::WHILE];
    } else if (Grammar::isIf(t_select.getType())) {
      allSelectedStmtsInInt = allStmts[queryType::GType::IF];
    } else if (Grammar::isCall(t_select.getType())) {  
      allSelectedStmtsInInt = allStmts[queryType::GType::CALL];  
    }

    // Change from vector<int> to vector<string>.
    /*std::vector<std::string> allSelectedStmts = Formatter::formatVectorIntToVectorStr(allSelectedStmtsInInt);*/

    // Push into the selectResults queue.
    return storeSelectResultFromPkb(t_select, allSelectedStmtsInInt);
  } else if (Grammar::isVar(t_select.getType())) {
    LIST_OF_VAR_INDICES allVariables = m_pkb->getAllVarIndices();
    if (allVariables.empty()) {
      return false;
    }

    return storeSelectResultFromPkb(t_select, allVariables);
  } else if (Grammar::isConst(t_select.getType())) {
    LIST_OF_CONSTANT_INDICES allConstants = m_pkb->getAllConstantsByIdx();
    if (allConstants.empty()) {
      return false;
    }

    return storeSelectResultFromPkb(t_select, allConstants);
  } else if (Grammar::isProc(t_select.getType())) {
    LIST_OF_PROC_INDICES allProcedures = m_pkb->getAllProcsIndices();
    if (allProcedures.empty()) {
      return false;
    }

    return storeSelectResultFromPkb(t_select, allProcedures);
  } else if (Grammar::isStmtLst(t_select.getType())) {
    LIST_OF_STMT_NUMS allStmtLst = m_pkb->getStmtList();
    if (allStmtLst.empty()) {
      return false;
    }

    //std::vector<std::string> allStmtList = Formatter::formatVectorIntToVectorStr(allStmtLst);
    return storeSelectResultFromPkb(t_select, allStmtLst);
  }

  return false;
}

BOOLEAN QueryEvaluator::getRelationResultFromPkb(Relation *t_relation, INTEGER t_tableIdx) {
  SET_OF_RESULTS_INDICES result;
  MAP_OF_STMT_NUM_TO_GTYPE typeOfStmts = m_pkb->getTypeOfStatementTable();
  Evaluator *eval = Relationship::createEvaluator(t_relation->getType());
  Grammar g1 = t_relation->getG1();
  Grammar g2 = t_relation->getG2();

  /*if (m_cache->isCacheable(t_relation)) {
    SET_OF_RESULTS *cachedResults = m_cache->getCache(t_relation);
    if (cachedResults) {
      return storeRelationResultFromPkb(t_relation, *cachedResults, t_tableIdx);
    }
  }*/

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

  /*if (m_cache->isCacheable(t_relation)) {
    m_cache->cache(t_relation, result);
  }*/

  // Store the result
  return storeRelationResultFromPkb(t_relation, result, t_tableIdx);
}

BOOLEAN QueryEvaluator::getPatternResultFromPkb(Pattern *t_pattern, INTEGER t_tableIdx) {
  SET_OF_RESULTS_INDICES result;
  PatternEvaluator *eval = Patterns::createEvaluator(t_pattern->getStmt().getType());
  Grammar stmt = t_pattern->getStmt();
  Grammar g1 = t_pattern->getLeft();
  Grammar g2 = t_pattern->getRight();
  BOOLEAN isExact = !t_pattern->isSubtree();

  /*if (m_cache->isCacheable(t_pattern)) {
    SET_OF_RESULTS *cachedResults = m_cache->getCache(t_pattern);
    if (cachedResults) {
      return storePatternResultFromPkb(t_pattern, *cachedResults, t_tableIdx);
    }
  }*/

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

  /*if (m_cache->isCacheable(t_pattern)) {
    m_cache->cache(t_pattern, result);
  }*/

  // Store the result
  return storePatternResultFromPkb(t_pattern, result, t_tableIdx);
}

/**
* A function that stores the result in a data structure.
* @param t_result a vector<string> argument
*/
BOOLEAN QueryEvaluator::storeSelectResultFromPkb(Grammar t_select, LIST_OF_RESULTS_INDICES t_result) {
  if (isDebugMode) {
    std::cout << "Storing the select result from PKB to the select result queue...\n";
  }
  
  return m_table->insertOneSynonym(t_select.getName(), t_result);
}

BOOLEAN QueryEvaluator::storeRelationResultFromPkb(Relation *t_relation, SET_OF_RESULTS_INDICES t_result, INTEGER t_tableIdx) {
MAP_OF_STMT_NUM_TO_GTYPE typeOfStmts = m_pkb->getTypeOfStatementTable();
  std::unordered_map<std::string, int>::const_iterator got;
  if (QueryUtil::hasOneLeftSynonym(t_relation->getG1(), t_relation->getG2())) {
    got = m_synonymsUsedInQuery.find(t_relation->getG1().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {   
        if (!Relation::isCalls(t_relation->getType()) && !Relation::isCallsStar(t_relation->getType())
          && !Grammar::isProc(t_relation->getG1().getType()) && !Grammar::isVar(t_relation->getG2().getType())) {
          t_result[0] = EvaluatorUtil::filterStmts(typeOfStmts, t_result[0], t_relation->getG1());
          if (t_result.empty()) {
            return false;
          }
        }

        return m_tables[t_tableIdx]->insertOneSynonym(t_relation->getG1().getName(), t_result[0]);
      }
    }
  } else if (QueryUtil::hasOneRightSynonym(t_relation->getG1(), t_relation->getG2())) {
    got = m_synonymsUsedInQuery.find(t_relation->getG2().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        if (!Relation::isCalls(t_relation->getType()) && !Relation::isCallsStar(t_relation->getType())
          && !Grammar::isProc(t_relation->getG2().getType()) && !Grammar::isVar(t_relation->getG2().getType())) {
          t_result[0] = EvaluatorUtil::filterStmts(typeOfStmts, t_result[0], t_relation->getG2());
          if (t_result.empty()) {
            return false;
          }
        }
        return m_tables[t_tableIdx]->insertOneSynonym(t_relation->getG2().getName(), t_result[0]);
      }
    }
  } else if (QueryUtil::hasTwoSynonyms(t_relation->getG1(), t_relation->getG2())) {
    if (!Relation::isCalls(t_relation->getType()) && !Relation::isCallsStar(t_relation->getType())
      && !Grammar::isProc(t_relation->getG1().getType()) && !Grammar::isVar(t_relation->getG2().getType())) {
      SET_OF_RESULTS_INDICES results;
      for (auto& x : t_result) {
        LIST_OF_STMT_NUMS stmtStrVector = EvaluatorUtil::filterStmts(typeOfStmts, x.second, t_relation->getG2());
        if (!stmtStrVector.empty()) {
          LIST_OF_STMT_NUMS stmtVector = EvaluatorUtil::filterStmts(typeOfStmts, x.first, t_relation->getG1(), stmtStrVector);
          if (!stmtVector.empty()) {
            results[x.first] = stmtVector;
          }
        }
      }

      if (results.empty()) {
        return false;
      } else {
        t_result = results;
      }
    }

    got = m_synonymsUsedInQuery.find(t_relation->getG1().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        if ((Relation::isUses(t_relation->getType()) || Relation::isModifies(t_relation->getType()))
          && !Grammar::isProc(t_relation->getG1().getType())) {
          return m_tables[t_tableIdx]->insertTwoSynonym(t_relation->getG2().getName(), t_relation->getG1().getName(), t_result);
        } else {
          return m_tables[t_tableIdx]->insertTwoSynonym(t_relation->getG1().getName(), t_relation->getG2().getName(), t_result);
        }
      }

      got = m_synonymsUsedInQuery.find(t_relation->getG2().getName());
      if (got != m_synonymsUsedInQuery.end()) {
        if (got->second > 1) {
          if ((Relation::isUses(t_relation->getType()) || Relation::isModifies(t_relation->getType()))
            && !Grammar::isProc(t_relation->getG1().getType())) {
            return m_tables[t_tableIdx]->insertTwoSynonym(t_relation->getG2().getName(), t_relation->getG1().getName(), t_result);
          } else {
            return m_tables[t_tableIdx]->insertTwoSynonym(t_relation->getG1().getName(), t_relation->getG2().getName(), t_result);
          }
        }
      }
    }
  }

  return true;
}

BOOLEAN QueryEvaluator::storePatternResultFromPkb(Pattern *t_pattern, SET_OF_RESULTS_INDICES t_result, INTEGER t_tableIdx) {
  std::unordered_map<std::string, int>::const_iterator got;
  if (!Grammar::isVar(t_pattern->getLeft().getType())) {
    got = m_synonymsUsedInQuery.find(t_pattern->getStmt().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        return m_tables[t_tableIdx]->insertOneSynonym(t_pattern->getStmt().getName(), t_result[0]);
      }
    }
  } else if (Grammar::isVar(t_pattern->getLeft().getType())) {
    got = m_synonymsUsedInQuery.find(t_pattern->getStmt().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        return m_tables[t_tableIdx]->insertTwoSynonym(t_pattern->getStmt().getName(), t_pattern->getLeft().getName(), t_result);
      } else if (got->second == 1) {
        got = m_synonymsUsedInQuery.find(t_pattern->getLeft().getName());
        if (got != m_synonymsUsedInQuery.end()) {
          if (got->second > 1) {
            return m_tables[t_tableIdx]->insertTwoSynonym(t_pattern->getStmt().getName(), t_pattern->getLeft().getName(), t_result);
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
  std::vector<Grammar> selectedSynonyms;

  if (!m_isSelectOnly && !m_tables.empty()) {
    BOOLEAN hasResult = m_table->mergeTables(m_tables);
    /*if (!hasResult) {
      return finalResult;
    }*/
  } 

  if (Grammar::isBoolean(m_selects.front().getType())) {
    if (m_isSelectOnly) {
      finalResult.push_back(TRUE);
    } else if (m_table->hasSynonyms() && m_table->isEmpty()) {
      finalResult.clear();
    } else {
      finalResult.push_back(TRUE);
    }
  } else {
    for (auto& grammar : m_selects) {
      if (!Grammar::isCall(grammar.getType()) && !Grammar::isProcName(grammar.getAttr())) {
        grammar = EvaluatorUtil::rewriteSynonym(grammar, m_synsToBeRewritten);
      }

      if (!m_table->hasSynonym(grammar.getName()) || (!m_table->hasSynonym(grammar.getName())
        && Grammar::isCall(grammar.getType())
        && Grammar::isProcName(grammar.getAttr()))) {
        BOOLEAN hasResult = getSelectResultFromPkb(grammar);
        if (!hasResult) {
          return finalResult;
        }      
      }

      selectedSynonyms.push_back(grammar);
    }

    finalResult = m_table->getResults(selectedSynonyms, m_pkb);
    if (finalResult.empty()) {
      return finalResult;
    }  
  }

  m_table->clearTable();
  return finalResult;
}

BOOLEAN QueryEvaluator::getWithResult(With *t_with, INTEGER t_tableIdx) {
  Grammar left = t_with->getG1();
  Grammar right = t_with->getG2();

  if ((Grammar::isStmtNo(left.getType()) || Grammar::isString(left.getType())) 
    && (Grammar::isStmtNo(right.getType()) || Grammar::isString(right.getType()))) {
    //Compare left and right (int = int, str = str)
    return true;
  } else if (Grammar::isProgLine(left.getType()) && Grammar::isProgLine(right.getType())) {
    //Rewrite syn = syn
    //With n = n
    if (left.getName() == right.getName()) {
      return true;
    }

    //With n1 = n2 but no other clauses uses n1 and n2
    if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, left.getName()) 
      && !QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, right.getName())) {
      return true;
    }

    //With n1 = n2 with other clauses using n1 or n2
    if (m_synonymsUsedInQuery[left.getName()] >= m_synonymsUsedInQuery[right.getName()]) {
      m_synsToBeRewritten[right.getName()] = left;
    } else {
      m_synsToBeRewritten[left.getName()] = right;
    }
  } else if ((Grammar::isProgLine(left.getType()) || Grammar::isStmtNo(left.getType())) 
    && (Grammar::isStmtNo(right.getType()) || Grammar::isProgLine(right.getType()))) {
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
      m_synsToBeRewritten[right.getName()] = left;
      LIST_OF_RESULTS_INDICES results;
      results.push_back(std::stoi(left.getName()));
      return m_tables[t_tableIdx]->insertOneSynonym(right.getName(), results);
    } else {
      if (std::stoi(right.getName()) > totalStmts) {
        return false;
      }
      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, left.getName())) {
        return true;
      }
      m_synsToBeRewritten[left.getName()] = right;
      LIST_OF_RESULTS_INDICES results;
      results.push_back(std::stoi(right.getName()));
      return m_tables[t_tableIdx]->insertOneSynonym(left.getName(), results);
    }
  } else if (left.hasAttr() && right.hasAttr()) {
    //Evaluate attr = attr
    if (left.getName() == right.getName()) {
      return true;
    }

    if (left.getType() == right.getType()) {
      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, left.getName()) 
        && !QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, right.getName())) {
        return true;
      }

      if (Grammar::isConst(left.getType())) {
        //Select <c1, c2> or Select c1 or Select c2
        LIST_OF_CONSTANT_INDICES allConstants = m_pkb->getAllConstantsByIdx();
        SET_OF_RESULTS_INDICES results;
        for (auto& constant : allConstants) {
          LIST_OF_CONSTANT_INDICES constVector;
          constVector.push_back(constant);
          results[constant] = constVector;
        }

        return m_tables[t_tableIdx]->insertTwoSynonym(left.getName(), right.getName(), results);
      }

      if (m_synonymsUsedInQuery[left.getName()] >= m_synonymsUsedInQuery[right.getName()]) {
        m_synsToBeRewritten[right.getName()] = left;
        return true;
      } else {
        m_synsToBeRewritten[left.getName()] = right;
        return true;
      }
    }

    if (left.getAttr() == right.getAttr()) {
      if (Grammar::isStmtNum(left.getAttr())) {
        if (Grammar::isStmt(left.getType())) {
          m_synsToBeRewritten[left.getName()] = right;
        } else if (Grammar::isStmt(right.getType())) {
          m_synsToBeRewritten[right.getName()] = left;
        } else {
          return false;
        }
      } else if (Grammar::isProcName(left.getAttr())) {
        LIST_OF_PROC_NAMES allProcsCalled = m_pkb->getCalledByAnything();
        SET_OF_RESULTS_INDICES results;
        for (auto& procName : allProcsCalled) {
          PROC_INDEX procIdx = m_pkb->getProcIdxFromName(procName);
          LIST_OF_STMT_NUMS callStmts = m_pkb->getStmtNumsFromProcName(procName);
          if (callStmts.empty()) {
            return false;
          }
          
          results[procIdx] = callStmts;
        }

        if (Grammar::isProc(left.getType())) {
          return m_tables[t_tableIdx]->insertTwoSynonym(left.getName(), right.getName(), results);
        } else {
          return m_tables[t_tableIdx]->insertTwoSynonym(right.getName(), left.getName(), results);
        }
      }
    }

    if (left.getAttr() != right.getAttr()) {
      if ((Grammar::isStmtNum(left.getAttr()) && Grammar::isValue(right.getAttr())) 
        || (Grammar::isStmtNum(right.getAttr()) && Grammar::isValue(left.getAttr()))) {
        if (Grammar::isStmt(left.getType()) || Grammar::isStmt(right.getType())) {
          MAP_OF_STMT_NUM_TO_GTYPE allStmts = m_pkb->getTypeOfStatementTable();
          LIST_OF_CONSTANT_INDICES allConstants = m_pkb->getAllConstantsByIdx();
          SET_OF_RESULTS_INDICES results = EvaluatorUtil::getCommonProgLineAndConstant(allConstants, allStmts.size(), m_pkb);
          if (results.empty()) {
            return false;
          }

          if (Grammar::isStmt(left.getType())) {
            return m_tables[t_tableIdx]->insertTwoSynonym(right.getName(), left.getName(), results);
          }

          return m_tables[t_tableIdx]->insertTwoSynonym(left.getName(), right.getName(), results);
        } else if (Grammar::isAssign(left.getType()) || Grammar::isAssign(right.getType())) {
          LIST_OF_STMT_NUMS allAssignStmts = m_pkb->getAllAssignStmts();
          LIST_OF_CONSTANT_TERMS allConstants = m_pkb->getAllConstants();
          LIST_OF_RESULTS allAssignStmtsInStr = Formatter::formatVectorIntToVectorStr(allAssignStmts);
          LIST_OF_RESULTS commonResults = EvaluatorUtil::getCommonResults(allConstants, allAssignStmtsInStr);
          if (commonResults.empty()) {
            return false;
          }

          SET_OF_RESULTS_INDICES results = Formatter::formatVectorStrToMapIntVectorIntForValues(commonResults, m_pkb);
          if (Grammar::isAssign(left.getType())) {
            return m_tables[t_tableIdx]->insertTwoSynonym(right.getName(), left.getName(), results);
          }
          
          return m_tables[t_tableIdx]->insertTwoSynonym(left.getName(), right.getName(), results);
        } else if (Grammar::isWhile(left.getType()) || Grammar::isWhile(right.getType())) {
          LIST_OF_STMT_NUMS allWhileStmts = m_pkb->getAllWhileStmts();
          LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
          LIST_OF_RESULTS allWhileStmtsInStr = Formatter::formatVectorIntToVectorStr(allWhileStmts);
          LIST_OF_RESULTS commonResults = EvaluatorUtil::getCommonResults(allConstants, allWhileStmtsInStr);
          if (commonResults.empty()) {
            return false;
          }

          SET_OF_RESULTS_INDICES results = Formatter::formatVectorStrToMapIntVectorIntForValues(commonResults, m_pkb);
          if (Grammar::isWhile(left.getType())) {
            return m_tables[t_tableIdx]->insertTwoSynonym(right.getName(), left.getName(), results);
          }
          
          return m_tables[t_tableIdx]->insertTwoSynonym(left.getName(), right.getName(), results);
        } else if (Grammar::isIf(left.getType()) || Grammar::isIf(right.getType())) {
          LIST_OF_STMT_NUMS allIfStmts = m_pkb->getAllIfStmts();
          LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
          LIST_OF_RESULTS allIfStmtsInStr = Formatter::formatVectorIntToVectorStr(allIfStmts);
          LIST_OF_RESULTS commonResults = EvaluatorUtil::getCommonResults(allConstants, allIfStmtsInStr);
          if (commonResults.empty()) {
            return false;
          }

          SET_OF_RESULTS_INDICES results = Formatter::formatVectorStrToMapIntVectorIntForValues(commonResults, m_pkb);
          if (Grammar::isIf(left.getType())) {
            return m_tables[t_tableIdx]->insertTwoSynonym(right.getName(), left.getName(), results);
          }

          return m_tables[t_tableIdx]->insertTwoSynonym(left.getName(), right.getName(), results);
        } else if (Grammar::isCall(left.getType()) || Grammar::isCall(right.getType())) {
          MAP_OF_GTYPE_TO_LIST_OF_STMT_NUMS allStmts = m_pkb->getStatementTypeTable();
          LIST_OF_STMT_NUMS allCallStmts = allStmts[queryType::GType::CALL];
          LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
          LIST_OF_RESULTS allCallStmtsInStr = Formatter::formatVectorIntToVectorStr(allCallStmts);
          LIST_OF_RESULTS commonResults = EvaluatorUtil::getCommonResults(allConstants, allCallStmtsInStr);
          if (commonResults.empty()) {
            return false;
          }

          SET_OF_RESULTS_INDICES results = Formatter::formatVectorStrToMapIntVectorIntForValues(commonResults, m_pkb);
          if (Grammar::isCall(left.getType())) {
            return m_tables[t_tableIdx]->insertTwoSynonym(right.getName(), left.getName(), results);
          }

          return m_tables[t_tableIdx]->insertTwoSynonym(left.getName(), right.getName(), results);
        }
      } else if ((Grammar::isProcName(left.getAttr()) && Grammar::isVarName(right.getAttr())) 
        || (Grammar::isProcName(right.getAttr()) && Grammar::isVarName(left.getAttr()))) {
        if (Grammar::isProc(left.getType()) || Grammar::isProc(right.getType())) {
          LIST_OF_PROC_NAMES allProcNames = m_pkb->getAllProcsName();
          LIST_OF_VAR_NAMES allVarNames = m_pkb->getAllVarNames();
          LIST_OF_RESULTS commonNames = EvaluatorUtil::getCommonResults(allProcNames, allVarNames);
          if (commonNames.empty()) {
            return false;
          }

          SET_OF_RESULTS_INDICES results = Formatter::formatVectorStrToMapIntVectorIntForNames(commonNames, m_pkb);
          if (Grammar::isProc(left.getType())) {
            return m_tables[t_tableIdx]->insertTwoSynonym(left.getName(), right.getName(), results);
          }

          return m_tables[t_tableIdx]->insertTwoSynonym(right.getName(), left.getName(), results);
        } else if (Grammar::isCall(left.getType()) || Grammar::isCall(right.getType())) {
          LIST_OF_PROC_NAMES allProcsCalled = m_pkb->getCalledByAnything();
          LIST_OF_VAR_NAMES allVarNames = m_pkb->getAllVarNames();
          LIST_OF_RESULTS commonNames = EvaluatorUtil::getCommonResults(allProcsCalled, allVarNames);
          SET_OF_RESULTS_INDICES results;
          for (auto& procName : commonNames) {
            LIST_OF_STMT_NUMS callStmts = m_pkb->getStmtNumsFromProcName(procName);
            PROC_INDEX procIdx = m_pkb->getProcIdxFromName(procName);
            if (callStmts.empty()) {
              return false;
            }

            results[procIdx] = callStmts;
          }

          if (Grammar::isVar(left.getType())) {
            return m_tables[t_tableIdx]->insertTwoSynonym(left.getName(), right.getName(), results);
          } else {
            return m_tables[t_tableIdx]->insertTwoSynonym(right.getName(), left.getName(), results);
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
      LIST_OF_RESULTS_INDICES allConstants = m_pkb->getAllConstantsByIdx();
      SET_OF_RESULTS_INDICES results = EvaluatorUtil::getCommonProgLineAndConstant(allConstants, allStmts.size(), m_pkb);
      if (results.empty()) {
        return false;
      }

      if (Grammar::isProgLine(left.getType())) {
        return m_tables[t_tableIdx]->insertTwoSynonym(right.getName(), left.getName(), results);
      }

      return m_tables[t_tableIdx]->insertTwoSynonym(left.getName(), right.getName(), results);
    } else if (Grammar::isStmtNum(left.getAttr()) || Grammar::isStmtNum(right.getAttr())) {
      if (Grammar::isStmt(left.getType())) {
        m_synsToBeRewritten[right.getName()] = left;
      } else if (Grammar::isStmt(right.getType())) {
        m_synsToBeRewritten[left.getName()] = right;
      } else if (Grammar::isAssign(left.getType())) {
        m_synsToBeRewritten[right.getName()] = left;
      } else if (Grammar::isAssign(right.getType())) {
        m_synsToBeRewritten[left.getName()] = right;
      } else if (Grammar::isWhile(left.getType())) {
        m_synsToBeRewritten[right.getName()] = left;
      } else if (Grammar::isWhile(right.getType())) {
        m_synsToBeRewritten[left.getName()] = right;
      } else if (Grammar::isIf(left.getType())) {
        m_synsToBeRewritten[right.getName()] = left;
      } else if (Grammar::isIf(right.getType())) {
        m_synsToBeRewritten[left.getName()] = right;
      } else if (Grammar::isCall(left.getType())) {
        m_synsToBeRewritten[right.getName()] = left;
      } else if (Grammar::isCall(right.getType())) {
        m_synsToBeRewritten[left.getName()] = right;
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

      return m_tables[t_tableIdx]->insertOneSynonym(left.getName(), callStmts);
    } else if (Grammar::isCall(right.getType())) {
      LIST_OF_STMT_NUMS callStmts = m_pkb->getStmtNumsFromProcName(left.getName());
      if (callStmts.empty()) {
        return false;
      }

      return m_tables[t_tableIdx]->insertOneSynonym(right.getName(), callStmts);
    }

    if (Grammar::isString(left.getType())) {
      INTEGER index;
      if (Grammar::isProc(right.getType())) {
        LIST_OF_RESULTS allProcNames = m_pkb->getAllProcsName();
        if (std::find(allProcNames.begin(), allProcNames.end(), left.getName()) == allProcNames.end()) {
          return false;
        }

        index = m_pkb->getProcIdxFromName(left.getName());
      } else if (Grammar::isVar(right.getType())) {
        LIST_OF_RESULTS allVarNames = m_pkb->getAllVarNames();
        if (std::find(allVarNames.begin(), allVarNames.end(), left.getName()) == allVarNames.end()) {
          return false;
        }

        index = m_pkb->getVarIdxFromName(left.getName());
      }

      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, right.getName())) {
        return true;
      }

      m_synsToBeRewritten[right.getName()] = left;
      LIST_OF_RESULTS_INDICES results;
      results.push_back(index);
      return m_tables[t_tableIdx]->insertOneSynonym(right.getName(), results);
    } else {
      INTEGER index;
      if (Grammar::isProc(left.getType())) {
        LIST_OF_RESULTS allProcNames = m_pkb->getAllProcsName();
        if (std::find(allProcNames.begin(), allProcNames.end(), right.getName()) == allProcNames.end()) {
          return false;
        }

        index = m_pkb->getProcIdxFromName(right.getName());
      } else if (Grammar::isVar(left.getType())) {
        LIST_OF_RESULTS allVarNames = m_pkb->getAllVarNames();
        if (std::find(allVarNames.begin(), allVarNames.end(), right.getName()) == allVarNames.end()) {
          return false;
        }

        index = m_pkb->getVarIdxFromName(right.getName());
      }

      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, left.getName())) {
        return true;
      }

      m_synsToBeRewritten[left.getName()] = right;
      LIST_OF_RESULTS_INDICES results;
      results.push_back(index);
      return m_tables[t_tableIdx]->insertOneSynonym(left.getName(), results);
    }
  } else if ((left.hasAttr() || Grammar::isStmtNo(left.getType())) 
    && (Grammar::isStmtNo(right.getType()) || right.hasAttr())) {
    //Rewrite attr = int, int = attr
    if (Grammar::isValue(left.getAttr())) {
      LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
      if (std::find(allConstants.begin(), allConstants.end(), right.getName()) != allConstants.end()) {
        CONSTANT_INDEX constantIndex = m_pkb->getConstantIdxFromConstant(right.getName());
        LIST_OF_RESULTS_INDICES results;
        results.push_back(constantIndex);
        return m_tables[t_tableIdx]->insertOneSynonym(left.getName(), results);
      } else {
        return false;
      }
    } else if (Grammar::isValue(right.getAttr())) {
      LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
      if (std::find(allConstants.begin(), allConstants.end(), left.getName()) != allConstants.end()) {
        CONSTANT_INDEX constantIndex = m_pkb->getConstantIdxFromConstant(left.getName());
        LIST_OF_RESULTS_INDICES results;
        results.push_back(constantIndex);
        return m_tables[t_tableIdx]->insertOneSynonym(right.getName(), results);
      } else {
        return false;
      }
    }

    if (Grammar::isStmtNum(left.getAttr())) {
      MAP_OF_STMT_NUM_TO_GTYPE allStmts = m_pkb->getTypeOfStatementTable();
      std::unordered_map<STMT_NUM, SYNONYM_TYPE>::iterator got;
      if (Grammar::isStmt(left.getType())) {  
        int maxStmtNo = allStmts.size();
        if (std::stoi(right.getName()) > maxStmtNo) {
          return false;
        }
      } else if (Grammar::isAssign(left.getType())) {
        got = allStmts.find(std::stoi(right.getName()));
        if (got != allStmts.end()) {
          if (!Grammar::isAssign(got->second)) {
            return false;
          }
        } else {
          return false;
        }
      } else if (Grammar::isWhile(left.getType())) {
        got = allStmts.find(std::stoi(right.getName()));
        if (got != allStmts.end()) {
          if (!Grammar::isWhile(got->second)) {
            return false;
          }
        } else {
          return false;
        }
      } else if (Grammar::isIf(left.getType())) {
        got = allStmts.find(std::stoi(right.getName()));
        if (got != allStmts.end()) {
          if (!Grammar::isIf(got->second)) {
            return false;
          }
        } else {
          return false;
        }
      } else if (Grammar::isCall(left.getType())) {
        got = allStmts.find(std::stoi(right.getName()));
        if (got != allStmts.end()) {
          if (!Grammar::isCall(got->second)) {
            return false;
          }
        } else {
          return false;
        }
      }

      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, left.getName())) {
        return true;
      }

      m_synsToBeRewritten[left.getName()] = right;
      LIST_OF_RESULTS_INDICES results;
      results.push_back(std::stoi(right.getName()));
      return m_tables[t_tableIdx]->insertOneSynonym(left.getName(), results);
    } else if (Grammar::isStmtNum(right.getAttr())) {
      MAP_OF_STMT_NUM_TO_GTYPE allStmts = m_pkb->getTypeOfStatementTable();
      std::unordered_map<STMT_NUM, SYNONYM_TYPE>::iterator got;
      if (Grammar::isStmt(right.getType())) {
        int maxStmtNo = allStmts.size();
        if (std::stoi(left.getName()) > maxStmtNo) {
          return false;
        }
      } else if (Grammar::isAssign(right.getType())) {
        got = allStmts.find(std::stoi(left.getName()));
        if (got != allStmts.end()) {
          if (!Grammar::isAssign(got->second)) {
            return false;
          }
        } else {
          return false;
        }
      } else if (Grammar::isWhile(right.getType())) {
        got = allStmts.find(std::stoi(left.getName()));
        if (got != allStmts.end()) {
          if (!Grammar::isWhile(got->second)) {
            return false;
          }
        } else {
          return false;
        }
      } else if (Grammar::isIf(right.getType())) {
        got = allStmts.find(std::stoi(left.getName()));
        if (got != allStmts.end()) {
          if (!Grammar::isIf(got->second)) {
            return false;
          }
        } else {
          return false;
        }
      } else if (Grammar::isCall(right.getType())) {
        got = allStmts.find(std::stoi(left.getName()));
        if (got != allStmts.end()) {
          if (!Grammar::isCall(got->second)) {
            return false;
          }
        } else {
          return false;
        }
      }

      if (!QueryUtil::isSynonymCommon(m_synonymsUsedInQuery, right.getName())) {
        return true;
      }

      m_synsToBeRewritten[right.getName()] = left;
      LIST_OF_RESULTS_INDICES results;
      results.push_back(std::stoi(left.getName()));
      return m_tables[t_tableIdx]->insertOneSynonym(right.getName(), results);
    } else {
      return false;
    }
  } else {
    return false;
  }

  return true;
}