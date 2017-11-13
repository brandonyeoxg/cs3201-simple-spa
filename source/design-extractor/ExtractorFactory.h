#pragma once
#include "Extractor.h"


enum DESIGN_TYPE {
  FOLLOWS,
  PARENT,
  CALLS,
  MODIFIESP,
  USESP,
  MODIFIES,
  USES,
  NEXT,
  END_ENUM,
  AFFECTS
};

class ExtractorFactory
{
public:
  ExtractorFactory() {};
  ~ExtractorFactory() {};

  static Extractor* makeExtractor(DESIGN_TYPE t_type, PkbTablesOnly* t_pkb);

private:
};

