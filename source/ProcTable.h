#pragma once
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include "nodes\ProcedureNode.h"
#include "ProcTableData.h"
#include "GlobalTypeDef.h"

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
   * Inserts a procedure into the table.
   * 
   * @param t_name should not be null
   * @return the index position of the procedure in the table.
   */
  PROC_INDEX insertProc(PROC_NAME t_name);

  /**
  * Gets the procedure name using the procedure id
  *
  * @param t_node should not be null
  * @return the index position of the node
  */
  std::string getProcNameFromIdx(PROC_INDEX t_idx);

  /**
  * Gets the procedure name using the procedure id
  *
  * @param t_node should not be null
  * @return the index position of the node
  */
  PROC_INDEX getProcIdxFromName(PROC_NAME t_name);

  ///**
  // * Gets procedure index number by using the procedure name.
  // *
  // * @author Brandon
  // * @date 11/9/2017
  // *
  // * @param [in] procName Name of the proc.
  // *
  // * @return The proc index number with name.
  // */
  //PROC_INDEX getProcIdxNumWithName(std::string& procName);

  //ProcedureNode* getProcNodeWithIdx(PROC_INDEX& t_procIdx);

  //std::string getProcNameWithIdx(PROC_INDEX& t_procIdx);

  //std::list<std::string> getAllProcNameInProgram();

  //bool insertModifies(PROC_INDEX& t_procIdx, std::string& t_varIdx);
  //bool isModifies(std::string& t_procIdx, std::string& t_varName); /*< Modifies("First", "x") */
  //std::list<std::string>& getVarFromProcModifies(PROC_INDEX& t_procIdx); /*< Modifies("First", x) */
  //std::list<std::string>& getProcNameThatModifiesVar(std::string& t_varName); /*< Modifies(p, "x") */
  //std::unordered_map<std::string, std::list<std::string>>& getProcAndVarModifies(); /*< Modifies("First", _) */
  //bool isModifiesInProc(std::string& t_procName); /*< Modifies("First", _) */
  //std::list<std::string>& getProcNameThatModifies(); /*< Modifies(p, _) */

  //bool insertUses(PROC_INDEX& t_procIdx, std::string& t_varIdx); 
  //bool isUses(std::string& t_procName, std::string& t_varName); /*< Uses("First", "x") */
  //std::list<std::string>& getVarFromProcUses(PROC_INDEX& t_procIdx); /*< Uses("First", x) */
  //std::list<std::string>& getProcNameThatUsesVar(std::string& t_varName); /*< Uses(p, "x") */
  //std::unordered_map<std::string, std::list<std::string>>& getProcAndVarUses(); /*< Uses(p, v) */
  //bool isUsesInProc(std::string& t_procName); /*< Uses("First", _) */
  //std::list<std::string>& getProcNameThatUses(); /*< Uses(p, _) */

  //void convertProcTableSetToList();
private:
  std::vector<PROC_NAME> m_procIdxToName;
  std::unordered_map<PROC_NAME, PROC_INDEX> m_procNameToIdx;
};