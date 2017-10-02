#pragma once
#include "ParentTable.h"
#include "AssignTable.h"
#include "VarTable.h"
/**
* Represents an interface for tables that resides in the PKB.
* The methods listed in this API is used by the Design Extractor
*
* @date 1/10/2017
*
*/
class PkbTablesOnly {
public:
  virtual ParentTable* getParentTable() = 0;
  virtual AssignTable* getAssignTable() = 0;
  virtual VarTable* getVarTable() = 0;
};