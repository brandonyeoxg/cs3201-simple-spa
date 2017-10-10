#include "NextTable.h"

NextTable::NextTable() {}

void NextTable::insertNextRelationship(PROG_LINE t_line1, PROG_LINE t_line2) {
  assert(!m_nextTable[t_line1][t_line2]);
  m_nextTable[t_line1][t_line2] = true;
  m_nextTable[t_line2][t_line1] = true;
}

bool NextTable::isNext(PROG_LINE t_line1, PROG_LINE t_line2) {
  return m_nextTable[t_line1][t_line2];
}

bool NextTable::isNextStar(PROG_LINE t_line1, PROG_LINE t_line2) {
  return m_nextStarTable[t_line1][t_line2];
}


