#pragma once

#include <queue>
#include <utility>

#include "Clause.h"
#include "Relation.h"
#include "Pattern.h"
#include "With.h"
#include "QueryUtil.h"
#include "PkbReadOnly.h"

class QueryOptimiser {
public:
  QueryOptimiser(std::vector<Grammar> t_selects, std::vector<Relation> t_relations, std::vector<Pattern> t_patterns, std::vector<With> t_withs)
    : m_selects(t_selects),
      m_relations(t_relations),
      m_patterns(t_patterns),
      m_withs(t_withs) {};

  void divideClausesIntoGroups(std::priority_queue<Clause*> &t_noSyns, std::priority_queue<std::priority_queue<Clause*>*> &t_withSyns);
  void sortBetweenGroups(std::priority_queue<std::priority_queue<Clause*>*> &t_groups);
  void sortWithinGroups(std::priority_queue<Clause*> &t_clauses, PkbReadOnly *t_pkb);

private:
  std::vector<Grammar> m_selects;
  std::vector<Relation> m_relations;
  std::vector<Pattern> m_patterns;
  std::vector<With> m_withs;
};