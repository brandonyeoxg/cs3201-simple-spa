#pragma once
#include "Extractor.h"

/*
* Handles extractions for the Modifies relation.
*
* @author Brandon
* @date 8/10/2017
*/
class ModifiesExtractor : public Extractor {
public:
  ModifiesExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~ModifiesExtractor() {};

  void extractDesign();
private:
  void populateModifiesByCallStatements();
  void populateModifiesAnythingRelationships();
};