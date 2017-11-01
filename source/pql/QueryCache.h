#pragma once

#include <assert.h>

#include "../GlobalTypeDef.h"

/** Class for caching results intra-query.
*   Currently caches results for Relationships involving 2 common synonyms,
*   or 1 common synonym and 1 wild card. 
*   Caches results that PKB takes more than O(1) time to deliver.
*   @author jazlyn
*/

class QueryCache {
public:
  QueryCache();

  ///////////////////////////////////////////////////////
  //  Getter Methods
  ///////////////////////////////////////////////////////

  /** Gets cached result for Next(l1, l2). 
  *   Returns nullptr if no results cached.
  */
  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES * getAllNext();

  /** Gets cached result for Next*(l1, l2).
  *   Returns nullptr if no results cached.
  */
  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES * getAllNextStar();

  /** Gets cached result for Next(_, l) and Next*(_, l).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_PROG_LINES * getAllLinesAfterAnyLine();

  /** Gets cached result for Next(l, _) and Next*(l, _).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_PROG_LINES * getAllLinesBeforeAnyLine();

  /** Gets cached result for Calls(proc1, proc2).
  *   Returns nullptr if no results cached.
  */
  MAP_OF_PROC_NAMES * getAllCalls();

  /** Gets cached result for Calls(proc1, _).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_PROC_NAMES * getCallsAnything();

  /** Gets cached result for Calls*(proc1, _).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_PROC_NAMES * getCallsStarAnything();
  
  /** Gets cached result for Calls(_, proc1).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_PROC_NAMES * getCalledByAnything();
  
  /** Gets cached result for Calls*(_, proc1).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_PROC_NAMES * getCalledByStarAnything();

  /** Gets cached result for Follows(_, s1) & Follows*(_, s1).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_STMT_NUMS * getFollowsAnything();

  /** Gets cached result for Follows(s1, _) & Follows*(s1, _).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_STMT_NUMS * getFollowedByAnything();

  /** Gets cached result for Parent(_, s1).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_STMT_NUMS * getChildrenOfAnything();

  /** Gets cached result for Parent(s1, _).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_STMT_NUMS * getParentOfAnything();

  /** Gets cached result for Parent*(_, s1).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_STMT_NUMS * getChildrenStarOfAnything();

  /** Gets cached result for Parent*(s1, _).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_STMT_NUMS * getParentStarOfAnything();

  /** Gets cached result for Uses(s1, _).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_STMT_NUMS * getStmtUsesAnything();

  /** Gets cached result for Modifies(s1, _).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_STMT_NUMS * getStmtModifiesAnything();

  /** Gets cached result for Pattern w(v, _).
  *   Returns nullptr if no results cached.
  */
  MAP_OF_STMT_NUM_TO_VAR_INDEX * getAllWhileStmtsWithVar();

  /** Gets cached result for Pattern ifs(v, _).
  *   Returns nullptr if no results cached.
  */
  MAP_OF_STMT_NUM_TO_VAR_INDEX * getAllIfStmtsWithVar();

  /** Gets cached result for Affects(a1, a2).
  *   Returns nullptr if no results cached.
  */
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS * getAllAffects();

  ///////////////////////////////////////////////////////
  //  Getter Methods With Parameter
  ///////////////////////////////////////////////////////

  /** Gets cached result for Next*(line, l). Requires Next*(l1, l2) to be already cached.
  *   Returns pointer to empty list if no lines after given line.
  *   Returns nullptr if Next*(l1, l2) is not cached.
  */
  LIST_OF_PROG_LINES * getAllLinesAfter(PROG_LINE t_line);

  ///////////////////////////////////////////////////////
  //  Cache Methods
  ///////////////////////////////////////////////////////

  void cacheAllNext(MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES &t_allNext);

  void cacheAllNextStar(MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES &t_allNextStar);

  void cacheAllLinesAfterAnyLine(LIST_OF_PROG_LINES &t_allLinesAfterAnyLine);

  void cacheAllLinesBeforeAnyLine(LIST_OF_PROG_LINES &t_allLinesBeforeAnyLine);

  void cacheAllCalls(MAP_OF_PROC_NAMES &t_allCalls);

  void cacheCallsAnything(LIST_OF_PROC_NAMES &t_callsAnything);

  void cacheCallsStarAnything(LIST_OF_PROC_NAMES &t_callsStarAnything);

  void cacheCalledByAnything(LIST_OF_PROC_NAMES &t_calledByAnything);

  void cacheCalledByStarAnything(LIST_OF_PROC_NAMES &t_calledByStarAnything);

  void cacheFollowsAnything(LIST_OF_STMT_NUMS &t_followsAnything);

  void cacheFollowedByAnything(LIST_OF_STMT_NUMS &t_followedByAnything);

  void cacheChildrenOfAnything(LIST_OF_STMT_NUMS &t_childrenOfAnything);

  void cacheParentOfAnything(LIST_OF_STMT_NUMS &t_parentOfAnything);

  void cacheChildrenStarOfAnything(LIST_OF_STMT_NUMS &t_childrenStarOfAnything);

  void cacheParentStarOfAnything(LIST_OF_STMT_NUMS &t_parentStarOfAnything);

  void cacheStmtUsesAnything(LIST_OF_STMT_NUMS &t_stmtUsesAnything);

  void cacheStmtModifiesAnything(LIST_OF_STMT_NUMS &t_stmtModifiesAnything);

  void cacheAllWhileStmtsWithVar(MAP_OF_STMT_NUM_TO_VAR_INDEX &t_allWhileStmtsWithVar);

  void cacheAllIfStmtsWithVar(MAP_OF_STMT_NUM_TO_VAR_INDEX &t_allIfStmtsWithVar);

  void cacheAllAffects(MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS &t_allAffects);

private:

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