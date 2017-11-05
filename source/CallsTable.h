#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "GlobalTypeDef.h"

/**
* Represents the calls relationship table. The table consists an unordered_map of line numbers mapped to vectors of line numbers that
track which line is followed by which lines.
*
*/
class CallsTable {
public:
  bool insertCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);
  bool insertCallsStmt(STMT_NUM t_lineNum, PROC_NAME t_proc);
  bool isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);
  bool isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2);
  LIST_OF_PROC_NAMES getCalls(PROC_NAME t_proc2);
  LIST_OF_PROC_INDICES getCallsByIdx(PROC_INDEX t_proc2Idx);
  LIST_OF_PROC_NAMES getCalledBy(PROC_NAME t_proc1);
  LIST_OF_PROC_INDICES getCalledByByIdx(PROC_INDEX t_proc1Idx);
  LIST_OF_PROC_NAMES getCallsStar(PROC_NAME t_proc2);
  LIST_OF_PROC_INDICES getCallsStarByIdx(PROC_INDEX t_proc2Idx);
  LIST_OF_PROC_NAMES getCalledByStar(PROC_NAME t_proc1);
  LIST_OF_PROC_INDICES getCalledByStarByIdx(PROC_INDEX t_proc1Idx);
  std::unordered_map<PROC_NAME, PROC_NAME> getAllCalls();
  MAP_OF_PROC_INDICES getAllCallsByIdx();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> getAllCallsStar(); //calls*(proc1, proc2) 
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES getAllCallsStarByIdx();
  LIST_OF_PROC_NAMES getCallsAnything();  //calls(proc1, _)
  LIST_OF_PROC_INDICES getCallsAnythingByIdx();
  LIST_OF_PROC_NAMES getCallsStarAnything();  //calls*(proc1, _)
  LIST_OF_PROC_INDICES getCallsStarAnythingByIdx();
  LIST_OF_PROC_NAMES getCalledByAnything(); //calls(_, proc2)
  LIST_OF_PROC_INDICES getCalledByAnythingByIdx();
  LIST_OF_PROC_NAMES getCalledByStarAnything(); //calls*(_, proc2)
  LIST_OF_PROC_INDICES getCalledByStarAnythingByIdx();
  bool hasCallsRelationship();  //calls(_, _)
  bool isCallsAnything(PROC_NAME t_proc1);
  bool isCalledByAnything(PROC_NAME t_proc2);

  PROC_NAME getProcNameFromCallStmtNum(STMT_NUM t_lineNum);
  LIST_OF_STMT_NUMS getStmtNumsFromProcName(PROC_NAME t_procName);

  void populateCallsStarMap();
  void populateCalledByStarMap();

  CallsTable();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& getCallsMap();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES& getCallsMapByIdx();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& getCalledByMap();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES& getCalledByMapByIdx();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& getCallsStarMap();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES& getCallsStarMapByIdx();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& getCalledByStarMap();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES& getCalledByStarMapByIdx();
  std::unordered_map<STMT_NUM, PROC_NAME>& getCallsStmtMap();
  std::unordered_map<PROC_NAME, LIST_OF_STMT_NUMS>& getProcNameToCallsStmtsMap();
private:
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_callsMap;
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES m_callsMapByIdx;

  MAP_OF_NAME_TO_SET_OF_NAMES m_callsSet;
  MAP_OF_PROC_INDEX_TO_SET_OF_PROC_INDICES m_callsSetByIdx;

  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_calledByMap;
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES m_calledByMapByIdx;

  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_callsStarMap;
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES m_callsStarMapByIdx;

  MAP_OF_NAME_TO_SET_OF_NAMES m_callsStarSet;
  MAP_OF_PROC_INDEX_TO_SET_OF_PROC_INDICES m_callsStarSetByIdx;

  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_calledByStarMap;
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES m_calledByStarMapByIdx;

  std::unordered_map<STMT_NUM, PROC_NAME> m_callsStmtMap;
  MAP_OF_STMT_NUM_TO_PROC_INDEX m_callsStmtMapByIdx;

  std::unordered_map<PROC_NAME, LIST_OF_STMT_NUMS> m_procNameToCallsStmtsMap;
  MAP_OF_PROC_INDEX_TO_LIST_OF_STMT_NUMS m_procNameToCallsStmtMapByIdx;

  std::set<PROC_NAME> m_allCalls;
  SET_OF_PROC_INDICES m_allCallsByIdx;

  std::set<PROC_NAME> m_allCalledBy;
  SET_OF_PROC_INDICES m_allCalledByByIdx;
};