#pragma once
#include "../IntermediateTable.h"
class IntermediateTableDriver : public IntermediateTable
{
public:
  IntermediateTableDriver() {};
  ~IntermediateTableDriver() {};

  void clearTable();
  void insertTable(const INTERMEDIATE_TABLE& t_results);
  void insertSynonymRow(std::vector<SYNONYM_NAME> m_synonymRow);
  bool TestHasSynonym(SYNONYM_NAME t_name);
  INTERMEDIATE_TABLE TestInsertOneSynonym(SYNONYM_NAME t_synonym, LIST_OF_RESULTS_INDICES t_results);
  INTERMEDIATE_TABLE TestInsertTwoSynonym(SYNONYM_NAME t_synonym1, SYNONYM_NAME t_synonym2, SET_OF_RESULTS_INDICES t_results);
};

