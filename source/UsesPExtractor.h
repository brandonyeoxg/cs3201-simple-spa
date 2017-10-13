#pragma once
#include "Extractor.h"
class UsesPExtractor : public Extractor
{
public:
  UsesPExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~UsesPExtractor() {};

  void extractDesign();
};

