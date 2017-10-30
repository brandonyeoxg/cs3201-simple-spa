#pragma once

#include "../GlobalTypeDef.h"

/** Class for caching results intra-query.
*   Currently caches results for Relationships involving 2 common synonyms.
*   @author jazlyn
*/

class QueryCache {
public:
  QueryCache();
  ~QueryCache();

  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES * getAllNextStar();

  MAP_OF_STMT_NUMS  *getAllFollows();

  LIST_OF_STMT_NUMS * getFollowsAnything();

  LIST_OF_STMT_NUMS * getFollowedByAnything();

private:

  MAP_OF_PROG_LINE_TO_LIST_OF_PROG_LINES *m_allNextStar; /**< Next*(l1, l2) */

  MAP_OF_STMT_NUMS  *m_allFollows;                       /**< Follows(s1, s2) */
  LIST_OF_STMT_NUMS *m_followsAnything;                  /**< Follows(_, s1) & Follows*(_, s1) */
  LIST_OF_STMT_NUMS *m_followedByAnything;               /**< Follows(s1, _) & Follows*(s1, _) */

  LIST_OF_STMT_NUMS *m_childrenOfAnything;               /**< Parent(_, s1) */
  LIST_OF_STMT_NUMS *m_parentOfAnything;                 /**< Parent(s1, _) */
  LIST_OF_STMT_NUMS *m_childrenStarOfAnything;           /**< Parent*(_, s1) */
  LIST_OF_STMT_NUMS *m_parentStarOfAnything;             /**< Parent*(s1, _) */

  //m_allWhileStmtsWithVar; /**< Pattern w(v, _) */
  //  m_allIfStmtsWithVar;  /**<  Pattern ifs(v, _) */
};