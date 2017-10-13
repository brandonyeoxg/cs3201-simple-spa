#pragma once
#include "Extractor.h"

class NextExtractor : public Extractor
{
public:
  NextExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~NextExtractor() {};

  void extractDesign();
};

