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
  struct compareClauses {
    bool operator()(Clause* t_clause1, Clause* t_clause2) {
      return t_clause1->getWeights() > t_clause2->getWeights();
    }
  };

  struct compareGroups {
    bool operator()(std::pair<std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses>*, int> t_group1, std::pair<std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses>*, int> t_group2) {
      return t_group1.second > t_group2.second;
    }
  };

  QueryOptimiser(std::vector<Grammar> t_selects, std::vector<Relation> t_relations, std::vector<Pattern> t_patterns, std::vector<With> t_withs)
    : m_selects(t_selects),
      m_relations(t_relations),
      m_patterns(t_patterns),
      m_withs(t_withs) {};

  void divideClausesIntoGroups(std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses> &t_noSyns, std::priority_queue<std::pair<std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses>*, int>, std::vector<std::pair<std::priority_queue<Clause*, std::vector<Clause*>, QueryOptimiser::compareClauses>*, int>>, QueryOptimiser::compareGroups> &t_withSyns);
  BOOLEAN hasSelectedSynonyms(Clause* t_clause);

private:
  std::vector<Grammar> m_selects;
  std::vector<Relation> m_relations;
  std::vector<Pattern> m_patterns;
  std::vector<With> m_withs;
};