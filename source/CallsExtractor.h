#pragma once
#include "Extractor.h"
class CallsExtractor : public Extractor
{
public:
  CallsExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~CallsExtractor() {};

  void extractDesign();

private:
  void populateCallsStarMap();
  void populateCalledByStarMap();
};

