#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "ProcTable.h"
#include "FollowTable.h"
#include "ParentTable.h"
#include "VarTable.h"
#include "AST.h"
#include "ASTBuilder.h"
#include "nodes\TNode.h"
#include "nodes\ProcedureNode.h"

typedef short PROC_INDEX_NO;
const int VARIABLE_S1 = -1;
const int VARIABLE_S2 = -2;

class PKB {
public:
  PKB();
  ~PKB();
  FollowTable* getFollowTable();
  ParentTable* getParentTable();
  VarTable* getVarTable();
  std::unordered_map<int, std::vector<int>> returnFollowTable(int s1, int s2);
  std::unordered_map<int, std::vector<int>> returnParentTable(int s1, int s2);
  std::unordered_map<std::string, std::vector<int>> returnVarTable(std::string var);
  
  PROC_INDEX_NO insertProc(ProcedureNode* t_node);
  int setProcToAST(PROC_INDEX_NO t_index, ProcedureNode* t_node);
  TNode* getRootAST(PROC_INDEX_NO t_index);
private:
  FollowTable* m_followTable;
  ParentTable* m_parentTable;
  VarTable* m_varTable;
  ProcTable* m_procTable;

  ASTBuilder m_builder;
  AST m_programAst; /*< Brandon stuff */
};