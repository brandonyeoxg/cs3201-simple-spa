#include "ModifiesExtractor.h"

void ModifiesExtractor::extractDesign() {
  populateModifiesByCallStatements();
}

void ModifiesExtractor::populateModifiesByCallStatements() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  StatementTable* statementTable = m_pkb->getStatementTable();
  ParentTable* parentTable = m_pkb->getParentTable();
  ModifiesP* ModifiesP = m_pkb->getModifiesP();
}