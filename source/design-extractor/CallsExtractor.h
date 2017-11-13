#pragma once
#include "Extractor.h"
#include <algorithm>

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
  void hasCyclicCall();
  void hasNonExistantCall();

private:
  void populateCallsIdx();
  void populateCallsStarMap();
  void populateCalledByStarMap();
  void populateAllCallsLists();
  void populateAllCallsMap();
};

