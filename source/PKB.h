#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "FollowTable.h"
#include "ParentTable.h"
#include "VarTable.h"

typedef short PROC;
const int VARIABLE_S1 = -1;
const int VARIABLE_S2 = -2;


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
  std::unordered_map<int, std::vector<int>> returnFollowTable(int s1, int s2);
  std::unordered_map<int, std::vector<int>> returnParentTable(int s1, int s2);
  std::unordered_map<std::string, std::vector<int>> returnVarTable(std::string var);

private:
  FollowTable* m_followTable;
  ParentTable* m_parentTable;
  VarTable* m_varTable;
};