#include "NextExtractor.h"

void NextExtractor::extractDesign() {
  m_pkb->getNextTable()->executeAfterAllNextInserts();
}