#pragma once

#include <assert.h>

#include "../GlobalTypeDef.h"

/** Class for caching results intra-query.
*   Currently caches results for Relationships involving 2 common synonyms,
*   or 1 common synonym and 1 wild card.
*   @author jazlyn
*/

class QueryCache {
public:
  QueryCache();
  ~QueryCache();

  ///////////////////////////////////////////////////////
  //  Getter Methods
  ///////////////////////////////////////////////////////

  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES * getAllNext();

  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES * getAllNextStar();

  LIST_OF_PROG_LINES * getAllLinesAfterAnyLine();

  LIST_OF_PROG_LINES * getAllLinesBeforeAnyLine();

  LIST_OF_STMT_NUMS * getFollowsAnything();

  LIST_OF_STMT_NUMS * getFollowedByAnything();

  LIST_OF_STMT_NUMS * getChildrenOfAnything();

  LIST_OF_STMT_NUMS * getParentOfAnything();

  LIST_OF_STMT_NUMS * getChildrenStarOfAnything();

  LIST_OF_STMT_NUMS * getParentStarOfAnything();

  LIST_OF_STMT_NUMS * getStmtUsesAnything();

  LIST_OF_STMT_NUMS * getStmtModifiesAnything();

  MAP_OF_STMT_NUM_TO_VAR_INDEX * getAllWhileStmtsWithVar();

  MAP_OF_STMT_NUM_TO_VAR_INDEX * getAllIfStmtsWithVar();

  ///////////////////////////////////////////////////////
  //  Cache Methods
  ///////////////////////////////////////////////////////

  void cacheAllNext(MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES t_allNext);

  void cacheAllNextStar(MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES t_allNextStar);

  void cacheAllLinesAfterAnyLine(LIST_OF_STMT_NUMS t_allLinesAfterAnyLine);

  void cacheAllLinesBeforeAnyLine(LIST_OF_STMT_NUMS t_allLinesBeforeAnyLine);

  void cacheFollowsAnything(LIST_OF_STMT_NUMS t_followsAnything);

  void cacheFollowedByAnything(LIST_OF_STMT_NUMS t_followedByAnything);

  void cacheChildrenOfAnything(LIST_OF_STMT_NUMS t_childrenOfAnything);

  void cacheParentOfAnything(LIST_OF_STMT_NUMS t_parentOfAnything);

  void cacheChildrenStarOfAnything(LIST_OF_STMT_NUMS t_childrenStarOfAnything);

  void cacheParentStarOfAnything(LIST_OF_STMT_NUMS t_parentStarOfAnything);


private:

  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES *m_allNext;     /**< Next(l1, l2) */
  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES *m_allNextStar; /**< Next*(l1, l2) */
  LIST_OF_STMT_NUMS *m_allLinesAfterAnyLine;             /**< Next(_, l) and Next*(_, l) */
  LIST_OF_STMT_NUMS *m_allLinesBeforeAnyLine;            /**< Next(l, _) and Next*(l, _) */

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

  // affects
};