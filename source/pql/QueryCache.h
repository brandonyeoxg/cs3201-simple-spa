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

  /** Gets cached result for Next*(line, l). Requires Next*(l1, l2) to be already cached.
  *   Returns pointer to empty list if no lines after given line.
  *   Returns nullptr if no results cached.
  */
  LIST_OF_PROG_LINES * getAllLinesAfter(PROG_LINE t_line);

  /** Gets cached result for Next(_, l) and Next*(_, l).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_PROG_LINES * getAllLinesAfterAnyLine();

  /** Gets cached result for Next(l, _) and Next*(l, _).
  *   Returns nullptr if no results cached.
  */
  LIST_OF_PROG_LINES * getAllLinesBeforeAnyLine();

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

  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS * getAllAffects();

  ///////////////////////////////////////////////////////
  //  Cache Methods
  ///////////////////////////////////////////////////////

  void cacheAllNext(MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES &t_allNext);

  void cacheAllNextStar(MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES &t_allNextStar);

  void cacheAllLinesAfterAnyLine(LIST_OF_PROG_LINES &t_allLinesAfterAnyLine);

  void cacheAllLinesBeforeAnyLine(LIST_OF_PROG_LINES &t_allLinesBeforeAnyLine);

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

  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES *m_allNext;     /**< Next(l1, l2) */
  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES *m_allNextStar; /**< Next*(l1, l2) */
  LIST_OF_PROG_LINES *m_allLinesAfterAnyLine;            /**< Next(_, l) and Next*(_, l) */
  LIST_OF_PROG_LINES *m_allLinesBeforeAnyLine;           /**< Next(l, _) and Next*(l, _) */

  LIST_OF_STMT_NUMS *m_followsAnything;                  /**< Follows(_, s1) & Follows*(_, s1) */
  LIST_OF_STMT_NUMS *m_followedByAnything;               /**< Follows(s1, _) & Follows*(s1, _) */

  LIST_OF_STMT_NUMS *m_childrenOfAnything;               /**< Parent(_, s1) */
  LIST_OF_STMT_NUMS *m_parentOfAnything;                 /**< Parent(s1, _) */
  LIST_OF_STMT_NUMS *m_childrenStarOfAnything;           /**< Parent*(_, s1) */
  LIST_OF_STMT_NUMS *m_parentStarOfAnything;             /**< Parent*(s1, _) */

  LIST_OF_STMT_NUMS *m_stmtUsesAnything;                 /**< Uses(s1, _) */
  LIST_OF_STMT_NUMS *m_stmtModifiesAnything;             /**< Modifies(s1, _) */

  MAP_OF_STMT_NUM_TO_VAR_INDEX *m_allWhileStmtsWithVar;  /**< Pattern w(v, _) */
  MAP_OF_STMT_NUM_TO_VAR_INDEX *m_allIfStmtsWithVar;     /**< Pattern ifs(v, _) */

  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS *m_allAffects;    /**< Affects(a1, a2) */


  template <typename T, typename G>
  bool isKeyInMap(T key, std::unordered_map<T, G> map);
};

template <typename T, typename G>
inline bool QueryCache::isKeyInMap(T key, std::unordered_map<T, G> map) {
  return map.count(key) == 1;
}