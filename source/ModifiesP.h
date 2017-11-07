#pragma once
#include <unordered_set>
#include "GlobalTypeDef.h"

/**
* Represents the modifies relationship for procedures.
* If a procedure A calls procedure B, the modifies relationship of procedure B is also stored as belonging to procedure A in the table.
*
* @author Brandon
* @date 29/9/2017
*/
class ModifiesP
{
public:
  ModifiesP() {};
  ~ModifiesP() {};

  /**
  * Inserts the modifies relation for procedures.
  * 
  * @param t_procIdx the procedure index that has the modifies relation.
  * @param t_procName the procedure name of the modifies relation.
  * @param t_varIdx the variable index of the variable that is modified.
  * @param t_varName the variable name of the variable that is modified.
  */
  void insertModifiesP(PROC_INDEX t_procIdx, PROC_NAME t_procName, VAR_INDEX t_varIdx, VAR_NAME t_varName);

  /**
  * Returns true if the procedure has a modifies relation of that variable.
  * Used in the query evaluator for Modifies("First", "x").
  *
  * @param t_procIdx the procedure index that has the modifies relation.
  * @param t_varIdx the variable index of the variable that is modified.
  */
  BOOLEAN isModifiesP(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx);

  /**
  * Returns true if the procedure has a modifies relation.
  * Used in the query evaluator for Modifies("First", _).
  *
  * @param t_procIdx the procedure index that is checked.
  */
  BOOLEAN isModifiesInProc(PROC_INDEX t_idx);

  /**
  * Returns the list of variable names that are modified in the procedure specified.
  * Used in the query evaluator for Modifies("First", x).
  *
  * @param t_procIdx the procedure index that is checked.
  */
  LIST_OF_VAR_NAMES getVarNamesWithProcIdx(PROC_INDEX t_idx);
  LIST_OF_VAR_INDICES getVarIndicesWithProcIdx(PROC_INDEX t_idx);

  /**
  * Returns the list of procedure names that are modified by the variable.
  * Used in the query evaluator for Modifies(p, "x").
  *
  * @param t_varIdx the variable index that is checked.
  */
  LIST_OF_PROC_NAMES getProcNamesWithVarIdx(VAR_INDEX t_varIdx);
  LIST_OF_PROC_INDICES getProcIndicesWithVarIdx(VAR_INDEX t_idx);

  /**
  * Returns a results of a set of procedures mapped to a list of variables that they modifies.
  * Used in the query evaluator for Modifies(p, x);
  * 
  */
  MAP_OF_PROC_TO_VAR getAllProcToVar(); 
  MAP_OF_PROC_INDEX_TO_VAR_INDEX& getAllProcToVarByIdx();

  /**
  * Returns a list of procedures that modifies something.
  * Used in the query evaluator for Modifies(p, _)
  * 
  */
  LIST_OF_PROC_NAMES getAllProcNames(); 
  LIST_OF_PROC_INDICES& getAllProcIndices();
private:
  std::unordered_map<PROC_INDEX, LIST_OF_VAR_NAMES> m_procToVarNames;
  MAP_OF_PROC_INDEX_TO_LIST_OF_VAR_INDICES m_procToVarIndices;
  std::unordered_map<PROC_INDEX, VAR_HASH_SET> m_procToVarSet;
  std::unordered_map<VAR_INDEX, LIST_OF_PROC_NAMES> m_varToProcNames;
  MAP_OF_VAR_INDEX_TO_LIST_OF_PROC_INDICES m_varToProcIndices;
  MAP_OF_PROC_TO_VAR m_allProcNamesToVarNames;
  MAP_OF_PROC_INDEX_TO_VAR_INDEX m_allProcIndicesToVarIndices;
  LIST_OF_PROC_NAMES m_procNames;
  LIST_OF_PROC_INDICES m_procIndices;

  /**
  * Helper method to insert the modifies relation for procedures with procedure as a key and variable name as the value.
  *
  * @param t_procIdx the procedure index that has the modifies relation.
  * @param t_varName the variable name of the variable that is modified.
  */
  void insertModifiesWithProcAsKey(PROC_INDEX t_procIdx, const VAR_NAME& t_varName, VAR_INDEX t_varIdx);
  
  /**
  * Helper method to insert the modifies relation for procedures with procedure as a key and a variable index hash set as the value.
  *
  * @param t_procIdx the procedure index that has the modifies relation.
  * @param t_varIdx the variable index of the variable that is modified by the procedure.
  */
  void insertModifiesWithProcAsKeyWithVarHash(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx);

  /**
  * Helper method to insert the modifies relation for procedures with variable as a key and a procedure name as the value.
  *
  * @param t_varIdx the variable index that is modified.
  * @param t_procName the name of the procedure that has the is modifies relation.
  */
  void insertModifiesWithVarAsKey(VAR_INDEX t_varIdx, const PROC_NAME& t_procName, PROC_INDEX t_procIdx);

  /**
  * Helper method to insert the procedure name that has a modifies relation.
  *
  * @param t_name the name of the procedure that has the is modifies relation.
  */
  void insertModifiesProcName(const PROC_NAME& t_name, PROC_INDEX t_procIdx);

  /**
  * Helper method to insert modifies relation where the procedure name is mapped to a variable name.
  *
  * @param t_name the name of the procedure that has the is modifies relation.
  */
  void insertModifiesProcAndVarName(const PROC_NAME& t_procName, const VAR_NAME& t_varName);
  void insertModifiesProcAndVarIndices(PROC_INDEX t_procIdx, VAR_INDEX t_varIdx);
};

