#pragma once

#include <assert.h>

#include ".\GlobalTypeDef.h"

/** Class to represent Next relationship, Next* relationship
*   @author jazlyn
*
*/
class NextTable {
public:

  NextTable();

  /**
  * Establish relationship Next(line1, line2).
  * @param t_line1
  * @param t_line2
  */
  void insertNextRelationship(PROG_LINE t_line1, PROG_LINE t_line2);

  void executeAfterAllNextInserts();

  /** Next(line1, line2) */
  bool isNext(PROG_LINE t_line1, PROG_LINE t_line2);

  /** Next*(line1, line2) */
  bool isNextStar(PROG_LINE t_line1, PROG_LINE t_line2);

  /** Next(line, l) */
  std::vector<PROG_LINE> getLinesAfter(PROG_LINE t_line);

  /** Next(l, line) */
  std::vector<PROG_LINE> getLinesBefore(PROG_LINE t_line);

  /** Next*(line, l) */
  std::vector<PROG_LINE> getNextStar(PROG_LINE t_line);

  /** Next*(l, line) */
  std::vector<int> getBeforeStar(PROG_LINE t_line);

  /** Next(l1, l2) */
  std::unordered_map<int, int> getAllNext();

  /** Next*(l1, l2) */
  std::unordered_map<int, std::vector<int>> getAllNextStar();

  /** Next(l, _) */
  std::vector<int> getAllLinesAfter();

  /** Next(_, l) */
  std::vector<int> getAllLinesBefore();

  /** Next(_, _) or Next*(_, _) */
  bool hasNextRelationship();

  /** Next(_, line) */
  bool hasLineBefore(PROG_LINE t_line);

  /** Next(line, _) */
  bool hasNextLine(PROG_LINE t_line);

  ////////////////// for debugging
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> getAfterGraph() { return m_afterGraph; }
  PROG_LINE getMaxLines() { return MAX_LINE_NUM; }

private:
  PROG_LINE MAX_LINE_NUM;
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> m_afterGraph;  /**< Graph representation of lines after each program line */
  std::unordered_map<PROG_LINE, std::vector<PROG_LINE>> m_beforeGraph;  /**< Graph representation of lines before each program line */
  std::vector<std::vector<bool>> m_isNextTable; /**< 2D matrix to maintain boolean representation of existence of Next relationship between 2 lines */
  
  template <typename T>
  bool isKeyInMap(std::unordered_map<T, std::vector<T>> map, T key);
  bool isTherePathFromLine1ToLine2(PROG_LINE t_line1, PROG_LINE t_line2);

};

template<typename T>
inline bool NextTable::isKeyInMap(std::unordered_map<T, std::vector<T>> map, T key) {
  return map.count(key) == 1;
}
