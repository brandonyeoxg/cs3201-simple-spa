#pragma once

#include <assert.h>

#include "../GlobalTypeDef.h"
#include "../Clause.h"
#include "../Pattern.h"
#include "../Relation.h"
#include "../QueryUtil.h"

/** 
*   Class for caching results intra-query.
*
*   Currently caches results for: 
*     Relationships involving 2 common synonyms
*     Relationships involving 1 common synonym and 1 wildcard
*     Relationships involving 1 common synonym and 1 given parameter (number or string)
*     Patterns involving 1 common synonym
*
*   Caches results that PKB takes more than O(1) time to deliver.
*   
*   Can return result from another cached clause if given clause has no cached result. 
*   Refer to getCacheFromOtherClauses().
*
*   @author jazlyn
*/

/** List of cacheable Clauses:

    NOTE: 0 represents a given input for that parameter. i.e. Next(0, l1) indicates Next(3, l1), Next(500, l1) etc.

    Next(p1, p2)
    Next*(p1, p2)
    Next(_, p2) and Next*(_, p2) [has optimization]
    Next(p1, _) and Next*(p1, _) [has optimization]
    Next(0, p2) [has optimization]
    Next*(0, p2) [has optimization]
    Next(p1, 0)
    Next*(p1, 0)

    Calls(proc1, proc2)
    Calls*(proc1, proc2)
    Calls(proc1, _)
    Calls*(proc1, _)
    Calls(_, proc2)
    Calls*(_, proc2)
    Calls(proc1, 0)
    Calls*(proc1, 0)
    Calls(0, proc2)
    Calls*(0, proc2)

    Follows(_, s2) & Follows*(_, s2) [has optimization]
    Follows(s1, _) & Follows*(s1, _) [has optimization]
    Follows(0, s2) [has optimization]
    Follows*(0, s2) [has optimization]
    Follows(s1, 0)
    Follows*(s1, 0)

    Parent(_, s1)
    Parent(s1, _)
    Parent*(_, s1)
    Parent*(s1, _)
    Parent(0, s1) [has optimization]
    Parent(s1, 0)
    Parent*(0, s1) [has optimization]
    Parent*(s1, 0)

    Affects(a1, a2)
    Affects*(a1, a2)
    Affects(a1, _)
    Affects(_, a2)
    Affects*(a1, _)
    Affects*(_, a2)
    Affects(a1, 0)
    Affects(0, a2) [has optimization]
    Affects*(a1, 0)
    Affects*(0, a2) [has optimization]

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

  /** Converts a Clause object of Pattern or Relation type to a string to be used as a key.
  *   Clause can only be of Pattern or Relation type.
  *
  *   Key design: 
  *   [Relation]/[_/s/s1]<type>/[_/s/s1]<type>
  *     i.e. Next(s1, s2) would mean "Next/s1<progLine>/s2<progLine>"
  *   Pattern_[While/If/Assign]
  *     i.e. pattern w(v, _) would mean "Pattern_While"
  *
  *   @param t_clause clause
  *   @return key representing the clause
  */
  std::string getKey(Clause &t_clause);

  ///////////////// for debugging
  //std::unordered_map<std::string, SET_OF_RESULTS> getEntireCache() { return m_cache; }

private:
  std::unordered_map<std::string, SET_OF_RESULTS_INDICES> m_cache;  /**< Cache that maps each Clause (in string form) to its set of results */
  
  ////////// Keys for Clauses in the Cache map /////////////////
  //  To be used for optimizing cache retrieval
  const std::string KEY_ALL_NEXT_STAR = "Next*/s1<progLine>/s2<progLine>";
  const std::string KEY_ALL_NEXT = "Next/s1<progLine>/s2<progLine>";
  const std::string KEY_ALL_NEXT_STAR_STMT = "Next*/s1<stmt>/s2<stmt>";
  const std::string KEY_ALL_NEXT_STMT = "Next/s1<stmt>/s2<stmt>";
  const std::string KEY_ALL_AFFECTS_STAR = "Affects*/s1<stmt>/s2<stmt>";
  const std::string KEY_ALL_AFFECTS = "Affects/s1<stmt>/s2<stmt>";
  const std::string KEY_ALL_FOLLOWS_STAR = "Follows*/s1<stmt>/s2<stmt>";
  const std::string KEY_ALL_FOLLOWS = "Follows/s1<stmt>/s2<stmt>";
  const std::string KEY_ALL_PARENT_STAR = "Parent*/s1<stmt>/s2<stmt>";
  const std::string KEY_ALL_PARENT = "Parent/s1<stmt>/s2<stmt>";

  const std::string KEY_NEXT_STAR = "Next*";
  const std::string KEY_NEXT = "Next";
  const std::string KEY_FOLLOWS = "Follows";
  const std::string KEY_FOLLOWS_STAR = "Follows*";

  bool isPatternCacheable(Pattern *t_pattern);
  bool isRelationCacheable(Relation *t_relation);

  std::string getKeyWithPattern(Pattern t_pattern);
  std::string getKeyWithRelation(Relation t_relation);

  // for clauses with only 1 common synonym
  std::string getKeyWithGrammar(Grammar t_grammar);

  // for clauses with 2 common synonyms
  std::string getKeyWithPairGrammar(Grammar t_grammar1, Grammar t_grammar2);

  std::string getTypeWithSynonym(Grammar t_grammar);

  // optimization method to check if result for given clause can be extracted from other cached clauses
  SET_OF_RESULTS_INDICES *getCacheFromOtherClauses(Clause *t_clause);
  SET_OF_RESULTS_INDICES *getCacheFromOtherRelations(Relation *t_relation);
  SET_OF_RESULTS_INDICES *getCacheForNextStar(Relation *t_relation);
  SET_OF_RESULTS_INDICES *getCacheForNext(Relation *t_relation);
  SET_OF_RESULTS_INDICES *getCacheForFollows(Relation *t_relation);
  SET_OF_RESULTS_INDICES *getCacheForFollowsStar(Relation *t_relation);
  SET_OF_RESULTS_INDICES *getCacheForAffects(Relation *t_relation);
  SET_OF_RESULTS_INDICES *getCacheForAffectsStar(Relation *t_relation);
  SET_OF_RESULTS_INDICES *getCacheForParent(Relation *t_relation);
  SET_OF_RESULTS_INDICES *getCacheForParentStar(Relation *t_relation);

  // for relationships with (given_parameter, common synonym)
  // extract result from cached (common synonym, common synonym) if present
  SET_OF_RESULTS_INDICES *getResultFromTwoSynonyms(Relation *t_relation, std::string t_key);

  template <typename T, typename G>
  bool isKeyInMap(T key, std::unordered_map<T, G> map);
};

template <typename T, typename G>
inline bool QueryCache::isKeyInMap(T key, std::unordered_map<T, G> map) {
  return map.count(key) == 1;
}