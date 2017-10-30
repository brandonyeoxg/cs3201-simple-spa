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

void AffectsTable::traverseCfg(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS &t_lmt) {
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

void AffectsTable::traverseContainerCfg(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS &t_lmt, queryType::GType t_type) {
  // Get the 2 stmts that leads to the container stmts
  if (t_type == queryType::GType::IF) {
    handleAffectsOnIfStmt(t_curProgLine, t_endBound, t_lmt);
  } else if (t_type == queryType::GType::WHILE) {
    handleAffectsOnWhileStmt(t_curProgLine, t_endBound, t_lmt);
  }
}

void AffectsTable::traverseNonContainerCfg(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS &t_lmt, queryType::GType t_type) {
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
  traverseCfg(nextProgLine, t_endBound, t_lmt);
}

void AffectsTable::handleAffectsOnIfStmt(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS &t_lmt) {
  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS ifLmt = t_lmt;
  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
  traverseCfg(stmts[0], stmtLstBound.back(), ifLmt);
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS elseLmt = t_lmt;
  traverseCfg(stmts[1], stmtLstBound.back(), elseLmt);
  // For if both stmts leads to stmt lst
  t_lmt = mergeLmt(ifLmt, elseLmt);
}

void AffectsTable::handleAffectsOnWhileStmt(PROG_LINE t_curProgLine, PROG_LINE t_endBound, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS &t_lmt) {
  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
  // Get the stmts lst of both then and else portion
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS insideStmtLst = t_lmt;
  traverseCfg(stmts[0], stmtLstBound.back(), insideStmtLst);
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS mergedLst = mergeLmt(insideStmtLst, t_lmt);
  traverseCfg(stmts[0], stmtLstBound.back(), mergedLst);
  // For while one stmt leads to stmt lst the other stmt if any leads to the next stmt in the cur stmt lst.
  if (stmts.size() > 1) {
    // Combine lms with lmt
    traverseCfg(stmts[1], t_endBound, mergedLst);
  }
  t_lmt = mergedLst;
}

void AffectsTable::handleAffectsOnAssgnStmt(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS &t_lmt) {
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
      auto aItr = affectsList.find(mItr);
      if (aItr == affectsList.end()) {
        affectsList.insert({ mItr, {t_curProgLine} });
        continue;
      }
      aItr->second.push_back(t_curProgLine);
    }
  }
  
  // update the LMT
  auto pItr = t_lmt.find(modifiesVar);
  if (pItr == t_lmt.end()) {
    t_lmt.insert({ modifiesVar, { t_curProgLine } });
    return;
  }
  pItr->second.clear();
  pItr->second.push_back(t_curProgLine);
}

void AffectsTable::handleAffectsOnCallStmt(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS &t_lmt) {
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

BOOLEAN AffectsTable::isContainerStmt(queryType::GType t_type) {
  return t_type == queryType::GType::IF || t_type == queryType::GType::WHILE;
}

MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS AffectsTable::mergeLmt(MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS t_lmt1, MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS t_lmt2) {
  MAP_OF_VAR_NAME_TO_LIST_OF_STMT_NUMS mergedList = t_lmt1;
  for (auto& mItr : t_lmt2) {
    auto mergedItr = mergedList.find(mItr.first);
    if (mergedItr == mergedList.end()) {
      mergedList.insert({mItr.first, mItr.second});
      continue;
    }
    for (auto& lItr : mItr.second) {
      mergedItr->second.push_back(lItr);
    }
  }
  return mergedList;
}