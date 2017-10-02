#pragma once
#include "Extractor.h"
class AssignExtractor : public Extractor
{
public:
  AssignExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~AssignExtractor() {};

  void extractDesign();
};

