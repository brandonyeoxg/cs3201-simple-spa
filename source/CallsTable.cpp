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
  } else {
    //if not, first check if the existing vector consists s2; if it does, return false
    LIST_OF_PROC_NAMES procNames = m_callsMap[t_proc1];
    if (std::find(procNames.begin(), procNames.end(), t_proc2) != procNames.end()) {
      return false;
    }
  }
  //else, retrieve the existing vector, append, and put back to callsMap.
  //also for every existing vector, check if s1 exists. If it does, append s2.
  for (auto it = m_callsMap.begin(); it != m_callsMap.end(); ++it) {
    LIST_OF_PROC_NAMES vect = it->second;
    for (int i = 0; i < vect.size(); i++) {
      if (vect[i] == t_proc1) { //if s1 present in vector
        vect.push_back(t_proc2);
        //update callsMap
        m_callsMap[it->first] = vect;
        m_allCalls.insert(t_proc2);
      }
    }
  }

  //insertion to calledByMap
  //firstly, check if s2 exists as key in calledByMap.
  if (m_calledByMap.find(t_proc2) == m_calledByMap.end()) {
    //if it doesn't, simply emplace s1 as key.
    LIST_OF_PROC_NAMES procNames;
    procNames.push_back(t_proc1);
    auto iterator = m_calledByMap.find(t_proc1);
    if (iterator != m_calledByMap.end()) {
      LIST_OF_PROC_NAMES procNamesCalledByProc1 = iterator->second;
      procNames.insert(procNames.end(), procNamesCalledByProc1.begin(), procNamesCalledByProc1.end());
    }
    m_calledByMap.emplace(t_proc2, procNames);
  }
  return true;
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
