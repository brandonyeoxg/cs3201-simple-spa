#include "CallsExtractor.h"
#include "../pkb/relationshipTables/CallsTable.h"
#include "../exception/SyntaxCyclicCalls.h"
#include "../exception/SyntaxNonExistantProcedure.h"
void CallsExtractor::extractDesign() {
  populateCallsStarMap();
  populateCalledByStarMap();
  //Need to populate the calls* relationships first, so that the indices version can be populated in the below method.
  populateCallsIdx();
  hasCyclicCall();
  hasNonExistantCall();
  populateAllCallsLists();
  populateAllCallsMap();
}

/*A mechanism to check if there exists a situation where procedure A calls B,
* and B calls A (or transitive closure of such situation).
*/
void CallsExtractor::hasCyclicCall() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES callsStarMap = callsTable->getCalledByStarMapByIdx();
  //for every key in calls* map,
  for (auto it = callsStarMap.begin(); it != callsStarMap.end(); ++it) {
    //if the list contains the key, throw exception.
    PROC_INDEX key = it->first;
    LIST_OF_PROC_INDICES list = it->second;
    if (std::find(list.begin(), list.end(), key) != list.end()) {
      throw SyntaxCyclicCalls();
    }
  }
}

void CallsExtractor::hasNonExistantCall() {
  CallsTable *callTable = m_pkb->getCallsTable();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES calledByMap = callTable->getCalledByMapByIdx();
  if (calledByMap.find(INVALID_INDEX) != calledByMap.end()) {
    throw SyntaxNonExistantProcedure();
  }
}

void CallsExtractor::populateCallsIdx() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  ProcTable* procTable = m_pkb->getProcTable();
  callsTable->populateCallsByIdx(procTable);
}

void CallsExtractor::populateCallsStarMap() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  callsTable->populateCallsStarMap();
}

void CallsExtractor::populateCalledByStarMap() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  callsTable->populateCalledByStarMap();

}

void CallsExtractor::populateAllCallsLists() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  callsTable->populateAllCallsLists();
}

void CallsExtractor::populateAllCallsMap() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  callsTable->populateAllCallsMap();
}
