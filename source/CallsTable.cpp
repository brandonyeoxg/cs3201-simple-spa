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
    procNames.push_back(t_proc2);
    m_callsMap.emplace(t_proc1, procNames);
    m_allCalls.insert(t_proc1);
    m_allCalledBy.insert(t_proc2);
  } else {
    //if not, first check if the existing vector consists s2; if it does, return false
    LIST_OF_PROC_NAMES procNames = m_callsMap[t_proc1];
    if (std::find(procNames.begin(), procNames.end(), t_proc2) != procNames.end()) {
      return false;
    } else {
      //append t_proc 2 to the vector.
      procNames.push_back(t_proc2);
      m_callsMap[t_proc1] = procNames;
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

bool CallsTable::isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2) {
  //if proc1 doesn't exist in callsMap, returns false.
  if (m_callsMap.find(t_proc1) == m_callsMap.end()) {
    return false;
  } else {
    LIST_OF_PROC_NAMES procNames = m_callsMap[t_proc1];
    if (std::find(procNames.begin(), procNames.end(), t_proc2) != procNames.end()) {
      return true;
    } else {
      return false;
    }
  }
}

/*
bool CallsTable::isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2) {
  
}
*/

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

/*
LIST_OF_PROC_NAMES CallsTable::getCallsStar(PROC_NAME t_proc2) {

}

LIST_OF_PROC_NAMES CallsTable::getCalledByStar(PROC_NAME t_proc1) {

}
*/

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

/*
std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> CallsTable::getAllCallsStar() {

}
*/

LIST_OF_PROC_NAMES CallsTable::getCallsAnything() {
  LIST_OF_PROC_NAMES procNames;
  //copy the m_allFollows set to values vector.
  procNames.assign(m_allCalls.begin(), m_allCalls.end());
  return procNames;
}

LIST_OF_PROC_NAMES CallsTable::getCalledByAnything() {
  LIST_OF_PROC_NAMES procNames;
  //copy the m_allFollows set to values vector.
  procNames.assign(m_allCalledBy.begin(), m_allCalledBy.end());
  return procNames;
}



/**
* A constructor.
* Instantiates unordered maps (hashmap) of procedure names to vector of procedure names associated.
*/
CallsTable::CallsTable() {
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_callsMap;
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_calledByMap;
  std::set<PROC_NAME> m_allCalls;
}

std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> CallsTable::getCallsMap() {
  return m_callsMap;
}

std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> CallsTable::getCalledByMap() {
  return m_calledByMap;
}
