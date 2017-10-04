#include "UsesExtractor.h"

void UsesExtractor::extractDesign() {
  populateUsesByCallStatements();
}

void UsesExtractor::populateUsesByCallStatements() {
  CallsTable* callsTable = m_pkb->getCallsTable();
  StatementTable* statementTable = m_pkb->getStatementTable();
  ParentTable* parentTable = m_pkb->getParentTable();
  UsesP* usesP = m_pkb->getUsesP();
}