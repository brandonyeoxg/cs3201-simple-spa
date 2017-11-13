#pragma once
#include "relationshipTables/FollowTable.h"
#include "relationshipTables/ParentTable.h"
#include "relationshipTables/AssignTable.h"
#include "relationshipTables/VarTable.h"
#include "relationshipTables/ProcTable.h"
#include "relationshipTables/CallsTable.h"
#include "relationshipTables/ModifiesP.h"
#include "relationshipTables/UsesP.h"
#include "relationshipTables/StatementTable.h"
#include "relationshipTables/ModifiesTable.h"
#include "relationshipTables/UsesTable.h"
#include "relationshipTables/NextTable.h"
/**
* Represents an interface for tables that resides in the PKB.
* The methods listed in this API is used by the Design Extractor
*
* @date 1/10/2017
*
*/
class PkbTablesOnly {
public:
  virtual FollowTable* getFollowTable() = 0;
  virtual ParentTable* getParentTable() = 0;
  virtual AssignTable* getAssignTable() = 0;
  virtual ProcTable* getProcTable() = 0;
  virtual VarTable* getVarTable() = 0;
  virtual CallsTable* getCallsTable() = 0;
  virtual ModifiesP* getModifiesP() = 0;
  virtual UsesP* getUsesP() = 0;
  virtual StatementTable* getStatementTable() = 0;
  virtual ModifiesTable* getModifiesTable() = 0;
  virtual UsesTable* getUsesTable() = 0;
  virtual NextTable* getNextTable() = 0;
};