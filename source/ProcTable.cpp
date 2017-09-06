#include "ProcTable.h"

INDEX_NO ProcTable::insertIntoProcTable(ProcedureNode* t_proc) {
  m_table.insert({ m_tableCount, t_proc });
  return m_table.size() - 1;
}

ProcedureNode* ProcTable::getProcTableFromIndex(INDEX_NO t_index) {
  return m_table.find(t_index)->second;
}