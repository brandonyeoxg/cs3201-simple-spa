#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <stdexcept>
#include <algorithm>

#include "CallsTable.h"

bool CallsTable::insertCalls(PROC_NAME t_proc1, PROC_NAME t_proc2) {
  //insertion to callsMap
  if (m_callsMap.find(t_proc1) == m_callsMap.end()) {
    //if s1 is not present in callsMap
    LIST_OF_PROC_NAMES procNames;
    UNORDERED_SET_OF_NAMES procNamesSet;
    procNames.push_back(t_proc2);
    procNamesSet.insert(t_proc2);
    m_callsMap.emplace(t_proc1, procNames);
    m_callsSet.emplace(t_proc1, procNamesSet);
    m_allCalls.insert(t_proc1);
    m_allCalledBy.insert(t_proc2);
  } else {
    //if not, first check if the existing vector consists s2; if it does, return false
    LIST_OF_PROC_NAMES procNames = m_callsMap[t_proc1];
    UNORDERED_SET_OF_NAMES procNamesSet = m_callsSet[t_proc1];
   // if (std::find(procNames.begin(), procNames.end(), t_proc2) != procNames.end()) {
    if (procNamesSet.find(t_proc2) != procNamesSet.end()) {
      return false;
    } else {
      //append t_proc 2 to the vector.
      procNames.push_back(t_proc2);
      procNamesSet.insert(t_proc2);
      m_callsMap[t_proc1] = procNames;
      m_callsSet[t_proc1] = procNamesSet;
      m_allCalledBy.insert(t_proc2);
    }

  }
  
  //insertion to calledByMap
  //firstly, check if s2 exists as key in calledByMap.
  if (m_calledByMap.find(t_proc2) == m_calledByMap.end()) {
    //if it doesn't, simply emplace s1 as key.
    LIST_OF_PROC_NAMES procNames;
    procNames.push_back(t_proc1);
    m_calledByMap.emplace(t_proc2, procNames);
  } else {
    LIST_OF_PROC_NAMES procNames;
    auto iterator = m_calledByMap.find(t_proc2);
    procNames = iterator->second;
    procNames.push_back(t_proc1);
    m_calledByMap[t_proc2] = procNames;
  }
  return true;
}

/*
* Inserts a statement number to procedure name mapping into the callsStmtMap.
*/
bool CallsTable::insertCallsStmt(STMT_NUM t_lineNum, PROC_NAME t_proc) {
  auto itr = m_callsStmtMap.find(t_lineNum);
  if (itr != m_callsStmtMap.end()) {
    return false;
  } else {
    //insertion to procNameToCallsStmtsMap.
    LIST_OF_STMT_NUMS stmtNums;
    auto itr2 = m_procNameToCallsStmtsMap.find(t_proc);
    if (itr2 == m_procNameToCallsStmtsMap.end()) {
      stmtNums.push_back(t_lineNum);
      m_procNameToCallsStmtsMap.emplace(t_proc, stmtNums);
    } else {
      stmtNums = m_procNameToCallsStmtsMap[t_proc];
      stmtNums.push_back(t_lineNum);
      m_procNameToCallsStmtsMap[t_proc] = stmtNums;
    }
    //insertion to callsStmtMap.
    m_callsStmtMap.emplace(t_lineNum, t_proc);
    return true;
  }
}
bool CallsTable::isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2) {
  //if proc1 doesn't exist in callsMap, returns false.
  if (m_callsMap.find(t_proc1) == m_callsMap.end()) {
    return false;
  } else {
    //LIST_OF_PROC_NAMES procNames = m_callsMap[t_proc1];
    UNORDERED_SET_OF_NAMES procNamesSet = m_callsSet[t_proc1];
    //if (std::find(procNames.begin(), procNames.end(), t_proc2) != procNames.end()) {
    if (procNamesSet.find(t_proc2) != procNamesSet.end()) {
      return true;
    } else {
      return false;
    }
  }
}


bool CallsTable::isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2) {
  if (m_callsStarMap.find(t_proc1) == m_callsStarMap.end()) {
    //if proc1 is not present in callsStarMap
    return false;
  } else {
    //LIST_OF_PROC_NAMES procNames = m_callsStarMap[t_proc1];
    UNORDERED_SET_OF_NAMES procNamesSet = m_callsStarSet[t_proc1];
    //if (std::find(procNames.begin(), procNames.end(), t_proc2) != procNames.end()) {
    if (procNamesSet.find(t_proc2) != procNamesSet.end()) {
      //can be found in the vector
      return true;
    } else {
      return false;
    }
  }
}


LIST_OF_PROC_NAMES CallsTable::getCalls(PROC_NAME t_proc2) {
  LIST_OF_PROC_NAMES procNames;
  if (m_callsMap.find(t_proc2) != m_callsMap.end()) {
    procNames = m_callsMap[t_proc2];
    return procNames;
  } else {
    return procNames;
  }
}

LIST_OF_PROC_NAMES CallsTable::getCalledBy(PROC_NAME t_proc1) {
  LIST_OF_PROC_NAMES procNames;
  if (m_calledByMap.find(t_proc1) != m_calledByMap.end()) {
    procNames = m_calledByMap[t_proc1];
    return procNames;
  } else {
    return procNames;
  }
}


LIST_OF_PROC_NAMES CallsTable::getCallsStar(PROC_NAME t_proc2) {
  LIST_OF_PROC_NAMES procNames;
  if (m_callsStarMap.find(t_proc2) != m_callsStarMap.end()) {
    procNames = m_callsStarMap[t_proc2];
    return procNames;
  } else {
    return procNames;
  }
}


LIST_OF_PROC_NAMES CallsTable::getCalledByStar(PROC_NAME t_proc1) {
  LIST_OF_PROC_NAMES procNames;
  if (m_calledByStarMap.find(t_proc1) != m_calledByStarMap.end()) {
    procNames = m_calledByStarMap[t_proc1];
    return procNames;
  } else {
    return procNames;
  }
}


std::unordered_map<PROC_NAME, PROC_NAME> CallsTable::getAllCalls() {
  std::unordered_map<PROC_NAME, PROC_NAME> allCalls;
  for (auto it = m_callsMap.begin(); it != m_callsMap.end(); ++it) {
    LIST_OF_PROC_NAMES procNames = it->second;
    for (int i = 0; i < procNames.size(); i++) {
      allCalls.emplace(it->first, procNames[i]);
    }
  }
  return allCalls;
}


std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> CallsTable::getAllCallsStar() {
  return m_callsStarMap;
}


LIST_OF_PROC_NAMES CallsTable::getCallsAnything() {
  LIST_OF_PROC_NAMES procNames;
  //copy the m_allFollows set to values vector.
  procNames.assign(m_allCalls.begin(), m_allCalls.end());
  return procNames;
}

LIST_OF_PROC_NAMES CallsTable::getCallsStarAnything() {
  LIST_OF_PROC_NAMES procNames;
  procNames.assign(m_allCalls.begin(), m_allCalls.end());
  return procNames;
}

LIST_OF_PROC_NAMES CallsTable::getCalledByAnything() {
  LIST_OF_PROC_NAMES procNames;
  //copy the m_allFollows set to values vector.
  procNames.assign(m_allCalledBy.begin(), m_allCalledBy.end());
  return procNames;
}

LIST_OF_PROC_NAMES CallsTable::getCalledByStarAnything() {
  LIST_OF_PROC_NAMES procNames;
  //copy the m_allFollows set to values vector.
  procNames.assign(m_allCalledBy.begin(), m_allCalledBy.end());
  return procNames;
}

bool CallsTable::hasCallsRelationship() {
  if (m_callsMap.size() > 0) {
    return true;
  } else {
    return false;
  }
}

bool CallsTable::isCallsAnything(PROC_NAME t_proc1) {
  return (m_allCalls.find(t_proc1) != m_allCalls.end());
}

bool CallsTable::isCalledByAnything(PROC_NAME t_proc2) {
  return (m_allCalledBy.find(t_proc2) != m_allCalledBy.end());
}

PROC_NAME CallsTable::getProcNameFromCallStmtNum(STMT_NUM t_lineNum) {
  auto itr = m_callsStmtMap.find(t_lineNum);
  if (itr != m_callsStmtMap.end()) {
    return itr->second;
  } else {
    throw std::invalid_argument("Statement Number does not exist in CallStmtMap");;
  }
}

LIST_OF_STMT_NUMS CallsTable::getStmtNumsFromProcName(PROC_NAME t_procName) {
  LIST_OF_STMT_NUMS stmtNums;
  auto itr = m_procNameToCallsStmtsMap.find(t_procName);
  if (itr == m_procNameToCallsStmtsMap.end()) {
    return stmtNums;
  } else {
    return m_procNameToCallsStmtsMap[t_procName];
  }
}

void CallsTable::populateCallsStarMap() {
  //for every key in callsMap
  for (auto it = m_callsMap.begin(); it != m_callsMap.end(); ++it) {
    PROC_NAME proc = it->first;
    LIST_OF_PROC_NAMES procNames = it->second;
    UNORDERED_SET_OF_NAMES procNamesSet(procNames.begin(), procNames.end());
    m_callsStarMap.emplace(proc, procNames);
    m_callsStarSet.emplace(proc, procNamesSet);
    LIST_OF_PROC_NAMES procNamesStar = procNames;
    
    for (int i = 0; i < procNamesStar.size(); i++) {
      //for every child, if it can be found as a key in calls map, append all from it's mapped vector to children
      auto iterator = m_callsMap.find(procNamesStar[i]);
      if (iterator != m_callsMap.end()) {
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
    m_callsStarMap[proc] = procNamesStar;
    UNORDERED_SET_OF_NAMES procNamesStarSet(procNamesStar.begin(), procNamesStar.end());
    m_callsStarSet[proc] = procNamesStarSet;
  }
}

void CallsTable::populateCalledByStarMap() {
  //copy the calledByMap in first, then
  //for every key in calledByMap,
  //for every element in vector, if exists as key in calledByMap,
  //check that vector & append all distinct proc_names.
  //for every key in callsMap
  for (auto it = m_calledByMap.begin(); it != m_calledByMap.end(); ++it) {
    PROC_NAME proc = it->first;
    LIST_OF_PROC_NAMES procNames = it->second;
    m_calledByStarMap.emplace(proc, procNames);
    LIST_OF_PROC_NAMES procNamesStar = procNames;
    for (int i = 0; i < procNamesStar.size(); i++) {
      //for every child, if it can be found as a key in calls map, append all from it's mapped vector to children
      auto iterator = m_calledByMap.find(procNamesStar[i]);
      if (iterator != m_calledByMap.end()) {
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
    m_calledByStarMap[proc] = procNamesStar;
  }
}
/**
* A constructor.
* Instantiates unordered maps (hashmap) of procedure names to vector of procedure names associated.
*/
  CallsTable::CallsTable() {
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_callsMap;
  MAP_OF_NAME_TO_SET_OF_NAMES m_callsSet;
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_calledByMap;
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_callsStarMap;
  MAP_OF_NAME_TO_SET_OF_NAMES m_callsStarSet;
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_calledByStarMap;
  std::unordered_map<STMT_NUM, PROC_NAME> m_callsStmtMap;
  std::unordered_map<PROC_NAME, LIST_OF_STMT_NUMS> m_procNameToCallsStmtsMap;
  std::set<PROC_NAME> m_allCalls;
  std::set<PROC_NAME> m_allCalledBy;
}

std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& CallsTable::getCallsMap() {
  return m_callsMap;
}

std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& CallsTable::getCalledByMap() {
  return m_calledByMap;
}

std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& CallsTable::getCallsStarMap() {
  return m_callsStarMap;
}
std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& CallsTable::getCalledByStarMap() {
  return m_calledByStarMap;
}

std::unordered_map<STMT_NUM, PROC_NAME>& CallsTable::getCallsStmtMap() {
  return m_callsStmtMap;
}

std::unordered_map<PROC_NAME, LIST_OF_STMT_NUMS>& CallsTable::getProcNameToCallsStmtsMap() {
  return m_procNameToCallsStmtsMap;
}