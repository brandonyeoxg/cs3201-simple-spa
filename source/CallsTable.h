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
  bool insertCallsStmt(STMT_NUM t_lineNum, PROC_NAME t_proc);
  bool isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);
  bool isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2);
  LIST_OF_PROC_NAMES getCalls(PROC_NAME t_proc2);
  LIST_OF_PROC_NAMES getCalledBy(PROC_NAME t_proc1);
  LIST_OF_PROC_NAMES getCallsStar(PROC_NAME t_proc2);
  LIST_OF_PROC_NAMES getCalledByStar(PROC_NAME t_proc1);
  std::unordered_map<PROC_NAME, PROC_NAME> getAllCalls();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> getAllCallsStar(); //calls*(proc1, proc2) 
  LIST_OF_PROC_NAMES getCallsAnything();  //calls(proc1, _)
  LIST_OF_PROC_NAMES getCallsStarAnything();  //calls*(proc1, _)
  LIST_OF_PROC_NAMES getCalledByAnything(); //calls(_, proc2)
  LIST_OF_PROC_NAMES getCalledByStarAnything(); //calls*(_, proc2)
  bool hasCallsRelationship();  //calls(_, _)
  bool isCallsAnything(PROC_NAME t_proc1);
  bool isCalledByAnything(PROC_NAME t_proc2);

  PROC_NAME getProcNameFromCallStmtNum(STMT_NUM t_lineNum);

  void populateCallsStarMap();
  void populateCalledByStarMap();

  CallsTable();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& getCallsMap();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& getCalledByMap();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& getCallsStarMap();
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES>& getCalledByStarMap();
private:
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_callsMap;
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_calledByMap;
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_callsStarMap;
  std::unordered_map<PROC_NAME, LIST_OF_PROC_NAMES> m_calledByStarMap;
  std::unordered_map<STMT_NUM, PROC_NAME> m_callsStmtMap;
  std::set<PROC_NAME> m_allCalls;
  std::set<PROC_NAME> m_allCalledBy;
};