#include "NextTable.h"

NextTable::NextTable() {}

void NextTable::insertNext(PROG_LINE t_line1, PROG_LINE t_line2) {
  assert(!m_nextTable[t_line1][t_line2]);
  m_nextTable[t_line1][t_line2] = true;
  m_nextTable[t_line2][t_line1] = true;

  populateNextStarTable();
}

void NextTable::populateNextStarTable() {

  for (int i = 0; i < MAX_NUM_LINES; i++) {
    for (int k = 0; k < MAX_NUM_LINES; k++) {
      m_nextStarTable[i][k] = m_nextTable[i][k];
    }
  }

  for (int i = 0; i < MAX_NUM_LINES; i++) {
    for (int k = 0; k < MAX_NUM_LINES; k++) {
      for (int j = 0; j < MAX_NUM_LINES; j++) {

        if (m_nextStarTable[k][i] && m_nextStarTable[i][j]) {
          m_nextStarTable[k][j] = true;
        }
      }
    }
  }
}

bool NextTable::isNext(PROG_LINE t_line1, PROG_LINE t_line2) {
  return m_nextTable[t_line1][t_line2];
}

bool NextTable::isNextStar(PROG_LINE t_line1, PROG_LINE t_line2) {
  return m_nextStarTable[t_line1][t_line2];
}


