#include "AffectsTable.h"


BOOLEAN AffectsTable::hasAnyAffects() {
  return false;
}

BOOLEAN AffectsTable::hasAffectsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  return false;
}

PAIR_OF_AFFECTS_LIST AffectsTable::getAffectsListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  // Checks if the start point type
  const std::map<PROG_LINE, std::vector<PROG_LINE>> *readOnlyCFG = m_nextTable->getAfterGraph();
  // Check if the startBound is a container stmt
  traverseCfg(t_startBound, t_endBound);
  return {};
}

void AffectsTable::traverseCfg(PROG_LINE m_curProgLine, PROG_LINE m_endBound) {
  // Checks for the current line if it is a container stmt
  queryType::GType stmtType = m_stmtTable->getTypeOfStatement(m_curProgLine);
  if (isContainerStmt(stmtType)) {
    // Get the 2 stmts that leads to the container stmts
    LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(m_curProgLine);
  }
}

BOOLEAN AffectsTable::isContainerStmt(queryType::GType t_type) {
  return t_type == queryType::GType::IF || t_type == queryType::GType::WHILE;
}