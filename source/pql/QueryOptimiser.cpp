#pragma once

#include "QueryOptimiser.h"

void QueryOptimiser::divideClausesIntoGroups(std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses> &t_noSyns, std::priority_queue<std::pair<std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses>*, int>, std::vector<std::pair<std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses>*, int>>, QueryOptimiser::compareGroups> &t_withSyns) {
  std::queue<Clause*> syns;
  std::vector<std::vector<Clause*>*> allGroupsWithSyns;

  for (auto& relation : m_relations) {
    Relation *relPtr = &relation;
    if (QueryUtil::hasNoSynonyms(relPtr->getG1(), relPtr->getG2())) {
      t_noSyns.push(relPtr);
    } else { 
      syns.push(relPtr);
    }
  }

  for (auto& pattern : m_patterns) {
    Pattern *patPtr = &pattern;
    syns.push(patPtr);
  }

  for (auto& with : m_withs) {
    With *withPtr = &with;
    if (QueryUtil::hasNoSynonyms(withPtr->getG1(), withPtr->getG2())) {
      t_noSyns.push(withPtr);
    } else {
      syns.push(withPtr);
    }
  }
  
  if (syns.empty()) {
    return;
  }

  std::vector<Clause*> *newGroup = new std::vector<Clause*>();
  newGroup->push_back(syns.front());
  allGroupsWithSyns.push_back(newGroup);
  syns.pop();

  int numOfGroups = allGroupsWithSyns.size();
  for (int i = 0; i < numOfGroups; ++i) {
    int numOfClauses = (*allGroupsWithSyns[i]).size();
    for (int j = 0; j < numOfClauses; ++j) {
      Clause *existingClause = (*allGroupsWithSyns[i])[j];
      int numOfSyns = syns.size();
      for (int k = 0; k < numOfSyns; ++k) {
        bool hasCommon = false;
        Clause *clause = syns.front();
        clause->multiplyWeights(getDynamicWeights(clause));
        if (existingClause->getClauseType() == queryType::clauseType::RELATION 
          && clause->getClauseType() == queryType::clauseType::RELATION) {
          Relation *existingRelation = (Relation*)existingClause;
          Relation *relation = (Relation*)clause;
          if (relation->getG1().getName() == existingRelation->getG1().getName()
            || relation->getG1().getName() == existingRelation->getG2().getName()
            || relation->getG2().getName() == existingRelation->getG1().getName()
            || relation->getG2().getName() == existingRelation->getG2().getName()) {
            hasCommon = true;        
          }
        } else if (existingClause->getClauseType() == queryType::clauseType::RELATION
          && clause->getClauseType() == queryType::clauseType::PATTERN) {
          Relation *existingRelation = (Relation*)existingClause;
          Pattern *pattern = (Pattern*)clause;
          if (pattern->getStmt().getName() == existingRelation->getG1().getName()
            || pattern->getStmt().getName() == existingRelation->getG2().getName()
            || pattern->getLeft().getName() == existingRelation->getG1().getName()
            || pattern->getLeft().getName() == existingRelation->getG2().getName()) {
            hasCommon = true;
          }
        } else if (existingClause->getClauseType() == queryType::clauseType::RELATION
          && clause->getClauseType() == queryType::clauseType::WITH) {
          Relation *existingRelation = (Relation*)existingClause;
          With *with = (With*)clause;
          if (with->getG1().getName() == existingRelation->getG1().getName()
            || with->getG1().getName() == existingRelation->getG2().getName()
            || with->getG2().getName() == existingRelation->getG1().getName()
            || with->getG2().getName() == existingRelation->getG2().getName()) {
            hasCommon = true;
          }
        } else if (existingClause->getClauseType() == queryType::clauseType::PATTERN
          && clause->getClauseType() == queryType::clauseType::PATTERN) {
          Pattern *existingPattern = (Pattern*)existingClause;
          Pattern *pattern = (Pattern*)clause;
          if (pattern->getStmt().getName() == existingPattern->getStmt().getName()
            || pattern->getStmt().getName() == existingPattern->getLeft().getName()
            || pattern->getLeft().getName() == existingPattern->getStmt().getName()
            || pattern->getLeft().getName() == existingPattern->getLeft().getName()) {
            hasCommon = true;
          }
        } else if (existingClause->getClauseType() == queryType::clauseType::PATTERN
          && clause->getClauseType() == queryType::clauseType::RELATION) {
          Pattern *existingPattern = (Pattern*)existingClause;
          Relation *relation = (Relation*)clause;
          if (relation->getG1().getName() == existingPattern->getStmt().getName()
            || relation->getG1().getName() == existingPattern->getLeft().getName()
            || relation->getG2().getName() == existingPattern->getStmt().getName()
            || relation->getG2().getName() == existingPattern->getLeft().getName()) {
            hasCommon = true;
          }
        } else if (existingClause->getClauseType() == queryType::clauseType::PATTERN
          && clause->getClauseType() == queryType::clauseType::WITH) {
          Pattern *existingPattern = (Pattern*)existingClause;
          With *with = (With*)clause;
          if (with->getG1().getName() == existingPattern->getStmt().getName()
            || with->getG1().getName() == existingPattern->getLeft().getName()
            || with->getG2().getName() == existingPattern->getStmt().getName()
            || with->getG2().getName() == existingPattern->getLeft().getName()) {
            hasCommon = true;
          }
        } else if (existingClause->getClauseType() == queryType::clauseType::WITH
          && clause->getClauseType() == queryType::clauseType::WITH) {
          With *existingWith = (With*)existingClause;
          With *with = (With*)clause;
          if (with->getG1().getName() == existingWith->getG1().getName()
            || with->getG1().getName() == existingWith->getG2().getName()
            || with->getG2().getName() == existingWith->getG1().getName()
            || with->getG2().getName() == existingWith->getG2().getName()) {
            hasCommon = true;
          }
        } else if (existingClause->getClauseType() == queryType::clauseType::WITH
          && clause->getClauseType() == queryType::clauseType::RELATION) {
          With *existingWith = (With*)existingClause;
          Relation *relation = (Relation*)clause;
          if (relation->getG1().getName() == existingWith->getG1().getName()
            || relation->getG1().getName() == existingWith->getG2().getName()
            || relation->getG2().getName() == existingWith->getG1().getName()
            || relation->getG2().getName() == existingWith->getG2().getName()) {
            hasCommon = true;
          }
        } else if (existingClause->getClauseType() == queryType::clauseType::WITH
          && clause->getClauseType() == queryType::clauseType::PATTERN) {
          With *existingWith = (With*)existingClause;
          Pattern *pattern = (Pattern*)clause;
          if (pattern->getStmt().getName() == existingWith->getG1().getName()
            || pattern->getStmt().getName() == existingWith->getG2().getName()
            || pattern->getLeft().getName() == existingWith->getG1().getName()
            || pattern->getLeft().getName() == existingWith->getG2().getName()) {        
            hasCommon = true;
          }
        }
    
        if (hasCommon) {
          (*allGroupsWithSyns[i]).push_back(clause);
          numOfClauses++;
        } else {
          syns.push(clause);
        }

        syns.pop(); 
      }
    }

    if (!syns.empty()) {
      std::vector<Clause*> *newGroup = new std::vector<Clause*>();
      syns.front()->multiplyWeights(getDynamicWeights(syns.front()));
      newGroup->push_back(syns.front());
      allGroupsWithSyns.push_back(newGroup);
      numOfGroups++;
      syns.pop();
    }  
  }

  int numOfNonSelectedGroups = 0;
  for (auto& group : allGroupsWithSyns) {
    int totalWeights = 0;
    BOOLEAN hasSelectedSyns = false;
    std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses> *grp = new std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses>();
    for (auto& clause : *group) {
      grp->push(clause);
      if (hasSelectedSyns || hasSelectedSynonyms(clause)) {
        hasSelectedSyns = true;
        totalWeights += clause->getWeights();   
      } else {
        totalWeights += numOfNonSelectedGroups;
      } 
    }

    if (!hasSelectedSyns) {
      numOfNonSelectedGroups++;
    }

    t_withSyns.push(std::make_pair(grp, totalWeights));
  }

  for (auto* group : allGroupsWithSyns) {
    delete group;
  }
}

INTEGER QueryOptimiser::getDynamicWeights(Clause* t_clause) {
  if (t_clause->isRelationType()) {
    Relation *relation = (Relation*)t_clause;
    if (Relation::isFollows(relation->getType())) {
      return m_pkb->getAllFollows().size();
    } else if (Relation::isFollowsStar(relation->getType())) {
      return m_pkb->getAllFollowsStar().size();
    } else if (Relation::isParent(relation->getType())) {
      return m_pkb->getAllParents().size();
    } else if (Relation::isParentStar(relation->getType())) {
      return m_pkb->getAllParentsStar().size();
    } else if (Relation::isUses(relation->getType())) {
      if (Grammar::isProc(relation->getG1().getType()) || Grammar::isString(relation->getG1().getType())) {
        return m_pkb->getUsesPAllProcToVarByIdx().size();
      } else {
        return m_pkb->getAllStmtUsesByIdx().size();
      }  
    } else if (Relation::isModifies(relation->getType())) {
      if (Grammar::isProc(relation->getG1().getType()) || Grammar::isString(relation->getG1().getType())) {
        return m_pkb->getModifiesPAllProcToVarByIdx().size();
      } else {
        return m_pkb->getAllStmtModifiesByIdx().size();
      }
    } else if (Relation::isCalls(relation->getType())) {
      return m_pkb->getAllCallsByIdx().size();
    } else if (Relation::isCallsStar(relation->getType())) {
      return m_pkb->getAllCallsStarByIdx().size();
    } else if (Relation::isNext(relation->getType())) {
      return m_pkb->getAllNext().size();
    } else if (Relation::isNextStar(relation->getType())) {
      return m_pkb->getAllNextStar().size();
    } else if (Relation::isAffects(relation->getType())) {
      return m_pkb->getAllAffects().size();
    } else if (Relation::isAffectsStar(relation->getType())) {
      return m_pkb->getAllAffectsStar().size();
    }
  } else if (t_clause->isPatternType()) {
    Pattern *pattern = (Pattern*)t_clause;
    if (Grammar::isAssign(pattern->getStmt().getType())) {
      return m_pkb->getAllAssignStmts().size();
    } else if (Grammar::isWhile(pattern->getStmt().getType())) {
      return m_pkb->getAllWhileStmts().size();
    } else if (Grammar::isIf(pattern->getStmt().getType())) {
      return m_pkb->getAllIfStmts().size();
    }
  }

  return 0;
}

BOOLEAN QueryOptimiser::hasSelectedSynonyms(Clause* t_clause) {
  if (t_clause->getClauseType() == queryType::clauseType::RELATION) {
    Relation *clause = (Relation*)t_clause;
    for (auto& select : m_selects) {
      if (select.getName() == clause->getG1().getName()
        || select.getName() == clause->getG2().getName()) {
        return true;
      }
    }
  } else if (t_clause->getClauseType() == queryType::clauseType::PATTERN) {
    Pattern *clause = (Pattern*)t_clause;
    for (auto& select : m_selects) {
      if (select.getName() == clause->getStmt().getName()
        || select.getName() == clause->getLeft().getName()) {
        return true;
      }
    }
  } else if (t_clause->getClauseType() == queryType::clauseType::WITH) {
    With *clause = (With*)t_clause;
    for (auto& select : m_selects) {
      if (select.getName() == clause->getG1().getName()
        || select.getName() == clause->getG2().getName()) {
        return true;
      }
    }
  }

  return false;
}