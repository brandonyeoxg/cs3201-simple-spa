#pragma once

#include <assert.h>

#include "../GlobalTypeDef.h"
#include "../Clause.h"
#include "../Pattern.h"
#include "../Relation.h"
#include "../QueryUtil.h"

/** Class for caching results intra-query.
*   Currently caches results for Relationships involving 2 common synonyms,
*   or 1 common synonym and 1 wild card. 
*   Caches results that PKB takes more than O(1) time to deliver.
*   @author jazlyn
*/

class QueryCache {
public:
  QueryCache();

  SET_OF_RESULTS getCache(Clause t_clause);

  void cache(Clause t_clause, SET_OF_RESULTS t_results);

  bool isCacheable(Clause *t_clause);

  

  std::string getKey(Clause &t_clause);

private:
  std::unordered_map<std::string, SET_OF_RESULTS> m_cache;

  bool isPatternCacheable(Pattern *t_pattern);
  bool isRelationCacheable(Relation *t_relation);

  
  std::string getKeyWithPattern(Pattern t_pattern);
  std::string getKeyWithRelation(Relation t_relation);
  std::string getKeyWithGrammar(Grammar t_grammar);

  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES *m_allNext = nullptr;      /**< Next(l1, l2) */
  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES *m_allNextStar = nullptr;  /**< Next*(l1, l2) */
  LIST_OF_PROG_LINES *m_allLinesAfterAnyLine = nullptr;             /**< Next(_, l) and Next*(_, l) */
  LIST_OF_PROG_LINES *m_allLinesBeforeAnyLine = nullptr;            /**< Next(l, _) and Next*(l, _) */

  MAP_OF_PROC_NAMES *m_allCalls = nullptr;                          /**< Calls(proc1, proc2) */
  LIST_OF_PROC_NAMES *m_callsAnything = nullptr;                    /**< Calls(proc1, _) */
  LIST_OF_PROC_NAMES *m_callsStarAnything = nullptr;                /**< Calls*(proc1, _) */
  LIST_OF_PROC_NAMES *m_calledByAnything = nullptr;                 /**< Calls(_, proc1) */
  LIST_OF_PROC_NAMES *m_calledByStarAnything = nullptr;             /**< Calls*(_, proc1) */

  LIST_OF_STMT_NUMS *m_followsAnything = nullptr;                   /**< Follows(_, s1) & Follows*(_, s1) */
  LIST_OF_STMT_NUMS *m_followedByAnything = nullptr;                /**< Follows(s1, _) & Follows*(s1, _) */

  LIST_OF_STMT_NUMS *m_childrenOfAnything = nullptr;                /**< Parent(_, s1) */
  LIST_OF_STMT_NUMS *m_parentOfAnything = nullptr;                  /**< Parent(s1, _) */
  LIST_OF_STMT_NUMS *m_childrenStarOfAnything = nullptr;            /**< Parent*(_, s1) */
  LIST_OF_STMT_NUMS *m_parentStarOfAnything = nullptr;              /**< Parent*(s1, _) */

  LIST_OF_STMT_NUMS *m_stmtUsesAnything = nullptr;                  /**< Uses(s1, _) */
  LIST_OF_STMT_NUMS *m_stmtModifiesAnything = nullptr;              /**< Modifies(s1, _) */

  MAP_OF_STMT_NUM_TO_VAR_INDEX *m_allWhileStmtsWithVar = nullptr;   /**< Pattern w(v, _) */
  MAP_OF_STMT_NUM_TO_VAR_INDEX *m_allIfStmtsWithVar = nullptr;      /**< Pattern ifs(v, _) */

  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS *m_allAffects = nullptr;     /**< Affects(a1, a2) */


  template <typename T, typename G>
  bool isKeyInMap(T key, std::unordered_map<T, G> map);
};

template <typename T, typename G>
inline bool QueryCache::isKeyInMap(T key, std::unordered_map<T, G> map) {
  return map.count(key) == 1;
}