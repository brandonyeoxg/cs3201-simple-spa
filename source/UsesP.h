#pragma once
#include "GlobalTypeDef.h"
/**
* Represents the modifies relationship for procedures.
*
* @author Brandon
* @date 29/9/2017
*/
class UsesP
{
public:
  UsesP() {}
  ~UsesP() {}

  void insertUsesP(PROC_INDEX t_procIdx, const PROC_NAME& t_procName, VAR_INDEX t_varIdx, const VAR_NAME& t_varName);
  bool isUsesP(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx); /*< Uses("First", "x") */
  bool isUsesInProc(PROC_INDEX t_idx); /*< Uses("First", _) */
  LIST_OF_VAR_NAMES getVarNamesWithProcIdx(PROC_INDEX t_idx); /*< Uses("First", x) */
  LIST_OF_PROC_NAMES getProcNamesWithVarIdx(VAR_INDEX t_idx); /*< Uses(p, "x") */
  MAP_OF_PROC_TO_VAR& getAllProcToVar(); /*< Uses(p, x) */
  LIST_OF_PROC_NAMES& getAllProcNames(); /*< Uses(p, _) */

private:
  std::unordered_map<PROC_INDEX, LIST_OF_VAR_NAMES> m_procToVarNames;
  std::unordered_map<PROC_INDEX, VAR_HASH_SET> m_procToVarSet;
  std::unordered_map<VAR_INDEX, LIST_OF_PROC_NAMES> m_varToProcNames;
  MAP_OF_PROC_TO_VAR m_allProcNamesToVarNames;
  LIST_OF_PROC_NAMES m_procNames;

  void insertUsesWithProcAsKey(PROC_INDEX t_procIdx, const VAR_NAME& t_varName);
  void insertUsesWithProcAsKeyWithVarHash(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx);
  void insertUsesWithVarAsKey(VAR_INDEX t_varIdx, const PROC_NAME& t_procName);
  void insertUsesProcName(const PROC_NAME& t_name);
  void insertUsesProcAndVarName(const PROC_NAME& t_procName, const VAR_NAME& t_varName);
};

