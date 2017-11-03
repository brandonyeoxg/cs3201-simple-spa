#include "AffectsTable.h"

///////////////////////////////////////////////////////
//  Affects
///////////////////////////////////////////////////////
BOOLEAN AffectsTable::hasAffectsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound, PROG_LINE t_targetStart, PROG_LINE t_targetEnd) {
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms;
  // Checks if they are with in a while stmt
  PROG_LINE realStartBound = getRealStartBound(t_startBound);
  return traverseCfgWithBoundEarlyExit(realStartBound, t_endBound, lms, t_targetStart, t_targetEnd);
}

BOOLEAN AffectsTable::isAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) {
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms;
  PROG_LINE realStartBound = getRealStartBound(t_modifiesLine);
  traverseCfgWithBound(realStartBound, t_usesLine, lms);
  auto aItr = affectsList.find(t_modifiesLine);
  if (aItr == affectsList.end()) {
    return false;
  }
  auto bItr = affectedByList.find(t_usesLine);
  if (bItr == affectedByList.end()) {
    return false;
  }
  auto uItr = aItr->second.find(t_usesLine);
  if (uItr == aItr->second.end()) {
    return false;
  }
  return true;
}

PAIR_OF_AFFECTS_LIST AffectsTable::getAffectsListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  // Checks if the start point type
  const std::map<PROG_LINE, std::vector<PROG_LINE>> *readOnlyCFG = m_nextTable->getAfterGraph();
  // Check if the startBound is a container stmt
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms;
  PROG_LINE realStartBound = getRealStartBound(t_startBound);
  traverseCfgWithBound(realStartBound, t_endBound, lms);
  return {affectsList, affectedByList};
}

void AffectsTable::traverseCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
  if (t_curProgLine > t_endBound) {
    return;
  }
  // Checks for the current line if it is a container stmt
  queryType::GType stmtType = m_stmtTable->getTypeOfStatement(t_curProgLine);
  if (isContainerStmt(stmtType)) {
    traverseContainerCfgWithBound(t_curProgLine, t_endBound, t_lmt, stmtType);
  } else {
    traverseNonContainerCfgWithBound(t_curProgLine, t_endBound, t_lmt, stmtType);
  }
}

void AffectsTable::traverseContainerCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type) {
  // Get the 2 stmts that leads to the container stmts
  if (t_type == queryType::GType::IF) {
    traverseIfStmtWithBound(t_curProgLine, t_endBound, t_lmt);
  } else if (t_type == queryType::GType::WHILE) {
    traverseWhileStmtWithBound(t_curProgLine, t_endBound, t_lmt);
  }
}

void AffectsTable::traverseNonContainerCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type) {
  if (t_type == queryType::GType::ASGN) {
    handleAffectsOnAssgnStmt(t_curProgLine, t_lmt);
  } else if (t_type == queryType::GType::CALL) {
    handleAffectsOnCallStmt(t_curProgLine, t_lmt);
  }
  if (m_nextTable->getAfterGraph()->find(t_curProgLine) == m_nextTable->getAfterGraph()->end()) {
    return;
  }
  PROG_LINE nextProgLine = m_nextTable->getAfterGraph()->at(t_curProgLine)[0];
  if (nextProgLine < t_curProgLine) {
    return;
  }
  traverseCfgWithBound(nextProgLine, t_endBound, t_lmt);
}

void AffectsTable::traverseIfStmtWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS ifLmt = t_lmt;
  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
  traverseCfgWithBound(stmts[0], stmtLstBound.back(), ifLmt);
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS elseLmt = t_lmt;
  traverseCfgWithBound(stmts[1], stmtLstBound.back(), elseLmt);
  // For if both stmts leads to stmt lst
  auto nItr = m_nextTable->getAfterGraph()->find(stmtLstBound.back());
  queryType::GType stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(stmtLstBound.back());
  if (stmtType == queryType::GType::WHILE && nItr->second.size() < 2) {
    return;
  }
  if (nItr == m_nextTable->getAfterGraph()->end()) {
    return;
  }
  t_lmt = mergeLmt(ifLmt, elseLmt);
  PROG_LINE nextStmt = nItr->second.back();
  stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(nextStmt);
  if (stmtType == queryType::GType::WHILE) {
    return;
  }
  // Combine lms with lmt
  traverseCfgWithBound(nextStmt, t_endBound, t_lmt);
}

void AffectsTable::traverseWhileStmtWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
  // Get the stmts lst of both then and else portion
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS insideStmtLst = t_lmt;
  traverseCfgWithBound(stmts[0], stmtLstBound.back(), insideStmtLst);
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS mergedLst = mergeLmt(insideStmtLst, t_lmt);
  traverseCfgWithBound(stmts[0], stmtLstBound.back(), mergedLst);
  // For while one stmt leads to stmt lst the other stmt if any leads to the next stmt in the cur stmt lst.
  if (stmts.size() > 1) {
    queryType::GType stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(stmts[1]);
    if (stmtType == queryType::GType::WHILE) {
      return;
    }
    // Combine lms with lmt
    MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS nextMergedLst = mergeLmt(t_lmt, mergedLst);
    traverseCfgWithBound(stmts[1], t_endBound, nextMergedLst);
    t_lmt = nextMergedLst;
    return;
  }
  t_lmt = mergedLst;
}

void AffectsTable::handleAffectsOnAssgnStmt(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
  VAR_NAME modifiesVar = m_pkbTablesOnly->getModifiesTable()->getModifies(t_curProgLine)[0];
  LIST_OF_VAR_NAMES usesVars = m_pkbTablesOnly->getUsesTable()->getUses(t_curProgLine);
  // checks the lmt to see is any affects relation
  if (t_lmt.empty()) {
    t_lmt.insert({ modifiesVar, { t_curProgLine } });
    return;
  }
  // find affects relation
  for (auto &uItr : usesVars) {
    auto pItr = t_lmt.find(uItr);
    if (pItr == t_lmt.end()) {
      continue;
    }
    // there is a match of modifies and uses
    for (auto &mItr : pItr->second) {
      // insert into 'a' list
      auto aItr = affectsList.find(mItr);
      if (aItr == affectsList.end()) {
        affectsList.insert({ mItr, {t_curProgLine} });
      } else {
        aItr->second.insert(t_curProgLine);
      }
      // insert into 'b' list
      auto bItr = affectedByList.find(t_curProgLine);
      if (bItr == affectedByList.end()) {
        affectedByList.insert({ t_curProgLine, {mItr} });
      } else {
        bItr->second.insert(mItr);
      }
    }
  }
  // update the LMT
  auto pItr = t_lmt.find(modifiesVar);
  if (pItr == t_lmt.end()) {
    t_lmt.insert({ modifiesVar, { t_curProgLine } });
    return;
  }
  pItr->second.clear();
  pItr->second.insert(t_curProgLine);
}

void AffectsTable::handleAffectsOnCallStmt(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
  LIST_OF_VAR_NAMES listOfVarModified= m_pkbTablesOnly->getModifiesTable()->getModifies(t_curProgLine);
  if (t_lmt.empty()) {
    return;
  }
  for (auto& mItr : listOfVarModified) {
    auto pItr = t_lmt.find(mItr);
    if (pItr == t_lmt.end()) {
      continue;
    }
    t_lmt.erase(pItr);
  }
}

BOOLEAN AffectsTable::traverseCfgWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, PROG_LINE t_targetStart, PROG_LINE t_targetEnd) {
  if (t_curProgLine > t_endBound) {
    return false;
  }
  // Checks for the current line if it is a container stmt
  queryType::GType stmtType = m_stmtTable->getTypeOfStatement(t_curProgLine);
  if (isContainerStmt(stmtType)) {
    return traverseContainerCfgWithBoundEarlyExit(t_curProgLine, t_endBound, t_lmt, stmtType, t_targetStart, t_targetEnd);
  } else {
    return traverseNonContainerCfgWithBoundEarlyExit(t_curProgLine, t_endBound, t_lmt, stmtType, t_targetStart, t_targetEnd);
  }
  return false;
}

BOOLEAN AffectsTable::traverseContainerCfgWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type, PROG_LINE t_targetStart, PROG_LINE t_targetEnd) {
  // Get the 2 stmts that leads to the container stmts
  if (t_type == queryType::GType::IF) {
    return traverseIfStmtWithBoundEarlyExit(t_curProgLine, t_endBound, t_lmt, t_targetStart, t_targetEnd);
  } else if (t_type == queryType::GType::WHILE) {
    return traverseWhileStmtWithBoundEarlyExit(t_curProgLine, t_endBound, t_lmt, t_targetStart, t_targetEnd);
  }
  return false;
}

BOOLEAN AffectsTable::traverseNonContainerCfgWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type, PROG_LINE t_targetStart, PROG_LINE t_targetEnd) {
  if (t_type == queryType::GType::ASGN) {
    if (handleAffectsOnAssgnStmtEarlyExit(t_curProgLine, t_lmt, t_targetStart, t_targetEnd)) {
      return true;
    }
  } else if (t_type == queryType::GType::CALL) {
    handleAffectsOnCallStmt(t_curProgLine, t_lmt);
  }
  if (m_nextTable->getAfterGraph()->find(t_curProgLine) == m_nextTable->getAfterGraph()->end()) {
    return false;
  }
  PROG_LINE nextProgLine = m_nextTable->getAfterGraph()->at(t_curProgLine)[0];
  if (nextProgLine < t_curProgLine) {
    return false;
  }
  return traverseCfgWithBoundEarlyExit(nextProgLine, t_endBound, t_lmt, t_targetStart, t_targetEnd);
}

BOOLEAN AffectsTable::traverseIfStmtWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, PROG_LINE t_targetStart, PROG_LINE t_targetEnd) {
  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS ifLmt = t_lmt;
  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
  if (traverseCfgWithBoundEarlyExit(stmts[0], stmtLstBound.back(), ifLmt, t_targetStart, t_targetEnd)) {
    return true;
  }
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS elseLmt = t_lmt;
  if (traverseCfgWithBoundEarlyExit(stmts[1], stmtLstBound.back(), elseLmt, t_targetStart, t_targetEnd)) {
    return true;
  }
  // For if both stmts leads to stmt lst
  t_lmt = mergeLmt(ifLmt, elseLmt);

  auto nItr = m_nextTable->getAfterGraph()->find(stmtLstBound.back());
  queryType::GType stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(stmtLstBound.back());
  if (stmtType == queryType::GType::WHILE && nItr->second.size() < 2) {
    return false;
  }
  if (nItr == m_nextTable->getAfterGraph()->end()) {
    return false;
  }
  PROG_LINE nextStmt = nItr->second[0];
  stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(nextStmt);
  if (stmtType == queryType::GType::WHILE) {
    return false;
  }
  // Combine lms with lmt
  return traverseCfgWithBoundEarlyExit(nextStmt, t_endBound, t_lmt, t_targetStart, t_targetEnd);
}

BOOLEAN AffectsTable::traverseWhileStmtWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, PROG_LINE t_targetStart, PROG_LINE t_targetEnd) {
  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
  // Get the stmts lst of both then and else portion
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS insideStmtLst = t_lmt;
  if (traverseCfgWithBoundEarlyExit(stmts[0], stmtLstBound.back(), insideStmtLst, t_targetStart, t_targetEnd)) {
    return true;
  }
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS mergedLst = mergeLmt(insideStmtLst, t_lmt);
  if (traverseCfgWithBoundEarlyExit(stmts[0], stmtLstBound.back(), mergedLst, t_targetStart, t_targetEnd)) {
    return true;
  }
  // For while one stmt leads to stmt lst the other stmt if any leads to the next stmt in the cur stmt lst.
  if (stmts.size() > 1) {
    queryType::GType stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(stmts[1]);
    if (stmtType == queryType::GType::WHILE) {
      return false;
    }
    // Combine lms with lmt
    // Combine lms with lmt
    MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS nextMergedLst = mergeLmt(t_lmt, mergedLst);
    traverseCfgWithBound(stmts[1], t_endBound, nextMergedLst);
    if (traverseCfgWithBoundEarlyExit(stmts[1], t_endBound, mergedLst, t_targetStart, t_targetEnd)) {
      return true;
    }
    t_lmt = nextMergedLst;
    return false;
  }
  t_lmt = mergedLst;
  return false;
}

BOOLEAN AffectsTable::handleAffectsOnAssgnStmtEarlyExit(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, PROG_LINE t_targetStart, PROG_LINE t_targetEnd) {
  VAR_NAME modifiesVar = m_pkbTablesOnly->getModifiesTable()->getModifies(t_curProgLine)[0];
  LIST_OF_VAR_NAMES usesVars = m_pkbTablesOnly->getUsesTable()->getUses(t_curProgLine);
  // checks the lmt to see is any affects relation
  if (t_lmt.empty()) {
    t_lmt.insert({ modifiesVar,{ t_curProgLine } });
    return false;
  }
  // find affects relation
  for (auto &uItr : usesVars) {
    auto pItr = t_lmt.find(uItr);
    if (pItr == t_lmt.end()) {
      continue;
    }
    for (auto& mItr : pItr->second) {
      if (t_targetStart == INVALID_INDEX && t_targetEnd == t_curProgLine) {
        return true;
      }
      if (t_targetEnd == INVALID_INDEX && t_targetStart == mItr) {
        return true;
      }
    }
  }
  // update the LMT
  auto pItr = t_lmt.find(modifiesVar);
  if (pItr == t_lmt.end()) {
    t_lmt.insert({ modifiesVar,{ t_curProgLine } });
    return false;
  }
  pItr->second.clear();
  pItr->second.insert(t_curProgLine);
  return false;
}

///////////////////////////////////////////////////////
//  Affects*
///////////////////////////////////////////////////////
/**
* From verbena's doc on PKB > Affects
* getAffectsListFromLMS(INT, INT)
*/
PAIR_OF_AFFECTS_LIST AffectsTable::getAffectsStarListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {   // Checks if the start point type
  const std::map<PROG_LINE, std::vector<PROG_LINE>> *readOnlyCFG = m_nextTable->getAfterGraph();
  // Check if the startBound is a container stmt
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms, lastUseTable;
  PROG_LINE realStartBound = getRealStartBound(t_startBound);
  traverseCfgWithBound(realStartBound, t_endBound, lms, lastUseTable);
  return{ affectsList, affectedByList };
}

void AffectsTable::traverseCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lastUses) {
  if (t_curProgLine > t_endBound) {
    return;
  }
  // Checks for the current line if it is a container stmt
  queryType::GType stmtType = m_stmtTable->getTypeOfStatement(t_curProgLine);
  if (isContainerStmt(stmtType)) {
    traverseContainerCfgWithBound(t_curProgLine, t_endBound, t_lmt, t_lastUses, stmtType);
  } else {
    traverseNonContainerCfgWithBound(t_curProgLine, t_endBound, t_lmt, t_lastUses, stmtType);
  }
}

void AffectsTable::traverseContainerCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lastUses, queryType::GType t_type) {
  // Get the 2 stmts that leads to the container stmts
  if (t_type == queryType::GType::IF) {
    traverseIfStmtWithBound(t_curProgLine, t_endBound, t_lmt, t_lastUses);
  } else if (t_type == queryType::GType::WHILE) {
    traverseWhileStmtWithBound(t_curProgLine, t_endBound, t_lmt, t_lastUses);
  }
}


void AffectsTable::traverseNonContainerCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lastUses, queryType::GType t_type) {
  if (t_type == queryType::GType::ASGN) {
    handleAffectsOnAssgnStmt(t_curProgLine, t_lmt, t_lastUses);
  } else if (t_type == queryType::GType::CALL) {
    handleAffectsOnCallStmt(t_curProgLine, t_lmt, t_lastUses);
  }
  if (m_nextTable->getAfterGraph()->find(t_curProgLine) == m_nextTable->getAfterGraph()->end()) {
    return;
  }
  PROG_LINE nextProgLine = m_nextTable->getAfterGraph()->at(t_curProgLine)[0];
  if (nextProgLine < t_curProgLine) {
    return;
  }
  traverseCfgWithBound(nextProgLine, t_endBound, t_lmt, t_lastUses);
}

void AffectsTable::traverseIfStmtWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lastUses) {
  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS ifLmt = t_lmt, ifLastUses = t_lastUses;
  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
  traverseCfgWithBound(stmts[0], stmtLstBound.back(), ifLmt, ifLastUses);
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS elseLmt = t_lmt, elseLastUses = t_lastUses;
  traverseCfgWithBound(stmts[1], stmtLstBound.back(), elseLmt, elseLastUses);
  // For if both stmts leads to stmt lst
  t_lmt = mergeLmt(ifLmt, elseLmt);
  t_lastUses = mergeLmt(ifLastUses, elseLastUses);

  auto nItr = m_nextTable->getAfterGraph()->find(stmtLstBound.back());
  if (nItr == m_nextTable->getAfterGraph()->end()) {
    return;
  }
  PROG_LINE nextStmt = nItr->second[0];
  queryType::GType stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(nextStmt);
  if (stmtType == queryType::GType::WHILE) {
    return;
  }
  // Combine lms with lmt
  traverseCfgWithBound(nextStmt, t_endBound, t_lmt, t_lastUses);
}

void AffectsTable::traverseWhileStmtWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lastUses) {
  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
  // Get the stmts lst of both then and else portion
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS insideStmtLst = t_lmt, insideLastUses = t_lastUses;
  traverseCfgWithBound(stmts[0], stmtLstBound.back(), insideStmtLst, insideLastUses);
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS mergedLst = mergeLmt(insideStmtLst, t_lmt);
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS mergeUsesTable = mergeLmt(insideLastUses, t_lastUses);
  traverseCfgWithBound(stmts[0], stmtLstBound.back(), mergedLst, mergeUsesTable);
  // For while one stmt leads to stmt lst the other stmt if any leads to the next stmt in the cur stmt lst.
  if (stmts.size() > 1) {
    queryType::GType stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(stmts[1]);
    if (stmtType == queryType::GType::WHILE) {
      return;
    }
    // Combine lms with lmt
    traverseCfgWithBound(stmts[1], t_endBound, mergedLst, mergeUsesTable);
  }
  t_lmt = mergedLst;
}

void AffectsTable::handleAffectsOnAssgnStmt(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lastUses) {
  VAR_NAME modifiesVar = m_pkbTablesOnly->getModifiesTable()->getModifies(t_curProgLine)[0];
  LIST_OF_VAR_NAMES usesVars = m_pkbTablesOnly->getUsesTable()->getUses(t_curProgLine);
  // checks the lmt to see is any affects relation
  if (t_lmt.empty()) {
    t_lmt.insert({ modifiesVar,{ t_curProgLine } });
    t_lastUses.insert({ modifiesVar, {} });
  } else {
    auto pItr = t_lmt.find(modifiesVar);
    if (pItr == t_lmt.end()) {
      t_lmt.insert({ modifiesVar,{ t_curProgLine } });
      t_lastUses.insert({ modifiesVar,{} });
    } else {
      pItr->second.clear();
      pItr->second.insert(t_curProgLine);
      auto lastUseItr = t_lastUses.find(modifiesVar);
      lastUseItr->second.clear();
    }
  }
  // updates the last uses table
  for (auto &uItr : usesVars) {
    auto pItr = t_lmt.find(uItr);
    if (pItr == t_lmt.end()) {
      continue;
    }
    // Add via the LMT
    for (auto &sItr : pItr->second) {
      t_lastUses.find(modifiesVar)->second.insert(sItr);
    }
    auto lastUseItr = t_lastUses.find(uItr);
    for (auto &sItr : lastUseItr->second) {
      t_lastUses.find(modifiesVar)->second.insert(sItr);
    }
  }

  // Updates affects list (in this case this list represents affects*)
  auto  pItr = t_lmt.find(modifiesVar);
  for (auto &mItr : pItr->second) {
    auto uItr = t_lastUses.find(modifiesVar);
    if (uItr == t_lastUses.end()) {
      break;
    }
    for (auto &sItr : uItr->second) {
      auto aItr = affectsList.find(sItr);
      if (aItr == affectsList.end()) {
        affectsList.insert({ sItr, {mItr} });
      } else {
        aItr->second.insert(mItr);
      }
      auto bItr = affectedByList.find(mItr);
      if (bItr == affectedByList.end()) {
        affectedByList.insert({ mItr, { sItr} });
      } else {
        bItr->second.insert(sItr);
      }
    }
  }
}

void AffectsTable::handleAffectsOnCallStmt(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lastUses) {
  LIST_OF_VAR_NAMES listOfVarModified = m_pkbTablesOnly->getModifiesTable()->getModifies(t_curProgLine);
  if (t_lmt.empty()) {
    return;
  }
  for (auto& mItr : listOfVarModified) {
    auto pItr = t_lmt.find(mItr);
    if (pItr == t_lmt.end()) {
      continue;
    }
    t_lmt.erase(pItr);
    auto lastUseItr = t_lastUses.find(mItr);
    lastUseItr->second.clear();
  }
}

BOOLEAN AffectsTable::isContainerStmt(queryType::GType t_type) {
  return t_type == queryType::GType::IF || t_type == queryType::GType::WHILE;
}

MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS AffectsTable::mergeLmt(MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS t_lmt1, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS t_lmt2) {
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS mergedList = t_lmt1;
  for (auto& mItr : t_lmt2) {
    auto mergedItr = mergedList.find(mItr.first);
    if (mergedItr == mergedList.end()) {
      mergedList.insert({mItr.first, mItr.second});
      continue;
    }
    for (auto& lItr : mItr.second) {
      mergedItr->second.insert(lItr);
    }
  }
  return mergedList;
}

PROG_LINE AffectsTable::getRealStartBound(PROG_LINE t_startBound) {
  LIST_OF_STMT_NUMS parentsOfStart = m_pkbTablesOnly->getParentTable()->getParentStarOf(t_startBound);
  StatementTable *stmtTable = m_pkbTablesOnly->getStatementTable();
  PROG_LINE realStartBound = t_startBound;
  for (auto& pItr : parentsOfStart) {
    queryType::GType stmtType = stmtTable->getTypeOfStatement(pItr);
    if (stmtType == queryType::GType::WHILE) {
      if (realStartBound > pItr) {
        realStartBound = pItr;
      }
    }
  }
  return realStartBound;
}