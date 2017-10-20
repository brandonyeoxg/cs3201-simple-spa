#include "FollowsExtractor.h"

void FollowsExtractor::extractDesign() {
  TOTAL_NUMBER_OF_STMTS number_of_stmts;
  number_of_stmts = m_pkb->getStatementTable()->getNumberOfStatements();
  m_pkb->getFollowTable()->populateFollowsMatrix(number_of_stmts);
}