#pragma once

#include "QueryOptimiser.h"

void QueryOptimiser::divideClausesIntoGroups(std::priority_queue<Clause> &t_noSyns, std::priority_queue<std::priority_queue<Clause>> &t_noSelectSyns, std::priority_queue<std::priority_queue<Clause>> &t_hasSelectSyns) {
  int relationSize = m_relations.size();
  int patternSize = m_patterns.size();
  int withSize = m_withs.size();

  for (int i = 0; i < relationSize; ++i) {
    /*Relation relation = m_relations.front();
    if (QueryUtil::hasNoSynonyms(relation.getG1(), relation.getG2())) {
      t_noSyns.push(relation);
    } else {
      std::priority_queue<Clause> group;
      group.push(relation);
      t_noSelectSyns.push(group);
    }*/

    m_relations.pop();
  }
}

void QueryOptimiser::sortBetweenGroups(std::priority_queue<std::priority_queue<Clause>> &t_groups) {

}

void QueryOptimiser::sortWithinGroups(std::priority_queue<Clause> &t_clauses) {

}