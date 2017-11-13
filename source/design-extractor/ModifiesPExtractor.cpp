#include "ModifiesPExtractor.h"

void ModifiesPExtractor::extractDesign() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  ModifiesP* modifiesP = m_pkb->getModifiesP();
  ProcTable* procTable = m_pkb->getProcTable();
  VarTable* varTable = m_pkb->getVarTable();
  // Go through each procedure in the calls star table
  auto calledByStarMap = callsTable->getCalledByStarMap();
  for (auto& calledItr : calledByStarMap) {
    PROC_NAME name = calledItr.first;
    auto procsCallingThis = calledItr.second;
    // Check if prroc Name inside
    PROC_INDEX procIdx = procTable->getProcIdxFromName(name);
    // Get the modifies stuff from 
    LIST_OF_VAR_NAMES varModifiesInP = modifiesP->getVarNamesWithProcIdx(procIdx);
    for (auto& pItr : procsCallingThis) {
      PROC_INDEX calledProcIdx = procTable->getProcIdxFromName(pItr);
      for (auto& vItr : varModifiesInP) {
        VAR_INDEX varIdx = varTable->getVarIdxFromName(vItr);
        modifiesP->insertModifiesP(calledProcIdx, pItr, varIdx, vItr);
      }
    }
  }
}