#include "AffectsExtractor.h"

void AffectsExtractor::extractDesign() {  
}

SET_OF_AFFECTS AffectsExtractor::extractAllAffects() { // affects(a1,a2)
  SET_OF_AFFECTS LMPS = SET_OF_AFFECTS();
  int numberOfProcedures = m_pkb->getProcTable()->getAllProcsName().size;
  for (int i = 0; i < numberOfProcedures; i++) {
    //for every procedure, create LMS
    SET_OF_AFFECTS LMS = SET_OF_AFFECTS();
    //get the bounds (first and last) stmt no from proc
    //pairOfStmtNum = getBoundFromProc(i);
    int first = 0;//stub
    int second = 1;//stub
    PAIR_OF_AFFECTS_LIST intermediateResult = m_affectsTable->getAffectsListsFromBounds(first, second);
    MAP_OF_STMT_NUMS affectsList = intermediateResult.first;
    //add affectsList to LMPS.
    //run the appendAffectsList method.
  }
  return{}; //return LMPS
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffects(STMT_NUM t_modifiesLine) { // affects(a,12)
  return{};
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectedBy(STMT_NUM t_usesLine) { // affects(2,a)
  return{};
}

BOOLEAN AffectsExtractor::extractIsAffects(STMT_NUM t_modifiesLine, STMT_NUM t_usesLine) { // affects(1,12)
  return false;
}

BOOLEAN AffectsExtractor::extractHasAffectsRelationship() { // affects(_,_)
  return false;
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectsAnything() { // affects(_,a)
  return{};
}

LIST_OF_AFFECTS_STMTS AffectsExtractor::extractAffectedByAnything() { // affects(a,_)
  return{};
}

BOOLEAN AffectsExtractor::extractIsAffectsAnything(STMT_NUM t_modifiesLine) { // affects(_,12)
  return false;
}

BOOLEAN AffectsExtractor::extractIsAffectedByAnything(STMT_NUM t_usesLines) { // affects(1,_)
  return false;
}

void AffectsExtractor::appendAffectsList(std::unordered_map<int, std::vector<int>> toAdd, std::unordered_map<int, std::vector<int>> result) {
  //for every key in toAdd
  for (auto itr_toAdd = toAdd.begin(); itr_toAdd != toAdd.end(); ++itr_toAdd) {
    STMT_NUM key = itr_toAdd->first;
    LIST_OF_AFFECTS_STMTS affects = itr_toAdd->second;
    //case 1: if key not found in result
    if (result.find(key) == result.end()) {
      //just emplace the whole vector into result
      result.emplace(key, itr_toAdd->second);
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
}