#pragma once
#include "Extractor.h"
class ModifiesExtractor : public Extractor {
public:
  ModifiesExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~ModifiesExtractor() {};

  void extractDesign();
private:
  void populateModifiesByCallStatements();
};