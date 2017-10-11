#pragma once
#include "Extractor.h"

class ParentExtractor : public Extractor
{
public:
  ParentExtractor(PkbTablesOnly *t_pkb) : Extractor(t_pkb) {};
  ~ParentExtractor() {};

  void extractDesign();

private:
  void populateParentedByStarMap();
  void populateParentStarMap();
  void populateParentedByStarMap(std::unordered_map<STMT_NUM, STMT_NUM>::iterator t_mapItr, ParentTable* t_table);
};

