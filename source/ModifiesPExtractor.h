#pragma once
#include "Extractor.h"
class ModifiesPExtractor : public Extractor
{
public:
  ModifiesPExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~ModifiesPExtractor() {};

  void extractDesign();
};

