#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../../GlobalTypeDef.h"

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
  LIST_OF_PROC_NAMES& getAllProcsName();
  LIST_OF_PROC_INDICES& getAllProcsIndices();

private:
  LIST_OF_PROC_NAMES m_procIdxToName;
  LIST_OF_PROC_INDICES m_allProcIndices;
  MAP_OF_PROC_NAMES_TO_PROC_INDEX m_procNameToIdx;
};