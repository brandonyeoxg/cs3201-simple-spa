#pragma once
#include "Extractor.h"
class UsesExtractor : public Extractor {
public:
  UsesExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~UsesExtractor() {};

  void extractDesign();
private:
  void populateUsesByCallStatements();
};