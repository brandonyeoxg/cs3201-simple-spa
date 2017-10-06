#pragma once
#include "PkbTablesOnly.h"

class Extractor
{
public:
  Extractor(PkbTablesOnly* t_pkb) : m_pkb(t_pkb) {};

  virtual void extractDesign() = 0;
protected:
  PkbTablesOnly* m_pkb;
};

