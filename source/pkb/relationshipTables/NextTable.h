#pragma once

#include <iostream>
#include <assert.h>
#include <algorithm>

#include "../../GlobalTypeDef.h"

/** Class to represent Next relationship, Next* relationship in PKB.
*   Used to evaluate queries with clauses on Next relationships.
*   @author jazlyn
*/
class NextTable {
public:

  NextTable();

  /** Establish relationship Next(line1, line2).
  *   @param t_line1 the program line before
  *   @param t_line2 the program line after
  */
  void insertNextRelationship(PROG_LINE t_line1, PROG_LINE t_line2);

  /** To be executed after all Next relationships are added to NextTable.
  *   Populates additional design abstractions.
  */
  void executeAfterAllNextInserts();

  /** Checks if Next(line1, line2) is true, where line1 and line2 are a given line number each.
  *   @param t_line1 the program line before
  *   @param t_line2 the program line after
  *   @return true if relationship exists, else false
  */
  bool isNext(PROG_LINE t_line1, PROG_LINE t_line2);

  /** Checks if Next*(line1, line2) is true, where line1 and line2 are a given line number each.
  *   @param t_line1 the program line before
  *   @param t_line2 the program line after
  *   @return true if relationship exists, else false
  */
  bool isNextStar(PROG_LINE t_line1, PROG_LINE t_line2);

  /** For Next(line, l) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed directly after given line.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getLinesAfter(PROG_LINE t_line);

  /** For Next(l, line) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed directly before given line.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getLinesBefore(PROG_LINE t_line);

  /** For Next*(line, l) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed after given line, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getAllLinesAfter(PROG_LINE t_line);

  /** For Next*(l, line) where line is a given line number, and l is a common synonym for all lines.
  *   Gets all lines that can be executed before given line, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getAllLinesBefore(PROG_LINE t_line);

  /** For Next(l1, l2) where l1, l2 is a common synonym for all lines.
  *   Gets map of all lines, each with a corresponding list of lines that can be executed directly after it.
  *   @return map of <program line number, list of lines executed after it>
  */
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> getAllNext();

  /** For Next*(l1, l2) where l1, l2 is a common synonym for all lines.
  *   Gets map of all lines, each with a corresponding list of lines that can be executed after it, either directly or in some execution sequence.
  *   @return map of <program line number, list of lines executed after it>
  */
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> getAllNextStar();

  /** For Next(_, l) and Next*(_, l) where l is a common synonym for all lines.
  *   Gets list of all lines that can be executed after any particular line.
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getAllLinesAfterAnyLine();

  /** For Next(l, _) and Next*(l, _) where l is a common synonym for all lines.
  *   Gets list of all lines that can be executed before any particular line.
  *   @return list of program line numbers
  */
  LIST_OF_PROG_LINES getAllLinesBeforeAnyLine();

  /** For Next(_, _) or Next*(_, _).
  *   Checks if any Next relationship exists.
  *   @return true if data structure contains at least one Next(), else false
  */
  bool hasNextRelationship();

  /** For Next(line, _) and Next*(line, _), where line is a given line number.
  *   Checks if given line has any lines that can be executed after it, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return true if given line has at least one line that can be executed after it, else false
  */
  bool hasNextLine(PROG_LINE t_line);

  /** For Next(_, line) and Next*(_, line), where line is a given line number.
  *   Checks if given line has any lines that can be executed before it, either directly or in some execution sequence.
  *   @param t_line given program line
  *   @return true if given line has at least one line that can be executed before it, else false
  */
  bool hasLineBefore(PROG_LINE t_line);

  // Returns a constant pointer to m_afterGraph, for Affects() computation
  const std::map<PROG_LINE, std::vector<PROG_LINE>> *getAfterGraph();

private:
  PROG_LINE MAX_LINE_NUM; /**< Number is used to track the largest program line number in given source program. Used to initialize data structures. */
  std::map<PROG_LINE, std::vector<PROG_LINE>> m_afterGraph;  /**< Graph representation of lines after each program line */
  std::map<PROG_LINE, std::vector<PROG_LINE>> m_beforeGraph;  /**< Graph representation of lines before each program line */
  std::vector<std::vector<bool>> m_isNextTable; /**< 2D matrix to maintain boolean representation of existence of Next relationship between two lines */
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> m_cacheVisited; /**< Cache map for computing Next*() */
  bool isCaching = false; /**< Boolean to switch caching mode on and off, to be used in traverseGraphDfs() */

  /** Checks if a path exists from line1 to line2, using m_afterGraph.
  *   This function is used to help check for Next*(line1, line2) relationship.
  *   Uses depth first search to traverse graph, terminates once line2 is found
  *   @param t_line1 given program line
  *   @param t_line2 given program line
  *   @return true if line2 is reachable from line1, else false
  */
  bool isTherePath(PROG_LINE t_line1, PROG_LINE t_line2);

  /** Gets a list of all lines that can be reached from given line number in given graph.
  *   This function is used to help get lines after or before a particular line.
  *   Uses depth first search to traverse graph.
  *   @param t_line given program line
  *   @param t_graph given graph to search
  *   @return list of the lines (can be in unsorted order)
  */
  LIST_OF_PROG_LINES getListOfLinesReachable(PROG_LINE t_line, std::map<PROG_LINE, std::vector<PROG_LINE>> t_graph);

  // recursive dfs
  LIST_OF_PROG_LINES traverseGraphDfs(PROG_LINE t_line, std::map<PROG_LINE, std::vector<PROG_LINE>> t_graph, std::vector<bool>& visited);

  template <typename T, typename G>
  bool isKeyInMap(T key, std::map<T, G> map);  /**< Function to test if key exists in a map. Uses generics. */
  template <typename T, typename G>
  bool isKeyInMap(T key, std::unordered_map<T, G> map);
};

template <typename T, typename G>
inline bool NextTable::isKeyInMap(T key, std::map<T, G> map) {
  return map.count(key) == 1;
}

template <typename T, typename G>
inline bool NextTable::isKeyInMap(T key, std::unordered_map<T, G> map) {
  return map.count(key) == 1;
}
