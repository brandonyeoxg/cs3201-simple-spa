#include "CallsExtractor.h"
#include "CallsTable.h"
void CallsExtractor::extractDesign() {
  populateCallsStarMap();
  populateCalledByStarMap();
  //Need to populate the calls* relationships first, so that the indices version can be populated in the below method.
  populateCallsIdx();
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