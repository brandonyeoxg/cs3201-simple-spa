#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "AST.h"
#include "FollowTable.h"
#include "ParentTable.h"
#include "VarTable.h"
#include "ASTBuilder.h"
#include "ProcTable.h"

const int VARIABLE_S1 = -1;
const int VARIABLE_S2 = -2;


class TNode;

//class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
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
  
  PROC_INDEX_NO insertProcToAST(ProcedureNode* t_node);
  ProcedureNode* getRootAST(PROC_INDEX_NO t_index);
  
  //ParentTable Methods
  bool insertParent(int s1, int s2);
  bool isParent(int s1, int s2);
  bool isParentStar(int s1, int s2);
  int getParentOf(int s2);
  std::vector<int> getChildrenOf(int s1);
  std::vector<int> getParentStarOf(int s2);
  std::vector<int> getChildrenStarOf(int s1);

private:
  FollowTable* m_followTable;
  ParentTable* m_parentTable;
  VarTable* m_varTable;
  ProcTable* m_procTable;
  std::unordered_map<int, std::string> m_statementTypeTable;
  std::unordered_map<int, std::string> m_typeOfStatementTable;

  AST m_programNode;
  ASTBuilder m_builder;
};