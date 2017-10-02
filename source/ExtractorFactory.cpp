#include "ExtractorFactory.h"
#include "ParentExtractor.h"
#include "AssignExtractor.h"

Extractor* ExtractorFactory::makeExtractor(designExtractor::designType t_type, PkbTablesOnly* t_pkb) {
  switch (t_type) {
  case designExtractor::designType::PARENT:
    return new ParentExtractor(t_pkb);
    break;
  case designExtractor::designType::ASSIGN:
    return new AssignExtractor(t_pkb);
    break;
  }
  return nullptr;
}