#include "CallsExtractor.h"
#include "CallsTable.h"
void CallsExtractor::extractDesign() {
  populateCallsStarMap();
  populateCalledByStarMap();
}

void CallsExtractor::populateCallsStarMap() {
  //for every key in callsMap
  CallsTable* callsTable = m_pkb->getCallsTable();
  VarTable* varTable = m_pkb->getVarTable();

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