#pragma once
#include "Extractor.h"

/*
* Handles extractions for the Uses procedure relation.
*
* @author Brandon
* @date 8/10/2017
*/
class UsesPExtractor : public Extractor
{
public:
  UsesPExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~UsesPExtractor() {};

  void extractDesign();
};

