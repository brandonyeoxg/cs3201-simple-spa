#pragma once
#include <unordered_set>
#include "GlobalTypeDef.h"

/**
* Represents the modifies relationship for procedures.
*
* @author Brandon
* @date 29/9/2017
*/
class ModifiesP
{
public:
  ModifiesP() {};
  ~ModifiesP() {};

  void insertModifiesP(PROC_INDEX t_procIdx, const PROC_NAME& t_procName, VAR_INDEX t_varIdx, const VAR_NAME& t_varName);
  bool isModifiesP(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx); /*< Modifies("First", "x") */
  bool isModifiesInProc(PROC_INDEX t_idx); /*< Modifies("First", _) */
  LIST_OF_VAR_NAMES getVarNamesWithProcIdx(PROC_INDEX t_idx); /*< Modifies("First", x) */
  LIST_OF_PROC_NAMES getProcNamesWithVarIdx(VAR_INDEX t_idx); /*< Modifies(p, "x") */
  MAP_OF_PROC_TO_VAR& getAllProcToVar(); /*< Modifies(p, x) */
  LIST_OF_PROC_NAMES& getAllProcNames(); /*< Modifies(p, _) */

private:
  std::unordered_map<PROC_INDEX, LIST_OF_VAR_NAMES> m_procToVarNames;
  std::unordered_map<PROC_INDEX, VAR_HASH_SET> m_procToVarSet;
  std::unordered_map<VAR_INDEX, LIST_OF_PROC_NAMES> m_varToProcNames;
  MAP_OF_PROC_TO_VAR m_allProcNamesToVarNames;
  LIST_OF_PROC_NAMES m_procNames;

  void insertModifiesWithProcAsKey(PROC_INDEX t_procIdx, const VAR_NAME& t_varName);
  void insertModifiesWithProcAsKeyWithVarHash(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx);
  void insertModifiesWithVarAsKey(VAR_INDEX t_varIdx, const PROC_NAME& t_procName);
  void insertModifiesProcName(const PROC_NAME& t_name);
  void insertModifiesProcAndVarName(const PROC_NAME& t_procName, const VAR_NAME& t_varName);
};

