#include "UsesExtractor.h"

void UsesExtractor::extractDesign() {
  populateUsesByCallStatements();
  populateUsesAnythingRelationships();
}

void UsesExtractor::populateUsesByCallStatements() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  StatementTable* statementTable = m_pkb->getStatementTable();
  ParentTable* parentTable = m_pkb->getParentTable();
  UsesP* usesP = m_pkb->getUsesP();
  UsesTable* usesTable = m_pkb->getUsesTable();
  ProcTable* procTable = m_pkb->getProcTable();
  VarTable* varTable = m_pkb->getVarTable();
  
  //get all call statements
  LIST_OF_STMT_NUMS allCallStmts = statementTable->getListOfStatements(queryType::GType::CALL); //unit test covered

  //for every call statement
  for (int i = 0; i < allCallStmts.size(); i++) {
    //1. Get proc name from call Stmt no.
    PROC_NAME procName = callsTable->getProcNameFromCallStmtNum(allCallStmts[i]); //unit test covered
    //2. Get proc index from proc name.
    PROC_INDEX procIdx = procTable->getProcIdxFromName(procName);
    //3. Get list of var names involved for uses in the procedure.
    LIST_OF_VAR_NAMES listOfUsesVarNames = usesP->getVarNamesWithProcIdx(procIdx);
    LIST_OF_STMT_NUMS allParentStars = parentTable->getParentStarOf(allCallStmts[i]);

    //populate the uses for the call stmt itself.
    for (int j = 0; j < listOfUsesVarNames.size(); j++) {
      VAR_INDEX varIdx = varTable->getVarIdxFromName(listOfUsesVarNames[j]);
      usesTable->insertUsesForStmt(listOfUsesVarNames[j], allCallStmts[i], varIdx);
    }
    for (int k = 0; k < allParentStars.size(); k++) {
      //for every parent* of the statement
      //set all these varNames for parent*
      for (int l = 0; l < listOfUsesVarNames.size(); l++) {
        VAR_INDEX varIdx = varTable->getVarIdxFromName(listOfUsesVarNames[l]);
        usesTable->insertUsesForStmt(listOfUsesVarNames[l], allParentStars[k], varIdx);
      }
    }
  }
}

void UsesExtractor::populateUsesAnythingRelationships() {
  UsesTable* usesTable = m_pkb->getUsesTable();
  usesTable->populateUsesAnythingRelationships();
}