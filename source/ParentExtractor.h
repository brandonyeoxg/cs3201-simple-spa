#pragma once
#include "Extractor.h"

/*
* Handles extractions for the Parent relation.
*
* @author Brandon
* @date 8/10/2017
*/
class ParentExtractor : public Extractor
{
public:
  ParentExtractor(PkbTablesOnly *t_pkb) : Extractor(t_pkb) {};
  ~ParentExtractor() {};

  void extractDesign();

private:
  void populateParentedByStarMap();
  void populateParentStarMap();
  void populateParentMatrix(TOTAL_NUMBER_OF_STMTS total);
};

