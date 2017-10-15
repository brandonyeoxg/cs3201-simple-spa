#pragma once
#include "Extractor.h"

/*
* Handles extractions for the Next relation.
*
* @author Brandon
* @date 8/10/2017
*/
class NextExtractor : public Extractor
{
public:
  NextExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~NextExtractor() {};

  void extractDesign();
};

