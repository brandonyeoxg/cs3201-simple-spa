#pragma once

#include "QueryEvaluator.h"

/**
* A function that evaluates the query that has been pre-processed by the QueryPreprocessor.
*/
LIST_OF_RESULTS QueryEvaluator::evaluateQuery() {
  //std::cout << "Evaluating Query...\n";
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
  //std::cout << "Getting results from PKB...\n";
  int relationSize = m_relations.size();
  int patternSize = m_patterns.size();
  //int withSize = m_withs.size();

  Grammar grammar = m_selects.front();
  m_selectedSynonym = grammar.getName();
  m_selectedType = grammar.getType();

  //Loop through the With Queue
  /*for (int i = 0; i < withSize; ++i) {
    m_isSelectOnly = false;
    With with = m_withs.front();
    BOOLEAN hasResult = getWithResult(with);
    if (!hasResult) {
      return false;
    }

    m_withs.pop();
  }*/

  //Loop through the Relation Queue
  for (int i = 0; i < relationSize; ++i) {
    m_isSelectOnly = false;
    Relation relation = m_relations.front();
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
  //std::cout << "Storing the select result from PKB to the select result queue...\n";
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
  //std::cout << "Evaluating the final result...\n";
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

  WithEvaluator *eval = new WithEvaluator(m_pkb, left, right, m_synonymsUsedInQuery, m_selectedSynonym);
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_INTEGERS synToInt;
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_STRING synToStr;
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_SYNONYMS synToSyn;
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_INTEGERS synToIntList;
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_STRINGS synToStrList;

  if ((Grammar::isStmtNo(left.getType()) || Grammar::isString(left.getType())) && (Grammar::isStmtNo(right.getType()) || Grammar::isString(left.getType()))) {
    BOOLEAN isEquals = eval->isEquals();
    delete eval;
    return isEquals;
  } else if (Grammar::isStmtNo(left.getType()) && !Grammar::isStmtNo(right.getType()) && !Grammar::isString(right.getType())) {
    synToInt = eval->evaluateSynWithInt();
    if (synToInt.empty()) {
      return false;
    }

    if (Grammar::isConst(right.getType())) {
      if (m_selectedSynonym != right.getName()) {
        return true;
      } else {
        //Todo:
      }
    }

    rewriteSynAsInt(synToInt);
  } else if (!Grammar::isStmtNo(left.getType()) && !Grammar::isString(left.getType()) && Grammar::isStmtNo(right.getType())) {
    synToInt = eval->evaluateSynWithInt();
    if (synToInt.empty()) {
      return false;
    }

    if (Grammar::isConst(left.getType())) {
      if (m_selectedSynonym != left.getName()) {
        return true;
      } else {
        //Todo:
      }
    }

    rewriteSynAsInt(synToInt);
  } else if (Grammar::isString(left.getType()) && !Grammar::isStmtNo(right.getType()) && !Grammar::isString(right.getType())) {
    synToStr = eval->evaluateSynWithStr();
    if (synToStr.empty()) {
      return false;
    }
    rewriteSynAsStr(synToStr);
  } else if (!Grammar::isStmtNo(left.getType()) && !Grammar::isString(left.getType()) && Grammar::isString(right.getType())) {
    synToStr = eval->evaluateSynWithStr();
    if (synToStr.empty()) {
      return false;
    }
    rewriteSynAsStr(synToStr);
  } else if (!Grammar::isStmtNo(left.getType()) && !Grammar::isString(left.getType()) && !Grammar::isStmtNo(right.getType()) && !Grammar::isString(right.getType())) {
    if (Grammar::isProgLine(left.getType()) && !Grammar::isProgLine(right.getType())) {
      synToIntList = eval->evaluateIntAttrWithIntAttr();
      if (synToIntList.empty()) {
        return false;
      }
      rewriteSynAsIntList(synToIntList);
    } else if (!Grammar::isProgLine(left.getType()) && Grammar::isProgLine(right.getType())) {
      synToIntList = eval->evaluateIntAttrWithIntAttr();
      if (synToIntList.empty()) {
        return false;
      }
      rewriteSynAsIntList(synToIntList);
    } else if (Grammar::isProgLine(left.getType()) && Grammar::isProgLine(right.getType())) {
      if (left.getName() == right.getName()) {
        return true;
      }

      synToSyn = eval->evaluateSynWithSyn();
      if (synToSyn.empty()) {
        return false;
      }
      rewriteSynAsSyn(synToSyn);
    } else if (left.getAttr() == right.getAttr()) {
      if (left.getName() == right.getName()) {
        return true;
      }

      synToSyn = eval->evaluateSynWithSyn();
      if (synToSyn.empty()) {
        return false;
      }
      rewriteSynAsSyn(synToSyn);
    } else if (left.getAttr() != right.getAttr()) {
      if (Grammar::isProcName(left.getAttr()) || Grammar::isVarName(left.getAttr())) {
        synToStrList = eval->evaluateStrAttrWithStrAttr();
        if (synToStrList.empty()) {
          return false;
        }
        rewriteSynAsStrList(synToStrList);
      } else {
        synToIntList = eval->evaluateIntAttrWithIntAttr();
        if (synToIntList.empty()) {
          return false;
        }
        rewriteSynAsIntList(synToIntList);
      }
    }
  }

  delete eval;
  return true;
}

void QueryEvaluator::rewriteSynAsInt(MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_INTEGERS t_synToInt) {
  int relationSize = m_relations.size();
  for (int i = 0; i < relationSize; ++i) {
    Relation relation = m_relations.front();
    Grammar g1 = relation.getG1();
    Grammar g2 = relation.getG2();
    Grammar newGrammar;
    Relation newRelation;

    std::unordered_map<std::string, int>::const_iterator got;
    got = t_synToInt.find(g1.getName());
    if (got != t_synToInt.end()) {
      newGrammar = Grammar(10, std::to_string(got->second));
      newRelation = Relation(relation.getTypeInString(), newGrammar, g2);
      m_relations.push(newRelation);
    }

    got = t_synToInt.find(g2.getName());
    if (got != t_synToInt.end()) {
      newGrammar = Grammar(10, std::to_string(got->second));
      newRelation = Relation(relation.getTypeInString(), g1, newGrammar);
      m_relations.push(newRelation);
    }

    m_relations.pop();
  }

  int patternSize = m_patterns.size();
  for (int i = 0; i < patternSize; ++i) {
    Pattern pattern = m_patterns.front();
    Grammar stmt = pattern.getStmt();
    Grammar left = pattern.getLeft();
    Grammar right = pattern.getRight();
    BOOLEAN isSubtree = pattern.isSubtree();
    Grammar newGrammar;
    Pattern newPattern;

    std::unordered_map<std::string, int>::const_iterator got;
    got = t_synToInt.find(stmt.getName());
    if (got != t_synToInt.end()) {
      newGrammar = Grammar(10, std::to_string(got->second));
      newPattern = Pattern(newGrammar, left, right, isSubtree);
      m_patterns.push(newPattern);
    }

    m_patterns.pop();
  }
}

void QueryEvaluator::rewriteSynAsStr(MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_STRING t_synToStr) {
  int relationSize = m_relations.size();
  for (int i = 0; i < relationSize; ++i) {
    Relation relation = m_relations.front();
    Grammar g1 = relation.getG1();
    Grammar g2 = relation.getG2();
    Grammar newGrammar;
    Relation newRelation;

    std::unordered_map<std::string, std::string>::const_iterator got;
    got = t_synToStr.find(g1.getName());
    if (got != t_synToStr.end()) {
      newGrammar = Grammar(11, got->second);
      newRelation = Relation(relation.getTypeInString(), newGrammar, g2);
      m_relations.push(newRelation);
    }

    got = t_synToStr.find(g2.getName());
    if (got != t_synToStr.end()) {
      newGrammar = Grammar(11, got->second);
      newRelation = Relation(relation.getTypeInString(), g1, newGrammar);
      m_relations.push(newRelation);
    }

    m_relations.pop();
  }

  int patternSize = m_patterns.size();
  for (int i = 0; i < patternSize; ++i) {
    Pattern pattern = m_patterns.front();
    Grammar stmt = pattern.getStmt();
    Grammar left = pattern.getLeft();
    Grammar right = pattern.getRight();
    BOOLEAN isSubtree = pattern.isSubtree();
    Grammar newGrammar;
    Pattern newPattern;

    std::unordered_map<std::string, std::string>::const_iterator got;
    got = t_synToStr.find(left.getName());
    if (got != t_synToStr.end()) {
      newGrammar = Grammar(11, got->second);
      newPattern = Pattern(stmt, newGrammar, right, isSubtree);
      m_patterns.push(newPattern);
    }

    got = t_synToStr.find(right.getName());
    if (got != t_synToStr.end()) {
      newGrammar = Grammar(11, got->second);
      newPattern = Pattern(stmt, left, newGrammar, isSubtree);
      m_patterns.push(newPattern);
    }

    m_patterns.pop();
  }
}

void QueryEvaluator::rewriteSynAsSyn(MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_SYNONYMS t_synToSyn) {
  int relationSize = m_relations.size();
  for (int i = 0; i < relationSize; ++i) {
    Relation relation = m_relations.front();
    Grammar g1 = relation.getG1();
    Grammar g2 = relation.getG2();
    Grammar newGrammar;
    Relation newRelation;

    std::unordered_map<std::string, std::string>::const_iterator got;
    got = t_synToSyn.find(g1.getName());
    if (got != t_synToSyn.end()) {
      newGrammar = Grammar(g1.getType(), got->second);
      newRelation = Relation(relation.getTypeInString(), newGrammar, g2);
      m_relations.push(newRelation);
    }

    got = t_synToSyn.find(g2.getName());
    if (got != t_synToSyn.end()) {
      newGrammar = Grammar(g2.getType(), got->second);
      newRelation = Relation(relation.getTypeInString(), g1, newGrammar);
      m_relations.push(newRelation);
    }

    m_relations.pop();
  }

  int patternSize = m_patterns.size();
  for (int i = 0; i < patternSize; ++i) {
    Pattern pattern = m_patterns.front();
    Grammar stmt = pattern.getStmt();
    Grammar left = pattern.getLeft();
    Grammar right = pattern.getRight();
    BOOLEAN isSubtree = pattern.isSubtree();
    Grammar newGrammar;
    Pattern newPattern;

    std::unordered_map<std::string, std::string>::const_iterator got;
    got = t_synToSyn.find(stmt.getName());
    if (got != t_synToSyn.end()) {
      newGrammar = Grammar(stmt.getType(), got->second);
      newPattern = Pattern(newGrammar, left, right, isSubtree);
      m_patterns.push(newPattern);
    }

    got = t_synToSyn.find(left.getName());
    if (got != t_synToSyn.end()) {
      newGrammar = Grammar(left.getType(), got->second);
      newPattern = Pattern(stmt, newGrammar, right, isSubtree);
      m_patterns.push(newPattern);
    }

    got = t_synToSyn.find(right.getName());
    if (got != t_synToSyn.end()) {
      newGrammar = Grammar(right.getType(), got->second);
      newPattern = Pattern(stmt, left, newGrammar, isSubtree);
      m_patterns.push(newPattern);
    }

    m_patterns.pop();
  }
}

void QueryEvaluator::rewriteSynAsIntList(MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_INTEGERS t_synToIntList) {

}

void QueryEvaluator::rewriteSynAsStrList(MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_STRINGS t_synToStrList) {

}