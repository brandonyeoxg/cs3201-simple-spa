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

private:
  static const int MAX_NUM_LINES = 501;

};