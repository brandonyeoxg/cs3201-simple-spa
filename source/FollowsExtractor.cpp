#include "FollowsExtractor.h"

void FollowsExtractor::extractDesign() {
  FollowTable* followTable = m_pkb->getFollowTable();
  TOTAL_NUMBER_OF_STMTS number_of_stmts;
  number_of_stmts = m_pkb->getStatementTable()->getNumberOfStatements();
  std::cout << "num.of.stmt? " << number_of_stmts << std::endl;
  followTable->populateFollowsMatrix(number_of_stmts);
}