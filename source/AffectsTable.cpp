#include "AffectsTable.h"
#include <algorithm>
///////////////////////////////////////////////////////
//  Affects
///////////////////////////////////////////////////////
BOOLEAN AffectsTable::hasAffectsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound, PROG_LINE t_targetStart, PROG_LINE t_targetEnd) {
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms, lut;
  // Checks if they are with in a while stmt
  PROG_LINE realStartBound = getRealStartBound(t_startBound);
  m_targetStart = t_targetStart;
  m_targetEnd = t_targetEnd;
  return traverseCfgWithinBound(realStartBound, t_endBound, lms, lut);
}

BOOLEAN AffectsTable::isAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) {
  m_earlyExit = true; m_isAffectsStar = false;
  return hasAffectsFromBounds(t_modifiesLine, t_usesLine, t_modifiesLine, t_usesLine);
}

BOOLEAN AffectsTable::isAffectsStar(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) {
  m_earlyExit = m_isAffectsStar = true;
  return hasAffectsFromBounds(t_modifiesLine, t_usesLine, t_modifiesLine, t_usesLine);
}

PAIR_OF_AFFECTS_LIST AffectsTable::getAffectsListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  // Check if the startBound is a container stmt
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms, lut;
  PROG_LINE realStartBound = getRealStartBound(t_startBound);
  m_isAffectsStar = false;
  traverseCfgWithinBound(realStartBound, t_endBound, lms, lut);
  return {m_affectsList, m_affectedByList};
}

PAIR_OF_AFFECTS_LIST AffectsTable::getAffectsStarListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms, lut;
  PROG_LINE realStartBound = getRealStartBound(t_startBound);
  m_isAffectsStar = true;
  traverseCfgWithinBound(realStartBound, t_endBound, lms, lut);
  return{ m_affectsList, m_affectedByList };
}

BOOLEAN AffectsTable::traverseCfgWithinBound(PROG_LINE &t_nextLine, PROG_LINE t_endBound,
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut) {
  if (t_nextLine > t_endBound) {
    return false;
  }
  // traverse the first line
  queryType::GType stmtType = m_stmtTable->getTypeOfStatement(t_nextLine);
  switch (stmtType) {
  case queryType::GType::ASGN:
    if (handleAssignStmt(t_nextLine, t_lmt, t_lut) 
        && m_earlyExit == true) {
      return true;
    }
    break;
  case queryType::GType::CALL:
    handleCallStmt(t_nextLine, t_lmt, t_lut);
    break;
  case queryType::GType::IF:
    handleIfStmt(t_nextLine, t_lmt, t_lut);
    break;
  case queryType::GType::WHILE:
    handleWhileStmt(t_nextLine, t_lmt, t_lut);
    break;
  default:
    return false;
  }
  return traverseCfgWithinBound(t_nextLine, t_endBound, t_lmt, t_lut);
}

BOOLEAN AffectsTable::handleAssignStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut) {
  return false;
}
BOOLEAN AffectsTable::handleCallStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut) {
  return false;
}
BOOLEAN AffectsTable::handleIfStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut) {
  return false;
}
BOOLEAN AffectsTable::handleWhileStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut) {
  return false;
}

//void AffectsTable::traverseCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
//  t_nextLine = t_curProgLine;
//  if (t_curProgLine > t_endBound) {
//    return;
//  }
//  // Checks for the current line if it is a container stmt
//  queryType::GType stmtType = m_stmtTable->getTypeOfStatement(t_curProgLine);
//  if (isContainerStmt(stmtType)) {
//    traverseContainerCfgWithBound(t_curProgLine, t_endBound, t_nextLine, t_lmt, stmtType);
//  } else {
//    traverseNonContainerCfgWithBound(t_curProgLine, t_endBound, t_nextLine, t_lmt, stmtType);
//  }
//}
//
//void AffectsTable::traverseContainerCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type) {
//  // Get the 2 stmts that leads to the container stmts
//  if (t_type == queryType::GType::IF) {
//    traverseIfStmtWithBound(t_curProgLine, t_endBound, t_nextLine, t_lmt);
//  } else if (t_type == queryType::GType::WHILE) {
//    traverseWhileStmtWithBound(t_curProgLine, t_endBound, t_nextLine, t_lmt);
//  }
//}
//
//void AffectsTable::traverseNonContainerCfgWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, queryType::GType t_type) {
//  if (t_type == queryType::GType::ASGN) {
//    handleAffectsOnAssgnStmt(t_curProgLine, t_lmt);
//  } else if (t_type == queryType::GType::CALL) {
//    handleAffectsOnCallStmt(t_curProgLine, t_lmt);
//  }
//  if (m_nextTable->getAfterGraph()->find(t_curProgLine) == m_nextTable->getAfterGraph()->end()) {
//    t_nextLine = INVALID_PROG_LINE;
//    return;
//  }
//  PROG_LINE nextProgLine = m_nextTable->getAfterGraph()->at(t_curProgLine)[0];
//  if (nextProgLine < t_curProgLine) {
//    t_nextLine = nextProgLine;
//    return;
//  }
//  traverseCfgWithBound(nextProgLine, t_endBound, t_nextLine, t_lmt);
//}
//
//void AffectsTable::traverseIfStmtWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
//  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
//  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS ifLmt = t_lmt;
//  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
//  traverseCfgWithBound(stmts[0], stmtLstBound.back(), t_nextLine, ifLmt);
//  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS elseLmt = t_lmt;
//  traverseCfgWithBound(stmts[1], stmtLstBound.back(), t_nextLine, elseLmt);
//  // For if both stmts leads to stmt lst
//  auto nItr = m_nextTable->getAfterGraph()->find(stmtLstBound.back());
//  queryType::GType stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(stmtLstBound.back());
//  if (stmtType == queryType::GType::WHILE && nItr->second.size() < 2) {
//    return;
//  }
//  if (t_nextLine == INVALID_PROG_LINE) {
//    return;
//  }
//  t_lmt = mergeTable(ifLmt, elseLmt);
//  stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(t_nextLine);
//  if (stmtType == queryType::GType::WHILE && m_pkbTablesOnly->getParentTable()->isParentStar(t_nextLine, t_curProgLine)) {
//    return;
//  }
//  // Combine lms with lmt
//  traverseCfgWithBound(t_nextLine, t_endBound, t_nextLine, t_lmt);
//}
//
//void AffectsTable::traverseWhileStmtWithBound(PROG_LINE t_curProgLine, PROG_LINE t_endBound, PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
//  LIST_OF_STMT_NUMS stmts = m_nextTable->getAfterGraph()->at(t_curProgLine);
//  LIST_OF_STMT_NUMS stmtLstBound = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_curProgLine);
//  // Get the stmts lst of both then and else portion
//  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS insideStmtLst = t_lmt;
//  traverseCfgWithBound(stmts[0], stmtLstBound.back(), t_nextLine, insideStmtLst);
//  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS mergedLst = mergeTable(insideStmtLst, t_lmt);
//  traverseCfgWithBound(stmts[0], stmtLstBound.back(), t_nextLine, mergedLst);
//  mergedLst = mergeTable(t_lmt, mergedLst);
//  t_lmt = mergedLst;
//  // For while one stmt leads to stmt lst the other stmt if any leads to the next stmt in the cur stmt lst.
//  if (stmts.size() > 1) {
//    queryType::GType stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(stmts[1]);
//    if (stmtType == queryType::GType::WHILE && m_pkbTablesOnly->getParentTable()->isParentStar(stmts[1], t_curProgLine)) {
//      return;
//    }
//    // Combine lms with lmt
//    traverseCfgWithBound(stmts[1], t_endBound, stmts[1], t_lmt);
//    t_nextLine = stmts[1];
//    return;
//  }
//}
//
//void AffectsTable::handleAffectsOnAssgnStmt(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
//  VAR_NAME modifiesVar = m_pkbTablesOnly->getModifiesTable()->getModifies(t_curProgLine)[0];
//  LIST_OF_VAR_NAMES usesVars = m_pkbTablesOnly->getUsesTable()->getUses(t_curProgLine);
//  // checks the lmt to see is any affects relation
//  if (t_lmt.empty()) {
//    t_lmt.insert({ modifiesVar, { t_curProgLine } });
//    return;
//  }
//  // find affects relation
//  for (auto &uItr : usesVars) {
//    auto pItr = t_lmt.find(uItr);
//    if (pItr == t_lmt.end()) {
//      continue;
//    }
//    // there is a match of modifies and uses
//    for (auto &mItr : pItr->second) {
//      insertIntoAffectsLists(mItr, t_curProgLine);
//    }
//  }
//  // update the LMT
//  auto pItr = t_lmt.find(modifiesVar);
//  if (pItr == t_lmt.end()) {
//    t_lmt.insert({ modifiesVar, { t_curProgLine } });
//    return;
//  }
//  pItr->second.clear();
//  pItr->second.insert(t_curProgLine);
//}
//
//void AffectsTable::handleAffectsOnCallStmt(PROG_LINE t_curProgLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt) {
//  LIST_OF_VAR_NAMES listOfVarModified= m_pkbTablesOnly->getModifiesTable()->getModifies(t_curProgLine);
//  if (t_lmt.empty()) {
//    return;
//  }
//  for (auto& mItr : listOfVarModified) {
//    auto pItr = t_lmt.find(mItr);
//    if (pItr == t_lmt.end()) {
//      continue;
//    }
//    t_lmt.erase(pItr);
//  }
//}

BOOLEAN AffectsTable::isContainerStmt(queryType::GType t_type) {
  return t_type == queryType::GType::IF || t_type == queryType::GType::WHILE;
}

MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS AffectsTable::mergeTable(MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS t_lmt1, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS t_lmt2) {
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

void AffectsTable::insertIntoAffectsLists(PROG_LINE t_modifiesLine, PROG_LINE t_usesLine) {
  // insert into 'a' list
  auto aItr = m_affectsList.find(t_modifiesLine);
  if (aItr == m_affectsList.end()) {
    m_affectsList.insert({ t_modifiesLine,{ t_usesLine } });
  } else {
    aItr->second.insert(t_usesLine);
  }
  // insert into 'b' list
  auto bItr = m_affectedByList.find(t_usesLine);
  if (bItr == m_affectedByList.end()) {
    m_affectedByList.insert({ t_usesLine,{ t_modifiesLine } });
  } else {
    bItr->second.insert(t_modifiesLine);
  }
}