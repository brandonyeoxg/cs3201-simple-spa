#pragma once
#include "ParentTable.h"
#include "PkbWriteOnly.h"
/**
 * Represents an extractor which extract complicated abstractions from AST.
 *
 * @author Brandon
 * @date 8/27/2017
 */
class DesignExtractor {
public:
  DesignExtractor(PkbWriteOnly* t_pkb);
  void extractRestOfDesignAbstractions();

private:
  PkbWriteOnly* m_pkb;
};