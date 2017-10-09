#pragma once

#include ".\GlobalTypeDef.h"

/** Class to represent Next relationship, Next* relationship
*   @author jazlyn
*
*/
class NextTable {
public:

  NextTable();

  bool insertNext(PROG_LINE t_line1, PROG_LINE t_line2);

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
  std::vector<std::vector<PROG_LINE>> m_nextTable;
};