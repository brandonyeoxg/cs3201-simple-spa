#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include "DesignExtractor.h"
#include "ExtractorFactory.h"

DesignExtractor::DesignExtractor(PkbTablesOnly* t_pkb) {
  m_pkb = t_pkb;
}

void DesignExtractor::extractRestOfDesignAbstractions() {
  //populate the parent* maps.
  Extractor* extractor = ExtractorFactory::makeExtractor(designExtractor::designType::PARENT, m_pkb);
  extractor->extractDesign();
  delete extractor;

  extractor = ExtractorFactory::makeExtractor(designExtractor::designType::ASSIGN, m_pkb);
  extractor->extractDesign();
  delete extractor;
  //ExtractorFactory::makeExtractor(ExtractorType::)
  //m_pkb->populateParentedByStarMap();
  //m_pkb->populateParentStarMap();
  //m_pkb->populateAssignTableAbstractions();
}
