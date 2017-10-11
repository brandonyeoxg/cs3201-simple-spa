#pragma once
#include "ParentTable.h"
#include "PkbTablesOnly.h"
/**
 * Represents an extractor which extract complicated abstractions from AST.
 *
 * @author Brandon
 * @date 8/27/2017
 */
class DesignExtractor {
public:
  DesignExtractor(PkbTablesOnly* t_pkb);
  void extractRestOfDesignAbstractions();

private:
  PkbTablesOnly* m_pkb;
};