#pragma once
#include "Extractor.h"

namespace designExtractor {
  enum designType {
    PARENT,
    ASSIGN,
    CALLS,
    MODIFIESP,
    USESP
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

