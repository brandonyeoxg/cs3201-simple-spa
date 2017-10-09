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
  void insertNext(PROG_LINE t_line1, PROG_LINE t_line2);

  void populateNextStarTable();

  bool isNext(PROG_LINE t_line1, PROG_LINE t_line2);

  bool isNextStar(PROG_LINE t_line1, PROG_LINE t_line2);

  PROG_LINE getNextLine(PROG_LINE t_line);

  PROG_LINE getLineBefore(PROG_LINE t_line);

  std::vector<PROG_LINE> getNextStar(PROG_LINE t_line);

  std::vector<int> getBeforeStar(PROG_LINE t_line);

  std::unordered_map<int, int> getAllNext();

  std::unordered_map<int, std::vector<int>> getAllNextStar();

  std::vector<int> getNextAndNextStar();

  std::vector<int> getBeforeAndBeforeStar();

  bool hasNextRelationship();

  bool hasLineBefore(PROG_LINE t_line);

  bool hasNextLine(PROG_LINE t_line);

private:
  static const int MAX_NUM_LINES = 501;
  bool m_nextTable [MAX_NUM_LINES][MAX_NUM_LINES] = { { false } }; // what is the max number of lines?
  bool m_nextStarTable[MAX_NUM_LINES][MAX_NUM_LINES] = { { false } };

};