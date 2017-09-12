#pragma once
#include "ParentTable.h"
#include "PKB.h"
/**
 * Represents an extractor which extract complicated abstractions from AST.
 *
 * @author Brandon
 * @date 8/27/2017
 */
class DesignExtractor {
public:
  DesignExtractor(PKB* t_pkb);
  void extractRestOfDesignAbstractions();

private:
  PKB* m_pkb;
};