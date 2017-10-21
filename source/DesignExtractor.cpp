#pragma once
#include "DesignExtractor.h"
#include "ExtractorFactory.h"

DesignExtractor::DesignExtractor(PkbTablesOnly* t_pkb) {
  m_pkb = t_pkb;
}

void DesignExtractor::extractRestOfDesignAbstractions() {
  for (DESIGN_TYPE type = DESIGN_TYPE::FOLLOWS; type != DESIGN_TYPE::END_ENUM; type = DESIGN_TYPE(type + 1)) {
    Extractor *extractor = ExtractorFactory::makeExtractor(type, m_pkb);
    extractor->extractDesign();
    delete extractor;
  }
}
