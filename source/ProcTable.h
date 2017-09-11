#pragma once
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include "nodes\ProcedureNode.h"
#include "ProcTableData.h"

typedef short PROC_INDEX_NO;
typedef int VAR_INDEX_NO;
const PROC_INDEX_NO INVALID_PROC_NO = -1;

/**
 * Represents a procedure table.
 * Stores the procedures in an indexed table.
 *
 * @author Brandon
 * @date 6/9/2017
 */
class ProcTable {
public:
  /**
   * Inserts a Procedure node into the procedure.
   * 
   * @param t_node should not be null
   * @return the index position of the node 
   */
  PROC_INDEX_NO insertProcByProcNode(ProcedureNode *t_node);

  /**
  * Gets procedure from the table using the index.
  *
  * @param t_node should not be null
  * @return the index position of the node
  */
  ProcTableData* getProcTableDataWithName(std::string& procName);

  /**
   * Gets procedure index number by using the procedure name.
   *
   * @author Brandon
   * @date 11/9/2017
   *
   * @param [in] procName Name of the proc.
   *
   * @return The proc index number with name.
   */
  PROC_INDEX_NO getProcIdxNumWithName(std::string& procName);

  ProcedureNode* getProcNodeWithIdx(PROC_INDEX_NO& t_procIdx);

  std::string getProcNameWithIdx(PROC_INDEX_NO& t_procIdx);

  bool insertModifies(PROC_INDEX_NO& t_procIdx, std::string& t_varIdx);
  bool isModifies(PROC_INDEX_NO& t_procIdx, std::string& t_varName);
  std::list<std::string>& getProcOfVarModifies(PROC_INDEX_NO& t_procIdx);
  std::list<std::string>& getProcNameThatModifiesVar(std::string& t_varName);
  std::unordered_map<std::string, std::list<std::string>>& getProcAndVarModifies();
  bool isModifiesInProc(std::string& t_procName);
  std::list<std::string>& getProcNameThatModifies();

  bool insertUses(PROC_INDEX_NO& t_procIdx, std::string& t_varIdx);
  bool isUses(PROC_INDEX_NO& t_procIdx, std::string& t_varName);
  std::list<std::string>& getProcOfVarUses(PROC_INDEX_NO& t_procIdx);
  std::list<std::string>& getProcNameThatUsesVar(std::string& t_varName);
  std::unordered_map<std::string, std::list<std::string>>& getProcAndVarUses();
  bool isUsesInProc(std::string& t_procName);
  std::list<std::string>& getProcNameThatUses();

  void convertProcTableSetToList();
private:
  std::vector<ProcTableData> m_data;

  std::unordered_map<std::string, std::list<std::string>> m_varToProcLookUpForModifies;
  std::unordered_map<std::string, std::list<std::string>> m_varToProcLookUpForUses;
  std::unordered_map<std::string, std::list<std::string>> m_procWithVarNamesModifies;
  std::unordered_map<std::string, std::list<std::string>> m_procWithVarNamesUses;

  std::list<std::string> m_procThatModifies;
  std::list<std::string> m_procThatUses;
};