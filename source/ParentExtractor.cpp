#include "ParentExtractor.h"
#include "pkb/relationshipTables/ParentTable.h"

void ParentExtractor::extractDesign() {
  populateParentedByStarMap();
  populateParentStarMap();
  populateParentMatrix();
  populateParentAnythingRelationships();
}

void ParentExtractor::populateParentedByStarMap() {
  ParentTable* parentTable = m_pkb->getParentTable();
  parentTable->populateParentedByStarMap();
}

void ParentExtractor::populateParentStarMap() {
  ParentTable* parentTable = m_pkb->getParentTable();
  parentTable->populateParentStarMap();
}

void ParentExtractor::populateParentMatrix() {
  TOTAL_NUMBER_OF_STMTS number_of_stmts;
  number_of_stmts = m_pkb->getStatementTable()->getNumberOfStatements();
  ParentTable* parentTable = m_pkb->getParentTable();
  parentTable->populateParentMatrix(number_of_stmts);
}

void ParentExtractor::populateParentAnythingRelationships() {
  ParentTable* parentTable = m_pkb->getParentTable();
  parentTable->populateParentAnythingRelationships();
}