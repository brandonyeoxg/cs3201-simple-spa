#pragma once
#include "PkbTablesOnly.h"
/**
 * Represents a wrapper which extracts further complicated design abstractions from the PKB tables.
 * Designs are extracted through the ExtractorFactory.
 *
 * @author Brandon
 * @date 15/10/2017
 */
class DesignExtractor {
public:
  DesignExtractor(PkbTablesOnly* t_pkb);

  /*
  * Called to extract all design abstractions from the PKB table.
  */
  void extractRestOfDesignAbstractions();

private:
  PkbTablesOnly* m_pkb;
};