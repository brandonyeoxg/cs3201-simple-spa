#pragma once
#include "pkb/PkbTablesOnly.h"

/*
* Represents an interface for an Extractor.
*
* @author Brandon
* @date 8/10/2017
*/
class Extractor
{
public:
  Extractor(PkbTablesOnly* t_pkb) : m_pkb(t_pkb) {};

  /*
  * Extracts the design of abstractions.
  */
  virtual void extractDesign() = 0;
protected:
  PkbTablesOnly* m_pkb;
};

