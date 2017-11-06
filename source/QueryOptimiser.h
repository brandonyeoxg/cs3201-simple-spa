#pragma once

#include <queue>

#include "Clause.h"
#include "Relation.h"
#include "Pattern.h"
#include "With.h"
#include "QueryUtil.h"

class QueryOptimiser {
public:
  QueryOptimiser(std::queue<Grammar> t_selects, std::queue<Relation> t_relations, std::queue<Pattern> t_patterns, std::queue<With> t_withs)
    : m_selects(t_selects),
      m_relations(t_relations),
      m_patterns(t_patterns),
      m_withs(t_withs) {};

  void divideClausesIntoGroups(std::priority_queue<Clause*> &t_noSyns, std::priority_queue<std::priority_queue<Clause*>*> &t_withSyns);
  void sortBetweenGroups(std::priority_queue<std::priority_queue<Clause*>> &t_groups);
  void sortWithinGroups(std::priority_queue<Clause*> &t_clauses);

private:
  std::queue<Grammar> m_selects;
  std::queue<Relation> m_relations;
  std::queue<Pattern> m_patterns;
  std::queue<With> m_withs;
};