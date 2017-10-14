#pragma once
#include "DesignExtractor.h"
#include "ExtractorFactory.h"

using namespace designExtractor;

DesignExtractor::DesignExtractor(PkbTablesOnly* t_pkb) {
  m_pkb = t_pkb;
}

void DesignExtractor::extractRestOfDesignAbstractions() {
  for (designType type = designType::PARENT; type != designType::END_ENUM; type = designType(type + 1)) {
    Extractor *extractor = ExtractorFactory::makeExtractor(type, m_pkb);
    extractor->extractDesign();
    delete extractor;
  }
}
