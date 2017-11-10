#pragma once

#include <assert.h>

#include "../GlobalTypeDef.h"
#include "../Clause.h"
#include "../Pattern.h"
#include "../Relation.h"
#include "../QueryUtil.h"

/** Class for caching results intra-query.
*   Currently caches results for: 
*     Relationships involving 2 common synonyms
*     Relationships involving 1 common synonym and 1 wildcard
*     Patterns involving 1 common synonym
*   Caches results that PKB takes more than O(1) time to deliver.
*   @author jazlyn
*/

/** List of cacheable Clauses:

    NOTE: 0 represents a given input for that parameter. i.e. Next(0, l1) indicates Next(3, l1), Next(500, l1) etc.

    Next(p1, p2)
    Next*(p1, p2)
    Next(_, p2) and Next*(_, p2)
    Next(p1, _) and Next*(p1, _)
    Next(0, p2)
    Next*(0, p2)
    Next(p1, 0)
    Next*(p1, 0)

    Calls(proc1, proc2)
    Calls(proc1, _)
    Calls*(proc1, _)
    Calls(_, proc2)
    Calls*(_, proc2)
    Calls(proc1, 0)
    Calls*(proc1, 0)
    Calls(0, proc2)
    Calls*(0, proc2)

    Follows(_, s1) & Follows*(_, s1)
    Follows(s1, _) & Follows*(s1, _)
    Follows(0, s1)
    Follows*(0, s1)
    Follows(s1, 0)
    Follows*(s1, 0)

    Parent(_, s1)
    Parent(s1, _)
    Parent*(_, s1)
    Parent*(s1, _)
    Parent(0, s1)
    Parent(s1, 0)
    Parent*(0, s1)
    Parent*(s1, 0)

    Affects(a1, a2)
    Affects*(a1, a2)
    Affects(a1, _)
    Affects(_, a2)
    Affects*(a1, _)
    Affects*(_, a2)
    Affects(a1, 0)
    Affects(0, a2)
    Affects*(a1, 0)
    Affects*(0, a2)

    Uses(s1, _)
    Modifies(s1, _)
    Uses(s1, v)
    Modifies(s1, v)
    Uses(s1, 0)
    Modifies(s1, 0)

    Pattern w(v, _) - while statements
    Pattern ifs(v, _, _) - if statements
    Pattern a(v, _) - assignment statements
*/

class QueryCache {
public:
  QueryCache();

  /** Gets cached result with given clause.
  *   Given clause must be cacheable (will be checked with assert), use isCacheable() to check.
  *   @param t_clause clause, assumes it is a cacheable clause
  *   @return pointer to set of results, will return nullptr if no results cached
  */
  SET_OF_RESULTS_INDICES *getCache(Clause *t_clause);

  /** Caches given result with given clause.
  *   Given clause must be cacheable (will be checked with assert), use isCacheable() to check.
  *   @param t_clause clause, assumes it is a cacheable clause
  *   @param t_results set of results
  */
  void cache(Clause *t_clause, SET_OF_RESULTS_INDICES t_results);

  /** Checks if given clause is cacheable.
  *   @param t_clause clause
  *   @return true if clause is cacheable, else false
  */
  bool isCacheable(Clause *t_clause);

  ////////// exposed for debugging only
  // Converts a Clause object of Pattern or Relation type to a string to be used as a key
  std::string getKey(Clause &t_clause);

  ///////////////// for debugging
  //std::unordered_map<std::string, SET_OF_RESULTS> getEntireCache() { return m_cache; }

private:
  std::unordered_map<std::string, SET_OF_RESULTS_INDICES> m_cache;  /**< Cache that maps each Clause (in string form) to its set of results */
  const std::string KEY_ALL_NEXT_STAR = "Next*/s1/s2";  /**< Key for Next*(s1, s2) */

  bool isPatternCacheable(Pattern *t_pattern);
  bool isRelationCacheable(Relation *t_relation);

  std::string getKeyWithPattern(Pattern t_pattern);
  std::string getKeyWithRelation(Relation t_relation);
  std::string getKeyWithGrammar(Grammar t_grammar);
  std::string getKeyWithPairGrammar(Grammar t_grammar1, Grammar t_grammar2);

  // optimization method to check if result for given clause can be extracted from other cached clauses
  SET_OF_RESULTS_INDICES *getCacheFromOtherClauses(Clause *t_clause);
  SET_OF_RESULTS_INDICES *getCacheFromOtherRelations(Relation *t_relation);

  template <typename T, typename G>
  bool isKeyInMap(T key, std::unordered_map<T, G> map);
};

template <typename T, typename G>
inline bool QueryCache::isKeyInMap(T key, std::unordered_map<T, G> map) {
  return map.count(key) == 1;
}