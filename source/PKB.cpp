#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"

/**
* A constructor.
* Instantiates one VarTable, FollowTable, ParentTable each.
*/
PKB::PKB() {
  FollowTable *followTable = new FollowTable();
  ParentTable *parentTable = new ParentTable();
  VarTable *varTable = new VarTable();
}
int PKB::setProcToAST(PROC p, TNode* r) {
  return NULL;
}

TNode* PKB::getRootAST(PROC p) {
  return NULL;
}
