#pragma once
#include "Extractor.h"


enum DESIGN_TYPE {
  PARENT,
  CALLS,
  MODIFIESP,
  USESP,
  MODIFIES,
  USES,
  NEXT,
  END_ENUM
};

class ExtractorFactory
{
public:
  ExtractorFactory() {};
  ~ExtractorFactory() {};

  static Extractor* makeExtractor(DESIGN_TYPE t_type, PkbTablesOnly* t_pkb);

private:
};

