#pragma once

#include "QueryOptimiser.h"

void QueryOptimiser::divideClausesIntoGroups(std::priority_queue<Clause*> &t_noSyns, std::priority_queue<std::priority_queue<Clause*>*> &t_withSyns) {
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
  
  std::vector<Clause*> *newGroup = new std::vector<Clause*>();
  newGroup->push_back(syns.front());
  allGroupsWithSyns.push_back(newGroup);
  syns.pop();

  int numOfGroups = allGroupsWithSyns.size();
  for (int i = 0; i < numOfGroups; ++i) {
    //std::vector<Clause*> existingGroup = *allGroupsWithSyns[i];
    int numOfClauses = (*allGroupsWithSyns[i]).size();
    for (int j = 0; j < numOfClauses; ++j) {
      Clause *existingClause = (*allGroupsWithSyns[i])[j];
      int numOfSyns = syns.size();
      for (int k = 0; k < numOfSyns; ++k) {
        bool hasCommon = false;
        Clause *clause = syns.front();
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
      newGroup->push_back(syns.front());
      allGroupsWithSyns.push_back(newGroup);
      numOfGroups++;
      syns.pop();
    }  
  }

  for (auto& group : allGroupsWithSyns) {
    std::priority_queue<Clause*> *grp = new std::priority_queue<Clause*>();
    for (auto& clause : *group) {
      grp->push(clause);
    }

    t_withSyns.push(grp);
  }

  for (auto* group : allGroupsWithSyns) {
    delete group;
  }
}

void QueryOptimiser::sortBetweenGroups(std::priority_queue<std::priority_queue<Clause*>*> &t_groups) {
  /*int numOfGroups = t_groups.size();
  for (int grp = 0; grp < numOfGroups; ++grp) {
    std::priority_queue<Clause*> *existingGroup = t_groups.top;
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

void QueryOptimiser::sortWithinGroups(std::priority_queue<Clause*> &t_clauses) {

}