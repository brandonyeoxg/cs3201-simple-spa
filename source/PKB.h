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
  //getter methods
  FollowTable* getFollowTable();
  ParentTable* getParentTable();
  VarTable* getVarTable();
  //setter methods
  void setFollowTable(std::unordered_map<int, std::vector<int>> &table);

  //to be removed
  std::unordered_map<int, std::vector<int>> returnFollowTable(int s1, int s2);
  std::unordered_map<int, std::vector<int>> returnParentTable(int s1, int s2);
  std::unordered_map<std::string, std::vector<int>> returnVarTable(std::string var);

  //FollowTable Methods
  bool insertFollows(int s1, int s2);
  bool isFollows(int s1, int s2);
  bool isFollowsStar(int s1, int s2);
  int getFollows(int s1);
  int getFollowedBy(int s2);
  std::vector<int> getFollowsStar(int s1);
  std::vector<int> getFollowedByStar(int s2);


private:
  FollowTable* m_followTable;
  ParentTable* m_parentTable;
  VarTable* m_varTable;
};