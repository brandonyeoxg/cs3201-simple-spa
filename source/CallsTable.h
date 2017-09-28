#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include "GlobalTypeDef.h"

/**
* Represents the calls relationship table. The table consists an unordered_map of line numbers mapped to vectors of line numbers that
track which line is followed by which lines.
*
*/
class CallsTable {
public:
  bool insertCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);
  bool isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);
  bool isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2);
  PROC_NAME getCalls(PROC_NAME t_proc1);
  PROC_NAME gettCalledBy(PROC_NAME t_proc2);
  LIST_OF_PROC_NAMES getCallsStar(PROC_NAME t_proc1);
  LIST_OF_PROC_NAMES getCalledByStar(PROC_NAME t_proc2);
  std::unordered_map<PROC_NAME, PROC_NAME> getAllCalls();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> getAllCallsStar();
  LIST_OF_PROC_NAMES getCallsAnything();
  LIST_OF_PROC_NAMES getCalledByAnything();
  bool hasCallsRelationship();
  bool isCallsAnything();
  bool isCalledByAnything();  

  void populateParentStarMap();
  void populateParentedByStarMap(std::unordered_map<PROC_NAME, PROC_NAME>::iterator t_mapItr);

  CallsTable();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> getCallsMap();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> getCalledByMap();
private:
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_callsMap;
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_calledByMap;
  std::set<PROC_NAME> m_allCalls;
};