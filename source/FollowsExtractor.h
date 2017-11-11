#pragma once
#include "Extractor.h"

/*
* Handles extractions for the Follows relation.
*
*/
class FollowsExtractor : public Extractor
{
public:
  FollowsExtractor(PkbTablesOnly* t_pkb) : Extractor(t_pkb) {};
  ~FollowsExtractor() {};

  void extractDesign();
private:
  void populateFollowsMatrix();
  void populateAllFollowsMap();
  void populateFollowsAnythingRelationships();
};

