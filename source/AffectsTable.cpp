#include "AffectsTable.h"
#include <algorithm>
///////////////////////////////////////////////////////
//  Affects
///////////////////////////////////////////////////////
BOOLEAN AffectsTable::hasAffectsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound, PROG_LINE t_targetStart, PROG_LINE t_targetEnd) {
  MAP_OF_VAR_INDEX_TO_SET_OF_NUMS lms, lut;
  // Checks if they are with in a while stmt
  PROG_LINE realStartBound = getRealStartBound(t_startBound);
  m_targetStart = t_targetStart;
  m_targetEnd = t_targetEnd;
  m_isEarlyExit = true;
  return traverseCfgWithinBound(realStartBound, t_endBound, lms, lut);
}

BOOLEAN AffectsTable::isAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) {
  m_isEarlyExit = true; m_isAffectsStar = false;
  return hasAffectsFromBounds(t_modifiesLine, t_usesLine, t_modifiesLine, t_usesLine);
}

BOOLEAN AffectsTable::isAffectsStar(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) {
  m_isEarlyExit = m_isAffectsStar = true;
  return hasAffectsFromBounds(t_modifiesLine, t_usesLine, t_modifiesLine, t_usesLine);
}

PAIR_OF_AFFECTS_LIST AffectsTable::getAffectsListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  // Check if the startBound is a container stmt
  MAP_OF_VAR_INDEX_TO_SET_OF_NUMS lms, lut;
  PROG_LINE realStartBound = getRealStartBound(t_startBound);
  m_isAffectsStar = false;
  traverseCfgWithinBound(realStartBound, t_endBound, lms, lut);
  return {m_affectsList, m_affectedByList};
}

PAIR_OF_AFFECTS_LIST AffectsTable::getAffectsStarListsFromBounds(STMT_NUM t_startBound, STMT_NUM t_endBound) {
  MAP_OF_VAR_INDEX_TO_SET_OF_NUMS lms, lut;
  PROG_LINE realStartBound = getRealStartBound(t_startBound);
  m_isAffectsStar = true;
  traverseCfgWithinBound(realStartBound, t_endBound, lms, lut);
  return{ m_affectsList, m_affectedByList };
}

BOOLEAN AffectsTable::traverseCfgWithinBound(PROG_LINE &t_nextLine, PROG_LINE t_endBound,
  MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut) {
  if (t_nextLine == INVALID_PROG_LINE) {
    return false;
  }
  if (t_nextLine > t_endBound) {
    return false;
  }
  PROG_LINE prevNextLine = t_nextLine;
  // traverse the first line
  queryType::GType stmtType = m_stmtTable->getTypeOfStatement(t_nextLine);
  switch (stmtType) {
  case queryType::GType::ASGN:
    if (handleAssignStmt(t_nextLine, t_lmt, t_lut) 
        && m_isEarlyExit == true) {
      return true;
    }
    break;
  case queryType::GType::CALL:
    handleCallStmt(t_nextLine, t_lmt, t_lut);
    break;
  case queryType::GType::IF:
    if (handleIfStmt(t_nextLine, t_lmt, t_lut)
        && m_isEarlyExit == true) {
      return true;
    }
    break;
  case queryType::GType::WHILE:
    if (handleWhileStmt(t_nextLine, t_lmt, t_lut)
        && m_isEarlyExit) {
      return true;
    }
    break;
  default:
    return false;
  }
  if (t_nextLine < prevNextLine) {
    return false;
  }
  return traverseCfgWithinBound(t_nextLine, t_endBound, t_lmt, t_lut);
}

BOOLEAN AffectsTable::handleAssignStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut) {
  VarTable *varTable = m_pkbTablesOnly->getVarTable();
  StatementTable *stmtTable = m_pkbTablesOnly->getStatementTable();
  ModifiesTable *modifiesTable = m_pkbTablesOnly->getModifiesTable();
  UsesTable *usesTable = m_pkbTablesOnly->getUsesTable();
  VAR_INDEX modifiesIndex = modifiesTable->getModifiesByIdx(t_nextLine).front();
  LIST_OF_VAR_INDICES usesVars = usesTable->getUsesByIdx(t_nextLine);
  auto *cfg = m_pkbTablesOnly->getNextTable()->getAfterGraph();
  if (t_lmt.empty()) {
    t_lmt.insert({ modifiesIndex , {t_nextLine} });
    t_lut.insert({ modifiesIndex, {} });
  } else {
    // Update lut
    updateLutWithSameModifiesAndUses(modifiesIndex, usesVars, t_lut);
    updateLutWithOtherUses(modifiesIndex, usesVars, t_lmt, t_lut);
    // Update lmt
    updateLmt(t_nextLine, modifiesIndex, t_lmt);
    // Update affects list
    auto &lutEntry = t_lut.find(modifiesIndex);
    for (auto &useStmt : lutEntry->second) {
      if (m_isEarlyExit) {
        if (m_targetStart == INVALID_PROG_LINE && m_targetEnd == INVALID_PROG_LINE) {
          return true;
        } else if (m_targetStart == INVALID_PROG_LINE && m_targetEnd == t_nextLine) {
          return true;
        } else if (m_targetEnd == INVALID_PROG_LINE && m_targetStart == useStmt) {
          return true;
        } else if (m_targetStart == useStmt && m_targetEnd == t_nextLine) {
          return true;
        }
      } else {
        insertIntoAffectsLists(useStmt, t_nextLine);
      }
    }
  }

  auto &hasNextLine = cfg->find(t_nextLine);
  if (hasNextLine == cfg->end()) {
    t_nextLine = INVALID_PROG_LINE;
    return false;
  }
  t_nextLine = hasNextLine->second[0];
  //traversing part of the algo
  return false;
}
BOOLEAN AffectsTable::handleCallStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut) {
  LIST_OF_VAR_INDICES modifiesVars = m_pkbTablesOnly->getModifiesTable()->getModifiesByIdx(t_nextLine);
  auto *cfg = m_pkbTablesOnly->getNextTable()->getAfterGraph();
  auto &hasNextLine = cfg->find(t_nextLine);
  if (hasNextLine == cfg->end()) {
    t_nextLine = INVALID_PROG_LINE;
    return false;
  }
  t_nextLine = hasNextLine->second[0];
  if (t_lmt.empty()) {
    return false;
  }
  for (auto& mItr : modifiesVars) {
    auto lmtEntry = t_lmt.find(mItr);
    if (lmtEntry == t_lmt.end()) {
      continue;
    }
    t_lmt.erase(lmtEntry);
    auto lutEntry = t_lut.find(mItr);
    if (lutEntry == t_lut.end()) {
      continue;
    }
    t_lut.erase(lutEntry);
  }
  return false;
}
BOOLEAN AffectsTable::handleIfStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut) {
  MAP_OF_VAR_INDEX_TO_SET_OF_NUMS ifLmt = t_lmt, elseLmt = t_lmt;
  MAP_OF_VAR_INDEX_TO_SET_OF_NUMS ifLut = t_lut, elseLut = t_lut;
  PROG_LINE curLine = t_nextLine;
  LIST_OF_STMT_NUMS stmtLst = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_nextLine);
  auto &nextStmts = m_nextTable->getAfterGraph()->find(t_nextLine);
  if (nextStmts == m_nextTable->getAfterGraph()->end()) {
    t_nextLine == INVALID_PROG_LINE;
    return false;
  }
  PROG_LINE nextStmt = nextStmts->second.front();
  if (traverseCfgWithinBound(nextStmt, stmtLst.back(), ifLmt, ifLut)
      && m_isEarlyExit) {
    return true;
  }
  nextStmt = nextStmts->second.back();
  if (traverseCfgWithinBound(nextStmt, stmtLst.back(), elseLmt, elseLut)
    && m_isEarlyExit) {
    return true;
  }

  t_lmt = mergeTable(ifLmt, elseLmt);
  t_lut = mergeTable(ifLut, elseLut);

  t_nextLine = nextStmt;
  if (nextStmt == INVALID_PROG_LINE) {
    return false;
  }
  queryType::GType stmtType = m_pkbTablesOnly->getStatementTable()->getTypeOfStatement(nextStmt);
  if (stmtType == queryType::GType::WHILE && m_pkbTablesOnly->getParentTable()->isParent(nextStmt, curLine)) {
    return false;
  }
  return false;
}
BOOLEAN AffectsTable::handleWhileStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut) {
  MAP_OF_VAR_INDEX_TO_SET_OF_NUMS tempLmt = t_lmt, tempLut = t_lut;
  auto &nextStmts = m_nextTable->getAfterGraph()->find(t_nextLine);
  if (nextStmts == m_nextTable->getAfterGraph()->end()) {
    t_nextLine == INVALID_PROG_LINE;
    return false;
  }

  PROG_LINE curLine = t_nextLine;
  LIST_OF_STMT_NUMS stmtLst = m_pkbTablesOnly->getParentTable()->getChildrenOf(t_nextLine);
  PROG_LINE nextStmt = stmtLst.front();
  if (traverseCfgWithinBound(nextStmt, stmtLst.back(), tempLmt, tempLut)
      && m_isEarlyExit) {
    return true;
  }
  if (!m_isAffectsStar) {
    nextStmt = stmtLst.front();
    tempLmt = mergeTable(tempLmt, t_lmt);
    tempLut = mergeTable(tempLut, t_lut);
    if (traverseCfgWithinBound(nextStmt, stmtLst.back(), tempLmt, tempLut)
        && m_isEarlyExit) {
      return true;
    }
    tempLmt = mergeTable(tempLmt, t_lmt);
    tempLut = mergeTable(tempLut, t_lut);
  } else {
    // While the
    MAP_OF_VAR_INDEX_TO_SET_OF_NUMS prevLmt = tempLmt, prevLut = tempLut;
    BOOLEAN isNotSame = true;
    while (isNotSame) {
      nextStmt = stmtLst.front();
      if (traverseCfgWithinBound(nextStmt, stmtLst.back(), tempLmt, tempLut)
          && m_isEarlyExit) {
        return true;
      }
      tempLmt = mergeTable(tempLmt, t_lmt);
      tempLut = mergeTable(tempLut, t_lut);
      if (prevLut == tempLut) {
        isNotSame = false;
      }
      prevLmt = tempLmt;
      prevLut = tempLut;
    }
  }

  t_lmt = tempLmt;
  t_lut = tempLut;

  // nextLine loop back here
  t_nextLine = nextStmt;
  if (curLine == t_nextLine) {
    if (nextStmts->second.size() < 2) {
      t_nextLine = INVALID_PROG_LINE;
      return false;
    }
    t_nextLine = nextStmts->second.back();
  }
  return false;
}

BOOLEAN AffectsTable::isContainerStmt(queryType::GType t_type) {
  return t_type == queryType::GType::IF || t_type == queryType::GType::WHILE;
}

MAP_OF_VAR_INDEX_TO_SET_OF_STMT_NUMS AffectsTable::mergeTable(MAP_OF_VAR_INDEX_TO_SET_OF_NUMS t_lmt1, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS t_lmt2) {
  MAP_OF_VAR_INDEX_TO_SET_OF_NUMS mergedList = t_lmt1;
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

void AffectsTable::updateLutWithSameModifiesAndUses(VAR_INDEX modifiesIdx, LIST_OF_VAR_INDICES usesVars, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut) {
  // Update lut
  auto &lutEntry = t_lut.find(modifiesIdx);
  if (lutEntry == t_lut.end()) {
    t_lut.insert({ modifiesIdx,{} });
  }
  lutEntry = t_lut.find(modifiesIdx);
  SET_OF_STMT_NUMS tempStmtNumStorage;
  for (auto &useVar : usesVars) {
    if (modifiesIdx == useVar) {
      tempStmtNumStorage = lutEntry->second;
    }
  }
  lutEntry->second.clear();
  if (m_isAffectsStar) {
    for (auto &stmtYetToAdd : tempStmtNumStorage) {
      lutEntry->second.insert(stmtYetToAdd);
    }
  }
}

void AffectsTable::updateLutWithOtherUses(VAR_INDEX modifiesIdx, LIST_OF_VAR_INDICES usesVars, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lut) {
  auto &lutEntry = t_lut.find(modifiesIdx);
  for (auto &usesVar : usesVars) {
    // query lmt if they have uses
    auto &usesFromLmt = t_lmt.find(usesVar);
    if (usesFromLmt == t_lmt.end()) {
      continue;
    }
    // populate lut
    auto &stmtsFromLut = t_lut.find(usesVar)->second;
    if (m_isAffectsStar == true) {
      for (auto &stmtFromLut : stmtsFromLut) {
        lutEntry->second.insert(stmtFromLut);
      }
    }
    for (auto &stmtFromLmt : usesFromLmt->second) {
      lutEntry->second.insert(stmtFromLmt);
    }
  }
}

void AffectsTable::updateLmt(PROG_LINE t_nextLine, VAR_INDEX modifiesIdx, MAP_OF_VAR_INDEX_TO_SET_OF_NUMS &t_lmt) {
  // Update lmt
  auto &lmtEntry = t_lmt.find(modifiesIdx);
  if (lmtEntry == t_lmt.end()) {
    t_lmt.insert({ modifiesIdx,{} });
  }
  lmtEntry = t_lmt.find(modifiesIdx);
  lmtEntry->second.clear();
  lmtEntry->second.insert(t_nextLine);
}