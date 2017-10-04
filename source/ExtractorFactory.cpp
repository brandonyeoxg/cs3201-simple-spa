#include "ExtractorFactory.h"
#include "ParentExtractor.h"
#include "AssignExtractor.h"
#include "CallsExtractor.h"
#include "ModifiesPExtractor.h"
#include "UsesPExtractor.h"
#include "ModifiesExtractor.h"
#include "UsesExtractor.h"

Extractor* ExtractorFactory::makeExtractor(designExtractor::designType t_type, PkbTablesOnly* t_pkb) {
  switch (t_type) {
  case designExtractor::designType::PARENT:
    return new ParentExtractor(t_pkb);
  case designExtractor::designType::ASSIGN:
    return new AssignExtractor(t_pkb);
  case designExtractor::designType::CALLS:
    return new CallsExtractor(t_pkb);
  case designExtractor::designType::MODIFIESP:
    return new ModifiesPExtractor(t_pkb);
  case designExtractor::designType::USESP:
    return new UsesPExtractor(t_pkb);
  case designExtractor::designType::MODIFIES:
    return new ModifiesExtractor(t_pkb);
  case designExtractor::designType::USES:
    return new UsesExtractor(t_pkb);
  }
  return nullptr;
}