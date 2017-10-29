#include "AffectsTable.h"


BOOLEAN AffectsTable::hasAnyAffects() {
  return false;
}

BOOLEAN AffectsTable::hasAffectsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  return false;
}

PAIR_OF_AFFECTS_LIST AffectsTable::getAffectsListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  // Checks if the start point type
  const std::map<PROG_LINE, std::vector<PROG_LINE>> *readOnlyCFG = m_pkbTablesOnly->getNextTable()->getAfterGraph();
  auto cItnextLines  = readOnlyCFG->at(t_startBound);
  std::vector<PROG_LINE> stmts = readOnlyCFG->at(t_startBound);
  return {};
}