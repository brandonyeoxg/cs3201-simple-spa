#pragma once
#include "Extractor.h"

/**
* Handles extractions for the Affects relation.
*
* @author Brandon
* @date 28/10/2017
*/
class AffectsExtractor : public Extractor
{
public:
  AffectsExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~AffectsExtractor() {};

  void extractDesign();
};

