#include "ExtractorFactory.h"
#include "ParentExtractor.h"
#include "CallsExtractor.h"
#include "ModifiesPExtractor.h"
#include "UsesPExtractor.h"
#include "ModifiesExtractor.h"
#include "UsesExtractor.h"
#include "NextExtractor.h"
#include "FollowsExtractor.h"
#include "AffectsExtractor.h"

Extractor* ExtractorFactory::makeExtractor(DESIGN_TYPE t_type, PkbTablesOnly* t_pkb) {
  switch (t_type) {
  case DESIGN_TYPE::FOLLOWS:
    return new FollowsExtractor(t_pkb);
  case DESIGN_TYPE::PARENT:
    return new ParentExtractor(t_pkb);
  case DESIGN_TYPE::CALLS:
    return new CallsExtractor(t_pkb);
  case DESIGN_TYPE::MODIFIESP:
    return new ModifiesPExtractor(t_pkb);
  case DESIGN_TYPE::USESP:
    return new UsesPExtractor(t_pkb);
  case DESIGN_TYPE::MODIFIES:
    return new ModifiesExtractor(t_pkb);
  case DESIGN_TYPE::USES:
    return new UsesExtractor(t_pkb);
  case DESIGN_TYPE::NEXT:
    return new NextExtractor(t_pkb);
  case DESIGN_TYPE::AFFECTS:
    return new AffectsExtractor(t_pkb);
  }
  return nullptr;
}