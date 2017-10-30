#include "AffectsTable.h"

///////////////////////////////////////////////////////
//  Affects
///////////////////////////////////////////////////////
BOOLEAN AffectsTable::hasAnyAffects() {
  LIST_OF_PROC_NAMES procNames = m_pkbTablesOnly->getProcTable()->getAllProcsName();
  StatementTable *stmtTable = m_pkbTablesOnly->getStatementTable();
  for (auto& name : procNames) {
    PROC_INDEX procIdx = m_pkbTablesOnly->getProcTable()->getProcIdxFromName(name);
    LIST_OF_STMT_NUMS stmts = stmtTable->getStmtsFromProcIdx(procIdx);
    MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms;
    if (traverseCfgWithBoundEarlyExit(stmts.front(), stmts.back(), lms)) {
      return true;
    }
  }
  return false;
}

BOOLEAN AffectsTable::hasAffectsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms;
  // Checks if they are with in a while stmt
  PROG_LINE realStartBound = getRealStartBound(t_startBound);
  return traverseCfgWithBoundEarlyExit(realStartBound, t_endBound, lms);
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
  t_lmt = mergeLmt(ifLmt, elseLmt);
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
    // Combine lms with lmt
    traverseCfgWithBound(stmts[1], t_endBound, mergedLst);
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

BOOLEAN AffectsTable::traverseCfgWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
  if (t_curProgLine > t_endBound) {
    return false;
  }
  // Checks for the current line if it is a container stmt
  queryType::GType stmtType = m_stmtTable->getTypeOfStatement(t_curProgLine);
  if (isContainerStmt(stmtType)) {
    return traverseContainerCfgWithBoundEarlyExit(t_curProgLine, t_endBound, t_lmt, stmtType);
  } else {
    return traverseNonContainerCfgWithBoundEarlyExit(t_curProgLine, t_endBound, t_lmt, stmtType);
  }
  return false;
}

BOOLEAN AffectsTable::traverseContainerCfgWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type) {
  // Get the 2 stmts that leads to the container stmts
  if (t_type == queryType::GType::IF) {
    return traverseIfStmtWithBoundEarlyExit(t_curProgLine, t_endBound, t_lmt);
  } else if (t_type == queryType::GType::WHILE) {
    return traverseWhileStmtWithBoundEarlyExit(t_curProgLine, t_endBound, t_lmt);
  }
  return false;
}

BOOLEAN AffectsTable::traverseNonContainerCfgWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type) {
  if (t_type == queryType::GType::ASGN) {
    if (handleAffectsOnAssgnStmtEarlyExit(t_curProgLine, t_lmt)) {
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
  return traverseCfgWithBoundEarlyExit(nextProgLine, t_endBound, t_lmt);
}

BOOLEAN AffectsTable::traverseIfStmtWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS ifLmt = t_lmt;
  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
  if (traverseCfgWithBoundEarlyExit(stmts[0], stmtLstBound.back(), ifLmt)) {
    return true;
  }
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS elseLmt = t_lmt;
  if (traverseCfgWithBoundEarlyExit(stmts[1], stmtLstBound.back(), elseLmt)) {
    return true;
  }
  // For if both stmts leads to stmt lst
  t_lmt = mergeLmt(ifLmt, elseLmt);
}

BOOLEAN AffectsTable::traverseWhileStmtWithBoundEarlyExit(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
  // Get the stmts lst of both then and else portion
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS insideStmtLst = t_lmt;
  if (traverseCfgWithBoundEarlyExit(stmts[0], stmtLstBound.back(), insideStmtLst)) {
    return true;
  }
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS mergedLst = mergeLmt(insideStmtLst, t_lmt);
  if (traverseCfgWithBoundEarlyExit(stmts[0], stmtLstBound.back(), mergedLst)) {
    return true;
  }
  // For while one stmt leads to stmt lst the other stmt if any leads to the next stmt in the cur stmt lst.
  if (stmts.size() > 1) {
    // Combine lms with lmt
    if (traverseCfgWithBoundEarlyExit(stmts[1], t_endBound, mergedLst)) {
      return true;
    }
  }
  t_lmt = mergedLst;
  return false;
}

BOOLEAN AffectsTable::handleAffectsOnAssgnStmtEarlyExit(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
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
    return true;
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
* hasAffectsFromLMS()
*/
BOOLEAN AffectsTable::hasAnyAffectsStar() {
  return false;
}

/**
* From verbena's doc on PKB > Affects
* hasAffectsBetween(INT, INT)
*/
BOOLEAN AffectsTable::hasAffectsStarFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  return false;
}

/**
* From verbena's doc on PKB > Affects
* isAffectsFromLMS(INT, INT)
* Affects(4, 12) is true
*/
BOOLEAN AffectsTable::isAffectsStar(STMT_NUM t_modfiesLine, STMT_NUM t_usesLine) {
  return false;
}

/**
* From verbena's doc on PKB > Affects
* getAffectsListFromLMS(INT, INT)
*/
PAIR_OF_AFFECTS_LIST AffectsTable::getAffectsStarListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  return{};
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