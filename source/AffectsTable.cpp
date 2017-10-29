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
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS LMS;
  traverseCfg(t_startBound, t_endBound, LMS);
  return {};
}

void AffectsTable::traverseCfg(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS t_lmt) {
  if (t_curProgLine > t_endBound) {
    return;
  }
  // Checks for the current line if it is a container stmt
  queryType::GType stmtType = m_stmtTable->getTypeOfStatement(t_curProgLine);
  if (isContainerStmt(stmtType)) {
    traverseContainerCfg(t_curProgLine, t_endBound, t_lmt, stmtType);
  } else {
    traverseNonContainerCfg(t_curProgLine, t_endBound, t_lmt, stmtType);
  }
}

void AffectsTable::traverseContainerCfg(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS t_lmt, queryType::GType t_type) {
  // Get the 2 stmts that leads to the container stmts
  if (t_type == queryType::GType::IF) {
    LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
    MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS ifLmt;
    LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
    traverseCfg(stmts[0], stmtLstBound.back(), ifLmt);
    MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS elseLmt;
    traverseCfg(stmts[1], stmtLstBound.back(), elseLmt);
    // For if both stmts leads to stmt lst

  } else if (t_type == queryType::GType::WHILE) {
    LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
    MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS t_stmtLstLMT;
    // Get the stmts lst of both then and else portion
    MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS insideStmtLst = t_stmtLstLMT;
    traverseCfg(stmts[0], t_endBound, insideStmtLst);
    traverseCfg(stmts[0], t_endBound, insideStmtLst);
    // For while one stmt leads to stmt lst the other stmt if any leads to the next stmt in the cur stmt lst.
    if (stmts.size() > 1) {
      // Combine lms with lmt
      traverseCfg(stmts[1], t_endBound, t_lmt);
    }
  }
}

void AffectsTable::traverseNonContainerCfg(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS t_lmt, queryType::GType t_type) {
  if (t_type == queryType::GType::ASGN) {
    // checks the lmt to see is any affects relation
    if (t_lmt.empty()) {
      LIST_OF_VAR_NAMES modifiesVar = m_pkbTablesOnly->getModifiesTable()->getModifies(t_curProgLine);
      t_lmt.insert({ modifiesVar[0], {t_curProgLine} });
    }
  } else if (t_type == queryType::GType::CALL) {

  }
  if (m_nextTable->getAfterGraph()->find(t_curProgLine) == m_nextTable->getAfterGraph()->end()) {
    return;
  }
  PROG_LINE nextProgLine = m_nextTable->getAfterGraph()->at(t_curProgLine)[0];
  if (nextProgLine < t_curProgLine) {
    return;
  }
  traverseCfg(nextProgLine, t_endBound, t_lmt);
}

BOOLEAN AffectsTable::isContainerStmt(queryType::GType t_type) {
  return t_type == queryType::GType::IF || t_type == queryType::GType::WHILE;
}