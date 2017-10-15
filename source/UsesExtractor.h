#pragma once
#include "Extractor.h"

/*
* Handles extractions for the Uses relation.
*
* @author Brandon
* @date 8/10/2017
*/
class UsesExtractor : public Extractor {
public:
  UsesExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~UsesExtractor() {};

  void extractDesign();
private:
  void populateUsesByCallStatements();
};