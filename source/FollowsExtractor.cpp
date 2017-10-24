#include "FollowsExtractor.h"

void FollowsExtractor::extractDesign() {
  populateFollowsMatrix();
}

void FollowsExtractor::populateFollowsMatrix() {
  FollowTable* followTable = m_pkb->getFollowTable();
  TOTAL_NUMBER_OF_STMTS number_of_stmts;
  number_of_stmts = m_pkb->getStatementTable()->getNumberOfStatements();
  followTable->populateFollowsMatrix(number_of_stmts);
}