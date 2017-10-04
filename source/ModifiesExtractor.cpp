#include "ModifiesExtractor.h"

void ModifiesExtractor::extractDesign() {
  populateModifiesByCallStatements();
}

void ModifiesExtractor::populateModifiesByCallStatements() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  StatementTable* statementTable = m_pkb->getStatementTable();
  ParentTable* parentTable = m_pkb->getParentTable();
  ModifiesP* modifiesP = m_pkb->getModifiesP();
  ModifiesTable* modifiesTable = m_pkb->getModifiesTable();
  ProcTable* procTable = m_pkb->getProcTable();

  //get all call statements
  LIST_OF_STMT_NUMS allCallStmts = statementTable->getListOfStatements(queryType::GType::CALL);

  //for every call statement
  for (int i = 0; i < allCallStmts.size(); i++) {
    //1. Get proc name from call Stmt no.
    PROC_NAME procName = callsTable->getProcNameFromCallStmtNum(allCallStmts[i]); //unit test covered
    //2. Get proc index from proc name.
    PROC_INDEX procIdx = procTable->getProcIdxFromName(procName);
    //3. Get list of var names involved for modifies in the procedure.
    LIST_OF_VAR_NAMES listOfModifiesVarNames = modifiesP->getVarNamesWithProcIdx(procIdx);
    LIST_OF_STMT_NUMS allParentStars = parentTable->getParentStarOf(allCallStmts[i]);

    for (int j = 0; j < allParentStars.size(); j++) {
      //for every parent* of the statement
      //set all these varNames for parent*
      for (int k = 0; k < listOfModifiesVarNames.size(); k++) {
        modifiesTable->insertModifiesForStmt(listOfModifiesVarNames[k], allParentStars[j]);
      }
    }
  }
}