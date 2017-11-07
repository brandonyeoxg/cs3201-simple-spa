#pragma once

#include <assert.h>

#include "../GlobalTypeDef.h"
#include "../Clause.h"
#include "../Pattern.h"
#include "../Relation.h"
#include "../QueryUtil.h"

/** Class for caching results intra-query.
*   Currently caches results for Relationships involving 2 common synonyms,
*   or 1 common synonym and 1 wildcard. 
*   Caches results that PKB takes more than O(1) time to deliver.
*   @author jazlyn
*/

/** List of cacheable Clauses:

    Next(l1, l2)
    Next*(l1, l2)
    Next(_, l) and Next*(_, l)
    Next(l, _) and Next*(l, _)

    Calls(proc1, proc2)
    Calls(proc1, _)
    Calls*(proc1, _)
    Calls(_, proc1)
    Calls*(_, proc1)

    Follows(_, s1) & Follows*(_, s1)
    Follows(s1, _) & Follows*(s1, _)

    Parent(_, s1)
    Parent(s1, _)
    Parent*(_, s1)
    Parent*(s1, _)

    Affects(a1, a2)
    Affects*(a1, a2)
    Affects(a1, _)
    Affects(_, a2)
    Affects*(a1, _)
    Affects*(_, a2)

    Uses(s1, _)
    Modifies(s1, _)

    Pattern w(v, _) - while statements
    Pattern ifs(v, _, _) - if statements
    Pattern a(v, _) - assignment statements
*/

class QueryCache {
public:
  QueryCache();

  SET_OF_RESULTS *getCache(Clause *t_clause);

  void cache(Clause *t_clause, SET_OF_RESULTS t_results);

  bool isCacheable(Clause *t_clause);

  std::string getKey(Clause &t_clause);

private:
  std::unordered_map<std::string, SET_OF_RESULTS> m_cache;

  bool isPatternCacheable(Pattern *t_pattern);
  bool isRelationCacheable(Relation *t_relation);

  
  std::string getKeyWithPattern(Pattern t_pattern);
  std::string getKeyWithRelation(Relation t_relation);
  std::string getKeyWithGrammar(Grammar t_grammar);

  template <typename T, typename G>
  bool isKeyInMap(T key, std::unordered_map<T, G> map);
};

template <typename T, typename G>
inline bool QueryCache::isKeyInMap(T key, std::unordered_map<T, G> map) {
  return map.count(key) == 1;
}