#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "FollowTable.h"
#include "ParentTable.h"
#include "VarTable.h"

using namespace std;
typedef short PROC;
const int VARIABLE_S1 = -1;
const int VARIABLE_S2 = -2;
const string VARIABLE = "VARIABLE";


class TNode;

//class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
  static int setProcToAST(PROC p, TNode* r);
  static TNode* getRootAST(PROC p);
  PKB();
  FollowTable* getFollowTable();
  ParentTable* getParentTable();
  VarTable* getVarTable();


private:
  FollowTable* m_followTable;
  ParentTable* m_parentTable;
  VarTable* m_varTable;
};