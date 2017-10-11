#pragma once
#include "../IntermediateTable.h"
class IntermediateTableDriver : public IntermediateTable
{
public:
  IntermediateTableDriver() {};
  ~IntermediateTableDriver() {};

  void insertTable(const INTERMEDIATE_TABLE& t_results);
  void insertSynonymRow(std::vector<SYNONYM_NAME> m_synonymRow);
  bool TestHasSynonym(SYNONYM_NAME t_name);
  INTERMEDIATE_TABLE TestInsertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS t_results);
};

