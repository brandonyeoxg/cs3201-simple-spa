#include "AssignExtractor.h"

void AssignExtractor::extractDesign() {
  m_pkb->getAssignTable()->populateAssignToVarMap(m_pkb->getVarTable());
}