#include "UsesPExtractor.h"

void UsesPExtractor::extractDesign() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  UsesP* usesP = m_pkb->getUsesP();
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
    LIST_OF_VAR_NAMES varUsesInP = usesP->getVarNamesWithProcIdx(procIdx);
    for (auto& pItr : procsCallingThis) {
      PROC_INDEX calledProcIdx = procTable->getProcIdxFromName(pItr);
      for (auto& vItr : varUsesInP) {
        VAR_INDEX varIdx = varTable->getIndexOfVar(vItr);
        usesP->insertUsesP(calledProcIdx, pItr, varIdx, vItr);
      }
    }
  }
}
