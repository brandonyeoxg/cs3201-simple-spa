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
  m_followTable = new FollowTable();
  m_parentTable = new ParentTable();
  m_varTable = new VarTable();
}

FollowTable* PKB:: getFollowTable() {
  return m_followTable;
}

ParentTable* PKB:: getParentTable() {
  return m_parentTable;
}

VarTable* PKB::getVarTable() {
  return m_varTable;
}

unordered_map<int, vector<int>> PKB:: returnFollowTable(int s1, int s2) {
  FollowTable* testFollowTable = getFollowTable();
  
  //check s1 and s2
  //if both are querying variables, return the whole followTable i.e. every possible follow relationships
  if (s1 == VARIABLE_S1 && s2 == VARIABLE_S2) {
    return testFollowTable->getFollowTable();
  }

  unordered_map<int, vector<int>> finalResult;
  //if only s1 is querying variable
  if (s1 == VARIABLE_S1) {
    vector<int> s1Results = testFollowTable->getS1(s2);
    finalResult.emplace(s2, s1Results);
  }
  else if (s2 == VARIABLE_S2) {
    vector<int> s2Results = testFollowTable->getS2(s1);
    finalResult.emplace(s1, s2Results);
  }

  return finalResult;
}


int PKB::setProcToAST(PROC p, TNode* r) {
  return NULL;
}

TNode* PKB::getRootAST(PROC p) {
  return NULL;
}
