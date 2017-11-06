#include "AffectsExtractor.h"

void AffectsExtractor::extractDesign() {
}

MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS AffectsExtractor::extractAllAffects() { // affects(a1,a2)
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS LMS = MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS();
  int numberOfProcedures = m_pkb->getProcTable()->getAllProcsName().size();
  for (int i = 0; i < numberOfProcedures; i++) {
    AffectsTable *affectsTable = new AffectsTable(m_pkb);
    //get the bounds (first and last) stmt no from proc
    LIST_OF_STMT_NUMS bound = m_pkb->getStatementTable()->getStmtsFromProcIdx(i);
    STMT_NUM start = bound[0];
    STMT_NUM end = bound[bound.size()-1];
    PAIR_OF_AFFECTS_LIST intermediateResult = affectsTable->getAffectsListsFromBounds(start, end);
    //initialise new affectsTable as to start processing a new procedure. 
    MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectsList = intermediateResult.first;
    //add affectsList to LMS.
    LMS = appendAffectsList(affectsList, LMS);
    delete affectsTable;
  }
  return LMS;
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffects(STMT_NUM t_usesLine) { // affects(a,12)
  //return empty if t_usesLine exceeds number of lines in program.
  if (m_pkb->getStatementTable()->getNumberOfStatements() < t_usesLine) {
    return{};
  }
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  LIST_OF_STMT_NUMS bound = getListOfStmtsFromStmtNum(t_usesLine);
  STMT_NUM start = bound[0];
  STMT_NUM end = bound[bound.size() - 1];
  PAIR_OF_AFFECTS_LIST intermediateResult = m_affectsTable->getAffectsListsFromBounds(start, end);
  MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectedByList = intermediateResult.second;
  LIST_OF_AFFECTS_STMTS affectsStmts = LIST_OF_AFFECTS_STMTS();
  auto itr = affectedByList.find(t_usesLine);
  if (itr != affectedByList.end()) {
    SET_OF_STMT_NUMS affectsSet = itr->second;
    //copy set to vector    
    affectsStmts.insert(affectsStmts.end(), affectsSet.begin(), affectsSet.end());
    std::sort(affectsStmts.begin(), affectsStmts.end());
  }
  delete m_affectsTable;
  return affectsStmts;
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectedBy(STMT_NUM t_modifiesLine) { // affects(2,a)
  //return empty if t_modifiesLine exceeds number of lines in program.
  if (m_pkb->getStatementTable()->getNumberOfStatements() < t_modifiesLine) {
    return{};
  }
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  LIST_OF_AFFECTS_STMTS affectsStmts = LIST_OF_AFFECTS_STMTS();
  LIST_OF_STMT_NUMS bound = getListOfStmtsFromStmtNum(t_modifiesLine);
  STMT_NUM start = bound[0];
  STMT_NUM end = bound[bound.size() - 1];
  PAIR_OF_AFFECTS_LIST intermediateResult = m_affectsTable->getAffectsListsFromBounds(start, end);
  MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectsList = intermediateResult.first;
  auto itr = affectsList.find(t_modifiesLine);
  if (itr != affectsList.end()) {
    SET_OF_STMT_NUMS affectsSet = itr->second;
    //copy set to vector
    affectsStmts.insert(affectsStmts.end(), affectsSet.begin(), affectsSet.end());
    std::sort(affectsStmts.begin(), affectsStmts.end());
  }
  delete m_affectsTable;
  return affectsStmts;
}

BOOLEAN AffectsExtractor::extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) { // affects(1,12)
  //return empty if either line exceeds number of lines in program.
  TOTAL_NUMBER_OF_STMTS totalLines = m_pkb->getStatementTable()->getNumberOfStatements();
  if (totalLines < t_modifiesLine || totalLines < t_usesLine) {
    return false;
  }
  //check if both are assignment statements.
  if (m_pkb->getStatementTable()->getTypeOfStatement(t_modifiesLine) != queryType::GType::ASGN || m_pkb->getStatementTable()->getTypeOfStatement(t_usesLine) != queryType::GType::ASGN) {
    return false;
  }
  //check if the two are in same proc.
  if (m_pkb->getStatementTable()->getProcNameFromStmtNum(t_modifiesLine) != m_pkb->getStatementTable()->getProcNameFromStmtNum(t_usesLine)) {
    return false;
  }
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  //passes all the above checks, run the method.
  BOOLEAN isAffects = m_affectsTable->isAffects(t_modifiesLine, t_usesLine);
  delete m_affectsTable;
  return isAffects;
}

BOOLEAN AffectsExtractor::extractHasAffectsRelationship() { // affects(_,_)
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  LIST_OF_PROC_NAMES procNames = m_pkb->getProcTable()->getAllProcsName();
  StatementTable *stmtTable = m_pkb->getStatementTable();
  for (auto& name : procNames) {
    PROC_INDEX procIdx = m_pkb->getProcTable()->getProcIdxFromName(name);
    LIST_OF_STMT_NUMS stmts = stmtTable->getStmtsFromProcIdx(procIdx);
    MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms;
    if (m_affectsTable->hasAffectsFromBounds(stmts.front(), stmts.back(), stmts.front(), INVALID_INDEX)) {
      delete m_affectsTable;
      return true;
    }
  }
  delete m_affectsTable;
  return false;
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectsAnything() { // affects(a,_)
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS LMS = MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS();
  int numberOfProcedures = m_pkb->getProcTable()->getAllProcsName().size();
  for (int i = 0; i < numberOfProcedures; i++) {
    AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
    //get the bounds (first and last) stmt no from proc
    LIST_OF_STMT_NUMS bound = m_pkb->getStatementTable()->getStmtsFromProcIdx(i);
    STMT_NUM start = bound[0];
    STMT_NUM end = bound[bound.size() - 1];
    PAIR_OF_AFFECTS_LIST intermediateResult = m_affectsTable->getAffectsListsFromBounds(start, end);
    MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectsList = intermediateResult.first;
    //add affectsList to LMS.
    //run the appendAffectsList method.
    LMS = appendAffectsList(affectsList, LMS);
    delete m_affectsTable;
  }
  LIST_OF_AFFECTS_STMTS results = LIST_OF_AFFECTS_STMTS();
  for (auto itr = LMS.begin(); itr != LMS.end(); ++itr) {
    results.push_back(itr->first);
  }
  return results; //return all the keys affectsList in LMS
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectedByAnything() { // affects(_,a)
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS LMS = MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS();
  
  int numberOfProcedures = m_pkb->getProcTable()->getAllProcsName().size();
  for (int i = 0; i < numberOfProcedures; i++) {
    AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
    //get the bounds (first and last) stmt no from proc
    LIST_OF_STMT_NUMS bound = m_pkb->getStatementTable()->getStmtsFromProcIdx(i);
    STMT_NUM start = bound[0];
    STMT_NUM end = bound[bound.size() - 1];
    PAIR_OF_AFFECTS_LIST intermediateResult = m_affectsTable->getAffectsListsFromBounds(start, end);
    MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectedByList = intermediateResult.second;
    //add affectsList to LMS.
    //run the appendAffectsList method.
    LMS = appendAffectsList(affectedByList, LMS);
    delete m_affectsTable;
  }
  LIST_OF_AFFECTS_STMTS results = LIST_OF_AFFECTS_STMTS();
  for (auto itr = LMS.begin(); itr != LMS.end(); ++itr) {
    results.push_back(itr->first);
  }
  return results; //return all the keys affectedByList in LMS
}

BOOLEAN AffectsExtractor::extractIsAffectedByAnything(STMT_NUM t_usesLine) { // affects(_,12)
  //return empty if t_usesLine exceeds number of lines in program.
  if (m_pkb->getStatementTable()->getNumberOfStatements() < t_usesLine) {
    return false;
  }
  //return false if line exceeds number of lines in program.
  TOTAL_NUMBER_OF_STMTS totalLines = m_pkb->getStatementTable()->getNumberOfStatements();
  if (totalLines < t_usesLine) {
    return false;
  }
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  LIST_OF_STMT_NUMS bound = getListOfStmtsFromStmtNum(t_usesLine);
  STMT_NUM start = bound[0];
  STMT_NUM end = bound[bound.size() - 1];
  BOOLEAN hasAffects = m_affectsTable->hasAffectsFromBounds(start, t_usesLine, INVALID_INDEX, t_usesLine);
  delete m_affectsTable;
  return hasAffects;
}

BOOLEAN AffectsExtractor::extractIsAffectsAnything(STMT_NUM t_modifiesLine) { // affects(1,_)
  //return empty if t_modifiesLine exceeds number of lines in program.
  if (m_pkb->getStatementTable()->getNumberOfStatements() < t_modifiesLine) {
    return false;
  }
  //return false if line exceeds number of lines in program.
  TOTAL_NUMBER_OF_STMTS totalLines = m_pkb->getStatementTable()->getNumberOfStatements();
  if (totalLines < t_modifiesLine) {
    return false;
  }
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  LIST_OF_STMT_NUMS bound = getListOfStmtsFromStmtNum(t_modifiesLine);
  STMT_NUM start = bound[0];
  STMT_NUM end = bound[bound.size() - 1];
  BOOLEAN isAffectsAnything = m_affectsTable->hasAffectsFromBounds(t_modifiesLine, end, t_modifiesLine, INVALID_INDEX);
  return isAffectsAnything;
}



MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS AffectsExtractor::extractAllAffectsStar() { // affects*(a1,a2)
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS LMS = MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS();
  int numberOfProcedures = m_pkb->getProcTable()->getAllProcsName().size();
  for (int i = 0; i < numberOfProcedures; i++) {
    AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
    //get the bounds (first and last) stmt no from proc
    LIST_OF_STMT_NUMS bound = m_pkb->getStatementTable()->getStmtsFromProcIdx(i);
    STMT_NUM start = bound[0];
    STMT_NUM end = bound[bound.size() - 1];
    PAIR_OF_AFFECTS_LIST intermediateResult = m_affectsTable->getAffectsStarListsFromBounds(start, end);
    //initialise new affectsTable as to start processing a new procedure. 
    MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectsList = intermediateResult.first;
    //add affectsList to LMS.
    LMS = appendAffectsList(affectsList, LMS);
    delete m_affectsTable;
  }
  return LMS;
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectedByStar(STMT_NUM t_modifiesLine) { // affects*(2,a)
  if (m_pkb->getStatementTable()->getNumberOfStatements() < t_modifiesLine) {
    return{};
  }
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  LIST_OF_AFFECTS_STMTS affectsStmts = LIST_OF_AFFECTS_STMTS();
  LIST_OF_STMT_NUMS bound = getListOfStmtsFromStmtNum(t_modifiesLine);
  STMT_NUM start = bound[0];
  STMT_NUM end = bound[bound.size() - 1];
  PAIR_OF_AFFECTS_LIST intermediateResult = m_affectsTable->getAffectsStarListsFromBounds(start, end);
  MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectsList = intermediateResult.first;
  //return affectedByList.get(t_modifiesLine);
  auto itr = affectsList.find(t_modifiesLine);
  if (itr != affectsList.end()) {
    SET_OF_STMT_NUMS affectsSet = itr->second;
    //copy set to vector
    affectsStmts.insert(affectsStmts.end(), affectsSet.begin(), affectsSet.end());
    std::sort(affectsStmts.begin(), affectsStmts.end());
  }
  delete m_affectsTable;
  return affectsStmts;
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectsStar(STMT_NUM t_usesLine) { // affects*(a,12)
  //return empty if t_usesLine exceeds number of lines in program.
  if (m_pkb->getStatementTable()->getNumberOfStatements() < t_usesLine) {
    return{};
  }
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  LIST_OF_AFFECTS_STMTS affectsStmts = LIST_OF_AFFECTS_STMTS();
  LIST_OF_STMT_NUMS bound = getListOfStmtsFromStmtNum(t_usesLine);
  STMT_NUM start = bound[0];
  STMT_NUM end = bound[bound.size() - 1];
  PAIR_OF_AFFECTS_LIST intermediateResult = m_affectsTable->getAffectsStarListsFromBounds(start, end);
  MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectedByList = intermediateResult.second;
  auto itr = affectedByList.find(t_usesLine);
  if (itr != affectedByList.end()) {
    SET_OF_STMT_NUMS affectsSet = itr->second;
    //copy set to vector
    affectsStmts.insert(affectsStmts.end(), affectsSet.begin(), affectsSet.end());
    std::sort(affectsStmts.begin(), affectsStmts.end());
  }
  delete m_affectsTable;
  return affectsStmts;
}

BOOLEAN AffectsExtractor::extractIsAffectsStar(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) { // affects*(1,12)
  //return empty if either line exceeds number of lines in program.
  TOTAL_NUMBER_OF_STMTS totalLines = m_pkb->getStatementTable()->getNumberOfStatements();
  if (totalLines < t_modifiesLine || totalLines < t_usesLine) {
    return false;
  }
  //check if both are assignment statements.
  if (m_pkb->getStatementTable()->getTypeOfStatement(t_modifiesLine) != queryType::GType::ASGN || m_pkb->getStatementTable()->getTypeOfStatement(t_usesLine) != queryType::GType::ASGN) {
    return false;
  }
  //check if the two are in same proc.
  if (m_pkb->getStatementTable()->getProcNameFromStmtNum(t_modifiesLine) != m_pkb->getStatementTable()->getProcNameFromStmtNum(t_usesLine)) {
    return false;
  }
  //passes all the above checks, run the method.
  // Must change to affects* version
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  BOOLEAN isAffectsStar = m_affectsTable->isAffectsStar(t_modifiesLine, t_usesLine);
  delete m_affectsTable;
  return isAffectsStar;
}

BOOLEAN AffectsExtractor::extractHasAffectsRelationshipStar() { // affects*(_,_)
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  LIST_OF_PROC_NAMES procNames = m_pkb->getProcTable()->getAllProcsName();
  StatementTable *stmtTable = m_pkb->getStatementTable();
  for (auto& name : procNames) {
    PROC_INDEX procIdx = m_pkb->getProcTable()->getProcIdxFromName(name);
    LIST_OF_STMT_NUMS stmts = stmtTable->getStmtsFromProcIdx(procIdx);
    MAP_OF_VAR_NAME_TO_SET_OF_STMT_NUMS lms;
    if (m_affectsTable->hasAffectsFromBounds(stmts.front(), stmts.back(), stmts.front(), INVALID_INDEX)) {
      delete m_affectsTable;
      return true;
    }
  }
  delete m_affectsTable;
  return false;
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectsAnythingStar() {  // affects*(a,_)
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS LMS = MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS();
  int numberOfProcedures = m_pkb->getProcTable()->getAllProcsName().size();
  for (int i = 0; i < numberOfProcedures; i++) {
    AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
    //get the bounds (first and last) stmt no from proc
    LIST_OF_STMT_NUMS bound = m_pkb->getStatementTable()->getStmtsFromProcIdx(i);
    STMT_NUM start = bound[0];
    STMT_NUM end = bound[bound.size() - 1];
    PAIR_OF_AFFECTS_LIST intermediateResult = m_affectsTable->getAffectsStarListsFromBounds(start, end);
    MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectsList = intermediateResult.first;
    //add affectsList to LMS.
    //run the appendAffectsList method.
    LMS = appendAffectsList(affectsList, LMS);
    delete m_affectsTable;
  }
  LIST_OF_AFFECTS_STMTS results = LIST_OF_AFFECTS_STMTS();
  for (auto itr = LMS.begin(); itr != LMS.end(); ++itr) {
    results.push_back(itr->first);
  }
  return results; //return all the keys affectsList in LMS
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectedByAnythingStar() { // affects*(_,a)
  
  MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS LMS = MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS();
  int numberOfProcedures = m_pkb->getProcTable()->getAllProcsName().size();
  for (int i = 0; i < numberOfProcedures; i++) {
    AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
    //get the bounds (first and last) stmt no from proc
    LIST_OF_STMT_NUMS bound = m_pkb->getStatementTable()->getStmtsFromProcIdx(i);
    STMT_NUM start = bound[0];
    STMT_NUM end = bound[bound.size() - 1];
    PAIR_OF_AFFECTS_LIST intermediateResult = m_affectsTable->getAffectsStarListsFromBounds(start, end);
    MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS affectedByList = intermediateResult.second;
    //add affectsList to LMS.
    //run the appendAffectsList method.
    LMS = appendAffectsList(affectedByList, LMS);
    delete m_affectsTable;
  }
  LIST_OF_AFFECTS_STMTS results = LIST_OF_AFFECTS_STMTS();
  for (auto itr = LMS.begin(); itr != LMS.end(); ++itr) {
    results.push_back(itr->first);
  }
  return results; //return all the keys affectedByList in LMS
}

BOOLEAN AffectsExtractor::extractIsAffectsAnythingStar(STMT_NUM t_modifiesLine) { // affects*(1,_)
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  //return empty if t_modifiesLine exceeds number of lines in program.
  if (m_pkb->getStatementTable()->getNumberOfStatements() < t_modifiesLine) {
    return false;
  }
  //return false if line exceeds number of lines in program.
  TOTAL_NUMBER_OF_STMTS totalLines = m_pkb->getStatementTable()->getNumberOfStatements();
  if (totalLines < t_modifiesLine) {
    return false;
  }
  LIST_OF_STMT_NUMS bound = getListOfStmtsFromStmtNum(t_modifiesLine);
  STMT_NUM start = bound[0];
  STMT_NUM end = bound[bound.size() - 1];
  BOOLEAN hasAffects = m_affectsTable->hasAffectsFromBounds(t_modifiesLine, end, t_modifiesLine, INVALID_INDEX);
  delete m_affectsTable;
  return hasAffects;
}

BOOLEAN AffectsExtractor::extractIsAffectedByAnythingStar(STMT_NUM t_usesLine) { // affects*(_,12)
  //return empty if t_usesLine exceeds number of lines in program.
  if (m_pkb->getStatementTable()->getNumberOfStatements() < t_usesLine) {
    return false;
  }
  //return false if line exceeds number of lines in program.
  TOTAL_NUMBER_OF_STMTS totalLines = m_pkb->getStatementTable()->getNumberOfStatements();
  if (totalLines < t_usesLine) {
    return false;
  }
  AffectsTable *m_affectsTable = new AffectsTable(m_pkb);
  LIST_OF_STMT_NUMS bound = getListOfStmtsFromStmtNum(t_usesLine);
  STMT_NUM start = bound[0];
  STMT_NUM end = bound[bound.size() - 1];
  BOOLEAN hasAffectsAnything = m_affectsTable->hasAffectsFromBounds(start, t_usesLine, INVALID_INDEX, t_usesLine);
  delete m_affectsTable;
  return hasAffectsAnything;
}

MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS AffectsExtractor::appendAffectsList(MAP_OF_STMT_NUM_TO_SET_OF_STMT_NUMS toAdd, MAP_OF_STMT_NUM_TO_LIST_OF_STMT_NUMS result) {
  //for every key in toAdd
  for (auto itr_toAdd = toAdd.begin(); itr_toAdd != toAdd.end(); ++itr_toAdd) {
    STMT_NUM key = itr_toAdd->first;
    SET_OF_STMT_NUMS affects = itr_toAdd->second;
    //case 1: if key not found in result
    if (result.find(key) == result.end()) {
      //just emplace the whole set as vector into result
      LIST_OF_AFFECTS_STMTS affectsList;
      affectsList.insert(affectsList.end(), affects.begin(), affects.end());
      result.emplace(key, affectsList);
    } else {
      //case 2: if key found in result
      auto itr_result = result.find(key);
      LIST_OF_AFFECTS_STMTS affectsInResult = itr_result->second;
      //add the values in, sort and put back to the result
      affectsInResult.insert(affectsInResult.end(), affects.begin(), affects.end());
      std::sort(affectsInResult.begin(), affectsInResult.end());
      result[key] = affectsInResult;
    }
  }
  return result;
}

LIST_OF_STMT_NUMS AffectsExtractor::getListOfStmtsFromStmtNum(STMT_NUM t_stmtNum) {
  PROC_NAME procName = m_pkb->getStatementTable()->getProcNameFromStmtNum(t_stmtNum);
  PROC_INDEX procIdx = m_pkb->getProcTable()->getProcIdxFromName(procName);
  LIST_OF_STMT_NUMS result = LIST_OF_STMT_NUMS();
  result = m_pkb->getStatementTable()->getStmtsFromProcIdx(procIdx);
  return result;
}