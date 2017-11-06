#pragma once

#include "QueryOptimiser.h"

void QueryOptimiser::divideClausesIntoGroups(std::priority_queue<Clause*> &t_noSyns, std::priority_queue<std::priority_queue<Clause*>*> &t_withSyns) {
  int relationSize = m_relations.size();
  int patternSize = m_patterns.size();
  int withSize = m_withs.size();
  std::priority_queue<Clause*> syns;
  std::priority_queue<std::priority_queue<Clause*>*> allGroupsWithSyns;

  for (int i = 0; i < relationSize; ++i) {
    Relation *relPtr = &m_relations.front();
    syns.push(relPtr);
    if (QueryUtil::hasNoSynonyms(relPtr->getG1(), relPtr->getG2())) {
      t_noSyns.push(relPtr);
    } else { 
      syns.push(relPtr);
    }

    m_relations.pop();
  }

  for (int i = 0; i < patternSize; ++i) {
    Pattern *patPtr = &m_patterns.front();
    if (QueryUtil::hasNoSynonyms(patPtr->getLeft(), patPtr->getRight())) {
      t_noSyns.push(patPtr);
    } else {
      syns.push(patPtr);
    }

    m_patterns.pop();
  }

  for (int i = 0; i < withSize; ++i) {
    With *withPtr = &m_withs.front();
    if (QueryUtil::hasNoSynonyms(withPtr->getG1(), withPtr->getG2())) {
      t_noSyns.push(withPtr);
    } else {
      syns.push(withPtr);
    }

    m_withs.pop();
  }

  int synsSize = syns.size();
  for (int i = 0; i < synsSize; ++i) {
    Clause *clause = syns.top();
    int numOfGroups = allGroupsWithSyns.size();
    
    for (int grp = 0; grp < numOfGroups; ++grp) {
      std::priority_queue<Clause*> *existingGroup = allGroupsWithSyns.top();
      int numOfClauses = existingGroup->size();
      bool hasCommon = false;
      for (int cls = 0; cls < numOfClauses; ++cls) {
        Clause *existingClause = existingGroup->top();
        if (existingClause->getClauseType() == queryType::clauseType::RELATION) {
          Relation *existingRelation = (Relation*)existingClause;
          Relation *relation = (Relation*)clause;
          if (relation->getG1().getName() == existingRelation->getG1().getName()
            || relation->getG1().getName() == existingRelation->getG2().getName()
            || relation->getG2().getName() == existingRelation->getG1().getName()
            || relation->getG2().getName() == existingRelation->getG2().getName()) {
            existingGroup->push(clause);
            hasCommon = true;
            break;
          }
        } else if (existingClause->getClauseType() == queryType::clauseType::PATTERN) {
          Pattern *existingPattern = (Pattern*)existingClause;
          Pattern *pattern = (Pattern*)clause;
          if (pattern->getLeft().getName() == existingPattern->getLeft().getName()
            || pattern->getLeft().getName() == existingPattern->getRight().getName()
            || pattern->getRight().getName() == existingPattern->getLeft().getName()
            || pattern->getRight().getName() == existingPattern->getRight().getName()) {
            existingGroup->push(clause);
            hasCommon = true;
            break;
          }
        } else if (existingClause->getClauseType() == queryType::clauseType::WITH) {
          With *existingWith = (With*)existingClause;
          With *with = (With*)clause;
          if (with->getG1().getName() == existingWith->getG1().getName()
            || with->getG1().getName() == existingWith->getG2().getName()
            || with->getG2().getName() == existingWith->getG1().getName()
            || with->getG2().getName() == existingWith->getG2().getName()) {
            existingGroup->push(clause);
            hasCommon = true;
            break;
          }
        }
      }

      allGroupsWithSyns.pop();
      allGroupsWithSyns.push(existingGroup);

      if (hasCommon) {
        break;
      }
    }

    std::priority_queue<Clause*> *newGroup = new std::priority_queue<Clause*>();
    newGroup->push(clause);
    allGroupsWithSyns.push(newGroup);
    syns.pop();
  }

  std::cout << "HELLO";
  /*int numOfGroups = allGroupsWithSyns.size();
  for (int grp = 0; grp < numOfGroups; ++grp) {
    std::priority_queue<Clause*> *existingGroup = allGroupsWithSyns.front();
    int numOfClauses = existingGroup->size();
    bool isSelected = false;
    for (int cls = 0; cls < numOfClauses; ++cls) {
      Clause *existingClause = existingGroup->top();
      if (existingClause->getClauseType() == queryType::clauseType::RELATION) {
        Relation *existingClause = (Relation*)existingClause;
        int numOfSelects = m_selects.size();
        for (int sel = 0; sel < numOfSelects; ++sel) {
          Grammar select = m_selects.front();
          if (select.getName() == existingClause->getG1().getName()
            || select.getName() == existingClause->getG2().getName()) {
            t_hasSelectSyns.push(existingGroup);
            isSelected = true;
            break;
          }

          m_selects.pop();
          m_selects.push(select);
        }
        
        if (isSelected) {
          break;
        }
      }
    }
    
    if (!isSelected) {
      t_noSelectSyns.push(existingGroup);
    }
  }*/
}

void QueryOptimiser::sortBetweenGroups(std::priority_queue<std::priority_queue<Clause*>> &t_groups) {
  int size = t_groups.size();
  for (int grp = 0; grp < size; ++grp) {
    //std::priority_queue<Clause*> group = t_groups.top;
  }
}

void QueryOptimiser::sortWithinGroups(std::priority_queue<Clause*> &t_clauses) {

}