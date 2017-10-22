#pragma once
#include "FollowTable.h"
#include "ParentTable.h"
#include "AssignTable.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "CallsTable.h"
#include "ModifiesP.h"
#include "UsesP.h"
#include "StatementTable.h"
#include "ModifiesTable.h"
#include "UsesTable.h"
#include "pkb/relationshipTables/NextTable.h"
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