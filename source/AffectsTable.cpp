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

BOOLEAN AffectsTable::handleAssignStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut) {
  VarTable *varTable = m_pkbTablesOnly->getVarTable();
  StatementTable *stmtTable = m_pkbTablesOnly->getStatementTable();
  ModifiesTable *modifiesTable = m_pkbTablesOnly->getModifiesTable();
  UsesTable *usesTable = m_pkbTablesOnly->getUsesTable();
  VAR_NAME modifiesVar = modifiesTable->getModifies(t_nextLine).front();
  LIST_OF_VAR_NAMES usesVars = usesTable->getUses(t_nextLine);
  auto *cfg = m_pkbTablesOnly->getNextTable()->getAfterGraph();
  if (t_lmt.empty()) {
    t_lmt.insert({ modifiesVar , {t_nextLine} });
    t_lut.insert({ modifiesVar, {} });
  } else {
    // Update lut
    auto &lutEntry = t_lut.find(modifiesVar);
    if (lutEntry == t_lut.end()) {
      t_lut.insert({ modifiesVar,{} });
    }
    lutEntry = t_lut.find(modifiesVar);
    SET_OF_STMT_NUMS tempStmtNumStorage;
    for (auto &useVar : usesVars) {
      if (modifiesVar == useVar) {
        tempStmtNumStorage = lutEntry->second;
      }
    }
    lutEntry->second.clear();
    if (m_isAffectsStar) {
      for (auto &stmtYetToAdd : tempStmtNumStorage) {
        lutEntry->second.insert(stmtYetToAdd);
      }
    }
    SET_OF_STMT_NUMS lookedAt;
    for (auto &usesVar : usesVars) {
      // query lmt if they have uses
      auto &usesFromLmt = t_lmt.find(usesVar);
      if (usesFromLmt == t_lmt.end()) {
        continue;
      }
      // populate lut
      auto &stmtsFromLut = t_lut.find(usesVar)->second;
      if (m_isAffectsStar == true) {
          handleInsertionForAffectsStar(modifiesVar, t_lmt, t_lut, stmtsFromLut, lookedAt);
      }
      for (auto &stmtFromLmt : usesFromLmt->second) {
        lutEntry->second.insert(stmtFromLmt);
      }
    }
    // Update lmt
    auto &lmtEntry = t_lmt.find(modifiesVar);
    if (lmtEntry == t_lmt.end()) {
      t_lmt.insert({ modifiesVar,{} });
    }
    lmtEntry = t_lmt.find(modifiesVar);
    lmtEntry->second.clear();
    lmtEntry->second.insert(t_nextLine);

    // Update affects list
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
BOOLEAN AffectsTable::handleCallStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut) {
  LIST_OF_VAR_NAMES modifiesVars = m_pkbTablesOnly->getModifiesTable()->getModifies(t_nextLine);
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
  for (auto modifiesVar : modifiesVars) {
    auto isModifiesInLmt = t_lmt.find(modifiesVar);
    if (isModifiesInLmt == t_lmt.end()) {
      continue;
    }
    t_lmt.erase(isModifiesInLmt);
    auto isModifiesInLut = t_lut.find(modifiesVar);
    if (isModifiesInLut == t_lut.end()) {
      continue;
    }
    t_lut.erase(isModifiesInLut);
  }
  return false;
}
BOOLEAN AffectsTable::handleIfStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut) {
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS ifLmt = t_lmt, elseLmt = t_lmt;
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS ifLut = t_lut, elseLut = t_lut;
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
BOOLEAN AffectsTable::handleWhileStmt(PROG_LINE &t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut) {
  MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS tempLmt = t_lmt, tempLut = t_lut;
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
  tempLmt = mergeTable(tempLmt, t_lmt);
  tempLut = mergeTable(tempLut, t_lut);
  int itrCount = 1;
  if (m_isAffectsStar) {
    itrCount = 2;
  }

  for (int i = 0; i < itrCount; ++i) {
    nextStmt = stmtLst.front();
    if (traverseCfgWithinBound(nextStmt, stmtLst.back(), tempLmt, tempLut)
      && m_isEarlyExit) {
      return true;
    }
    tempLmt = mergeTable(tempLmt, t_lmt);
    tempLut = mergeTable(tempLut, t_lut);
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

void AffectsTable::handleInsertionForAffectsStar(VAR_NAME t_curModifiesVar, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut, SET_OF_STMT_NUMS t_usesStmt, SET_OF_STMT_NUMS &t_lookedAt) {
  auto lutEntry = t_lut.find(t_curModifiesVar);
  if (lutEntry == t_lut.end()) {
    return;
  }
  SET_OF_STMT_NUMS numNotYetVisited;
  for (auto &usesToBeAdded : t_usesStmt) {
    auto& varsLookedAt = t_lookedAt.find(usesToBeAdded);
    if (varsLookedAt == t_lookedAt.end()) {
      t_lookedAt.insert(usesToBeAdded);
      numNotYetVisited.insert(usesToBeAdded);
    }
  }
  if (numNotYetVisited.empty() == true) {
    return;
  }
  for (auto &usesToBeAdded : numNotYetVisited) {
    lutEntry->second.insert(usesToBeAdded);
    // add the new modifications here
    // check exist in lmt
    VAR_NAME usesVarToBeAdded = m_pkbTablesOnly->getModifiesTable()->getModifies(usesToBeAdded)[0];
    auto &usesExistInLmt = t_lmt.find(usesVarToBeAdded);
    if (usesExistInLmt == t_lmt.end()) {
      continue;
    }
    auto &usesInLut = t_lut.find(usesVarToBeAdded);
    handleInsertionForAffectsStar(t_curModifiesVar, t_lmt, t_lut, usesInLut->second, t_lookedAt);
  }
}

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

void AffectsTable::handleInsertionForAffectsStar(PROG_LINE t_nextLine, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lmt, MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS &t_lut, VAR_NAME t_modifiesVar, LIST_OF_VAR_NAMES t_usesVars) {

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