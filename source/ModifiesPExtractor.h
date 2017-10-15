#pragma once
#include "Extractor.h"

/*
* Handles extractions for the Modifies for procedures relation.
*
* @author Brandon
* @date 8/10/2017
*/
class ModifiesPExtractor : public Extractor
{
public:
  ModifiesPExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~ModifiesPExtractor() {};

  void extractDesign();
};

