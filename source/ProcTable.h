#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "nodes\ProcedureNode.h"
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
  * @param t_idx should not be a negtive number
  * @return the procedure name
  */
  PROC_NAME getProcNameFromIdx(PROC_INDEX t_idx);

  /**
  * Gets the procedure id using the procedure name
  *
  * @param t_name should not be null
  * @return the index position of the node
  */
  PROC_INDEX getProcIdxFromName(PROC_NAME t_name);

  /**
  * Reurns all procedure name in the program
  */
  std::vector<PROC_NAME>& getAllProcName();

  //bool insertUses(PROC_INDEX& t_procIdx, std::string& t_varIdx); 
  //bool isUses(std::string& t_procName, std::string& t_varName); /*< Uses("First", "x") */
  //std::list<std::string>& getVarFromProcUses(PROC_INDEX& t_procIdx); /*< Uses("First", x) */
  //std::list<std::string>& getProcNameThatUsesVar(std::string& t_varName); /*< Uses(p, "x") */
  //std::unordered_map<std::string, std::list<std::string>>& getProcAndVarUses(); /*< Uses(p, v) */
  //bool isUsesInProc(std::string& t_procName); /*< Uses("First", _) */
  //std::list<std::string>& getProcNameThatUses(); /*< Uses(p, _) */
private:
  std::vector<PROC_NAME> m_procIdxToName;
  std::unordered_map<PROC_NAME, PROC_INDEX> m_procNameToIdx;
};