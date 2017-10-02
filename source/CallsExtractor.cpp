#include "CallsExtractor.h"
#include "CallsTable.h"
void CallsExtractor::extractDesign() {
  populateCallsStarMap();
  populateCalledByStarMap();
}

void CallsExtractor::populateCallsStarMap() {
  //for every key in callsMap
  CallsTable* callsTable = m_pkb->getCallsTable();
  for (auto it = callsTable->getCallsMap().begin(); it != callsTable->getCallsMap().end(); ++it) {
    PROC_NAME proc = it->first;
    LIST_OF_PROC_NAMES procNames = it->second;
    callsTable->getCallsStarMap().emplace(proc, procNames);
    LIST_OF_PROC_NAMES procNamesStar = procNames;
    for (int i = 0; i < procNamesStar.size(); i++) {
      //for every child, if it can be found as a key in calls map, append all from it's mapped vector to children
      auto iterator = callsTable->getCallsMap().find(procNamesStar[i]);
      if (iterator != callsTable->getCallsMap().end()) {
        //for ea element in listToCheck, 
        //append only if it is not present in procNamesStar vector.
        LIST_OF_PROC_NAMES listToCheck = iterator->second;
        LIST_OF_PROC_NAMES toBeAppended;
        for (int j = 0; j < listToCheck.size(); j++) {
          if (std::find(procNamesStar.begin(), procNamesStar.end(), listToCheck[j]) == procNamesStar.end()) {
            toBeAppended.push_back(listToCheck[j]);
          }
        }
        procNamesStar.reserve(procNamesStar.size() + toBeAppended.size());
        procNamesStar.insert(procNamesStar.end(), toBeAppended.begin(), toBeAppended.end());
      }
    }
    callsTable->getCallsStarMap()[proc] = procNamesStar;
  }
}

void CallsExtractor::populateCalledByStarMap() {
  //copy the calledByMap in first, then
  //for every key in calledByMap,
  //for every element in vector, if exists as key in calledByMap,
  //check that vector & append all distinct proc_names.
  //for every key in callsMap
  CallsTable* callsTable = m_pkb->getCallsTable();
  for (auto it = callsTable->getCalledByMap().begin(); it != callsTable->getCalledByMap().end(); ++it) {
    PROC_NAME proc = it->first;
    LIST_OF_PROC_NAMES procNames = it->second;
    callsTable->getCalledByStarMap().emplace(proc, procNames);
    LIST_OF_PROC_NAMES procNamesStar = procNames;
    for (int i = 0; i < procNamesStar.size(); i++) {
      //for every child, if it can be found as a key in calls map, append all from it's mapped vector to children
      auto iterator = callsTable->getCalledByMap().find(procNamesStar[i]);
      if (iterator != callsTable->getCalledByMap().end()) {
        //for ea element in listToCheck, 
        //append only if it is not present in procNamesStar vector.
        LIST_OF_PROC_NAMES listToCheck = iterator->second;
        LIST_OF_PROC_NAMES toBeAppended;
        for (int j = 0; j < listToCheck.size(); j++) {
          if (std::find(procNamesStar.begin(), procNamesStar.end(), listToCheck[j]) == procNamesStar.end()) {
            toBeAppended.push_back(listToCheck[j]);
          }
        }
        procNamesStar.reserve(procNamesStar.size() + toBeAppended.size());
        procNamesStar.insert(procNamesStar.end(), toBeAppended.begin(), toBeAppended.end());
      }
    }
    callsTable->getCalledByStarMap()[proc] = procNamesStar;
  }
}