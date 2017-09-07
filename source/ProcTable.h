#pragma once
#include <vector>
#include "nodes\ProcedureNode.h"

typedef short PROC_INDEX_NO;

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
  ProcedureNode* getProcWithIndex(PROC_INDEX_NO t_index);
private:
  std::vector<std::pair<std::string, ProcedureNode*>> m_data;
};