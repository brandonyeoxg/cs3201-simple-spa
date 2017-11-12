#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "../../GlobalTypeDef.h"
#include "ProcTable.h"
/**
* Represents the calls relationship table. The table consists an unordered_map of line numbers mapped to vectors of line numbers that
track which line is followed by which lines.
*
*/
class CallsTable {
public:
  /**
  * Establish the calls(p1, p2) relationship.
  * @param t_proc1 the procedure that calls
  * @param t_proc2 the procedure being called
  * @return true if insertion is successfull.
  */
  BOOLEAN insertCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);
  /**
  * Inserts to the map that stores which stmt# calls which procedure.
  * @param t_lineNum the stmt# of the calls statement
  * @param t_proc the procedure being called
  * @return true if insertion is successful.
  */
  BOOLEAN insertCallsStmt(STMT_NUM t_lineNum, PROC_NAME t_proc);

  /**
  * Check if calls(p1, p2) is true.
  * @param t_proc1 a procedure name
  * @param t_proc2 a procedure name
  * @return true if calls(p1, p2) is true.
  */
  BOOLEAN isCalls(PROC_NAME t_proc1, PROC_NAME t_proc2);

  /**
  * Check if calls*(p1, p2) is true.
  * @param t_proc1 a procedure name
  * @param t_proc2 a procedure name
  * @return true if calls*(p1, p2) is true.
  */
  BOOLEAN isCallsStar(PROC_NAME t_proc1, PROC_NAME t_proc2);

  /**
  * Get the list of procedures that calls proc2.
  * i.e. calls(p, p2) where p2 is a known procedure
  * @param t_proc2 the procedure being called
  * @return the list of procedures that call t_proc2.
  */
  LIST_OF_PROC_NAMES getCalls(PROC_NAME t_proc2);
  LIST_OF_PROC_INDICES getCallsByIdx(PROC_INDEX t_proc2Idx);

  /**
  * Get the list of procedures that are called by proc1.
  * i.e. calls(p1, p) where p1 is a known procedure
  * @param t_proc1 the procedure that calls other procedures
  * @return the list of procedures that are called by t_proc1.
  */
  LIST_OF_PROC_NAMES getCalledBy(PROC_NAME t_proc1);
  LIST_OF_PROC_INDICES getCalledByByIdx(PROC_INDEX t_proc1Idx);

  /**
  * Get the list of procedures that directly/indirectly calls proc2.
  * i.e. calls*(p, p2) where p2 is a known procedure
  * @param t_proc2 the procedure being called directly/indirectly
  * @return the list of procedures that call* t_proc2.
  */
  LIST_OF_PROC_NAMES getCallsStar(PROC_NAME t_proc2);
  LIST_OF_PROC_INDICES getCallsStarByIdx(PROC_INDEX t_proc2Idx);

  /**
  * Get the list of procedures that are directly/indirectly called by proc1.
  * i.e. calls*(p1, p) where p1 is a known procedure
  * @param t_proc1 the procedure that directly/indirectly calls other procedures
  * @return the list of procedures that are called by t_proc1.
  */
  LIST_OF_PROC_NAMES getCalledByStar(PROC_NAME t_proc1);
  LIST_OF_PROC_INDICES getCalledByStarByIdx(PROC_INDEX t_proc1Idx);

  /**
  * Get the map of calls relationship i.e. for calls(p1, p2) where p1, p2 are synonyms.
  * @return the map that contains all calls relationship.
  */
  MAP_OF_PROC_NAMES getAllCalls();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES getAllCallsByIdx();

  /**
  * Get the map of calls* relationship i.e. for calls*(p1, p2) where p1, p2 are synonyms.
  * @return the map that contains all calls* relationship.
  */
  MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES getAllCallsStar();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES getAllCallsStarByIdx();

  /**
  * Get the list of procedures that calls any other procedures i.e. calls(p1, _).
  * @return the list of procedures.
  */
  LIST_OF_PROC_NAMES getCallsAnything();  //calls(proc1, _)
  LIST_OF_PROC_INDICES getCallsAnythingByIdx();

  /**
  * Get the list of procedures that call* any other procedures i.e. calls*(p1, _).
  * @return the list of procedures.
  */
  LIST_OF_PROC_NAMES getCallsStarAnything();
  LIST_OF_PROC_INDICES getCallsStarAnythingByIdx();

  /**
  * Get the list of procedures that called by any other procedures i.e. calls(_, p2).
  * @return the list of procedures.
  */
  LIST_OF_PROC_NAMES getCalledByAnything();
  LIST_OF_PROC_INDICES getCalledByAnythingByIdx();

  /**
  * Get the list of procedures that called by* any other procedures i.e. calls*(_, p2).
  * @return the list of procedures.
  */
  LIST_OF_PROC_NAMES getCalledByStarAnything();
  LIST_OF_PROC_INDICES getCalledByStarAnythingByIdx();

  /**
  * Check if there exists any calls relationship in the PKB.
  * @return true if there exists.
  */
  BOOLEAN hasCallsRelationship();

  /**
  * Checks if there exist any procedures that are called by proc1.
  * i.e. calls(p1, _) where p1 is a known procedure
  * @param t_proc1 the procedure that calls other procedures
  * @return true if calls(p1, _) holds.
  */
  BOOLEAN isCallsAnything(PROC_NAME t_proc1);

  /**
  * Checks if there exist any procedures that calls proc2.
  * i.e. calls(_, p2) where p2 is a known procedure
  * @param t_proc2 the procedure that are called by other procedures
  * @return true if there exists
  */
  BOOLEAN isCalledByAnything(PROC_NAME t_proc2);

  /**
  * Method that returns the procedure name given the stmt# of the call statement.
  * @param t_lineNum the stmt#
  * @return the procedure being called in the stmt#.
  */
  PROC_NAME getProcNameFromCallStmtNum(STMT_NUM t_lineNum);

  /**
  * Get the list of stmt# that calls a particular procedure.
  * @param t_procName the procedure being called.
  * @return the list of stmt#.
  */
  LIST_OF_STMT_NUMS getStmtNumsFromProcName(PROC_NAME t_procName);

  //methods called by the CallsExtractor in DE.
  void populateCallsByIdx(ProcTable* procTable);
  void populateCallsStarMap();
  void populateCalledByStarMap();
  void populateAllCallsLists();
  void populateAllCallsMap();

  CallsTable();
  ~CallsTable() {};
  /*
  * Getter methods for unit test purposes.
  */
  MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES& getCallsMap();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES& getCallsMapByIdx();
  MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES& getCalledByMap();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES& getCalledByMapByIdx();
  MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES& getCallsStarMap();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES& getCallsStarMapByIdx();
  MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES& getCalledByStarMap();
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES& getCalledByStarMapByIdx();
  MAP_OF_STMT_NUM_TO_PROC_NAME& getCallsStmtMap();
  MAP_OF_PROC_NAME_TO_LIST_OF_STMT_NUMS& getProcNameToCallsStmtsMap();

private:
  //data structures to store calls relationship
  MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES m_callsMap;
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES m_callsMapByIdx;
  MAP_OF_PROC_INDICES m_allCallsMapByIdx;

  MAP_OF_NAME_TO_SET_OF_NAMES m_callsSet;
  MAP_OF_PROC_INDEX_TO_SET_OF_PROC_INDICES m_callsSetByIdx;

  //data structures to store calls relationship (opposite direction)
  MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES m_calledByMap;
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES m_calledByMapByIdx;

  //data structures to store calls* relationship.
  MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES m_callsStarMap;
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES m_callsStarMapByIdx;

  MAP_OF_NAME_TO_SET_OF_NAMES m_callsStarSet;
  MAP_OF_PROC_INDEX_TO_SET_OF_PROC_INDICES m_callsStarSetByIdx;

  //data structures to store calls* relationship (opposite direction)
  MAP_OF_PROC_NAME_TO_LIST_OF_PROC_NAMES m_calledByStarMap;
  MAP_OF_PROC_INDEX_TO_LIST_OF_PROC_INDICES m_calledByStarMapByIdx;

  //data structure to store the stmt# that calls a certain procedure.
  MAP_OF_STMT_NUM_TO_PROC_NAME m_callsStmtMap;
  MAP_OF_STMT_NUM_TO_PROC_INDEX m_callsStmtMapByIdx;

  MAP_OF_PROC_NAME_TO_LIST_OF_STMT_NUMS m_procNameToCallsStmtsMap;
  MAP_OF_PROC_INDEX_TO_LIST_OF_STMT_NUMS m_procNameToCallsStmtMapByIdx;

  /*
  * stores all p that calls(p, _)
  */
  SET_OF_PROC_NAMES m_allCalls;
  SET_OF_PROC_INDICES m_allCallsByIdx;
  LIST_OF_PROC_INDICES m_allCallsByIdxList;

  SET_OF_PROC_NAMES m_allCalledBy;
  SET_OF_PROC_INDICES m_allCalledByByIdx;
  LIST_OF_PROC_INDICES m_allCalledByByIdxList;
};