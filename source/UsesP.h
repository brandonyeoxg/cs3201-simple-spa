#pragma once
#include "GlobalTypeDef.h"
/**
* Represents the uses relationship for procedures.
* If a procedure A calls procedure B, the uses relationship of procedure B is also stored as belonging to procedure A in the table.
*
* @author Brandon
* @date 29/9/2017
*/
class UsesP
{
public:
  UsesP() {}
  ~UsesP() {}

  /**
  * Inserts the uses relation for procedures.
  *
  * @param t_procIdx the procedure index that has the uses relation.
  * @param t_procName the procedure name of the uses relation.
  * @param t_varIdx the variable index of the variable that is used.
  * @param t_varName the variable name of the variable that is used.
  */
  void insertUsesP(PROC_INDEX t_procIdx, const PROC_NAME& t_procName, VAR_INDEX t_varIdx, const VAR_NAME& t_varName);
  
  /**
  * Returns true if the procedure has a uses relation of that variable.
  * Used in the query evaluator for Uses("First", "x").
  *
  * @param t_procIdx the procedure index that has the uses relation.
  * @param t_varIdx the variable index of the variable that is used.
  */
  BOOLEAN isUsesP(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx);

  /**
  * Returns true if the procedure has a uses relation.
  * Used in the query evaluator for Uses("First", _).
  *
  * @param t_procIdx the procedure index that is checked.
  */
  BOOLEAN isUsesInProc(PROC_INDEX t_idx);

  /**
  * Returns the list of variable names that are used in the procedure specified.
  * Used in the query evaluator for Uses("First", x).
  *
  * @param t_procIdx the procedure index that is checked.
  */
  LIST_OF_VAR_NAMES getVarNamesWithProcIdx(PROC_INDEX t_idx);
  
  /**
  * Returns the list of procedure names that are used by the variable.
  * Used in the query evaluator for Uses(p, "x").
  *
  * @param t_varIdx the variable index that is checked.
  */
  LIST_OF_PROC_NAMES getProcNamesWithVarIdx(VAR_INDEX t_idx);
  
  /**
  * Returns a results of a set of procedures mapped to a list of variables that they uses.
  * Used in the query evaluator for Uses(p, x);
  *
  */
  MAP_OF_PROC_TO_VAR& getAllProcToVar();

  /**
  * Returns a list of procedures that uses something.
  * Used in the query evaluator for Uses(p, _)
  *
  */
  LIST_OF_PROC_NAMES& getAllProcNames();

private:
  std::unordered_map<PROC_INDEX, LIST_OF_VAR_NAMES> m_procToVarNames;
  std::unordered_map<PROC_INDEX, VAR_HASH_SET> m_procToVarSet;
  std::unordered_map<VAR_INDEX, LIST_OF_PROC_NAMES> m_varToProcNames;
  MAP_OF_PROC_TO_VAR m_allProcNamesToVarNames;
  LIST_OF_PROC_NAMES m_procNames;

  /**
  * Helper method to insert the uses relation for procedures with procedure as a key and variable name as the value.
  *
  * @param t_procIdx the procedure index that has the uses relation.
  * @param t_varName the variable name of the variable that is used.
  */
  void insertUsesWithProcAsKey(PROC_INDEX t_procIdx, const VAR_NAME& t_varName);
  
  /**
  * Helper method to insert the uses relation for procedures with procedure as a key and a variable index hash set as the value.
  *
  * @param t_procIdx the procedure index that has the uses relation.
  * @param t_varIdx the variable index of the variable that is used by the procedure.
  */
  void insertUsesWithProcAsKeyWithVarHash(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx);
  
  /**
  * Helper method to insert the uses relation for procedures with variable as a key and a procedure name as the value.
  *
  * @param t_varIdx the variable index that is used.
  * @param t_procName the name of the procedure that has the is usesrelation.
  */
  void insertUsesWithVarAsKey(VAR_INDEX t_varIdx, const PROC_NAME& t_procName);
  
  /**
  * Helper method to insert the procedure name that has a uses relation.
  *
  * @param t_name the name of the procedure that has the is uses relation.
  */
  void insertUsesProcName(const PROC_NAME& t_name);

  /**
  * Helper method to insert uses relation where the procedure name is mapped to a variable name.
  *
  * @param t_name the name of the procedure that has the is uses relation.
  */
  void insertUsesProcAndVarName(const PROC_NAME& t_procName, const VAR_NAME& t_varName);
};

