#pragma once
#include "Extractor.h"

namespace designExtractor {
  enum designType {
    PARENT,
    CALLS,
    MODIFIESP,
    USESP,
    MODIFIES,
    USES,
    NEXT,
    END_ENUM
  };
}

class ExtractorFactory
{
public:
  ExtractorFactory() {};
  ~ExtractorFactory() {};

  static Extractor* makeExtractor(designExtractor::designType t_type, PkbTablesOnly* t_pkb);

private:
};

