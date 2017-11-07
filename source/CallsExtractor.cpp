#include "CallsExtractor.h"
#include "CallsTable.h"
void CallsExtractor::extractDesign() {
  populateCallsStarMap();
  populateCalledByStarMap();
  //Need to populate the calls* relationships first, so that the indices version can be populated in the below method.
  populateCallsIdx();
}

/*A mechanism to check if there exists a situation where procedure A calls B,
* and B calls A (or transitive closure of such situation). 
*/
void CallsExtractor::hasLoopingCalls() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES callsStarMap = callsTable->getCalledByStarMapByIdx();
  //for every key in calls* map,
  for (auto it = callsStarMap.begin(); it != callsStarMap.end(); ++it) {
    //if the list contains the key, throw exception.
    PROC_INDEX key = it->first;
    LIST_OF_PROC_INDICES list = it->second;
    if (std::find(list.begin(), list.end(), key) != list.end()) {
      //throws exception here! @Brandon
    }
  }

}

void CallsExtractor::populateCallsIdx() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  ProcTable* procTable = m_pkb->getProcTable();
  callsTable->populateCallsByIdx(procTable);
}

void CallsExtractor::populateCallsStarMap() {
  //for every key in callsMap
  CallsTable* callsTable = m_pkb->getCallsTable();
  callsTable->populateCallsStarMap();
}

void CallsExtractor::populateCalledByStarMap() {
  //copy the calledByMap in first, then
  //for every key in calledByMap,
  //for every element in vector, if exists as key in calledByMap,
  //check that vector & append all distinct proc_names.
  //for every key in callsMap
  CallsTable* callsTable = m_pkb->getCallsTable();
  callsTable->populateCalledByStarMap();
  
}