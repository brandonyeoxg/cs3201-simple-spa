#pragma once
#include <unordered_map>
#include "nodes\ProcedureNode.h"

typedef short INDEX_NO;

/**
 * Represents all procedures created in the SIMPLE program.
 *
 * @author Brandon
 * @date 6/9/2017
 */

class ProcTable
{
public:
  ProcTable(): m_tableCount(0) {};

  INDEX_NO insertIntoProcTable(ProcedureNode* t_proc);
  ProcedureNode* getProcTableFromIndex(INDEX_NO t_index);
private:
  std::unordered_map<INDEX_NO, ProcedureNode*> m_table;
  INDEX_NO m_tableCount;
};

