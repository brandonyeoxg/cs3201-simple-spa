#pragma once
#include "Extractor.h"

/**
* Handles extractions for the Calls relation.
*
* @author Brandon
* @date 8/10/2017
*/
class CallsExtractor : public Extractor
{
public:
  CallsExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~CallsExtractor() {};

  void extractDesign();

private:
  void populateCallsIdx();
  void populateCallsStarMap();
  void populateCalledByStarMap();
};

