#pragma once
#include <unordered_map>
#include "nodes\AssignNode.h"
#include "GlobalTypeDef.h"
#include "VarTable.h"

struct AssignData {
  AssignNode* m_assignNode;
  STMT_NUM m_assignStmt;
  AssignData(AssignNode* t_node, STMT_NUM t_stmt) : m_assignNode(t_node) , m_assignStmt(t_stmt) {}
};

/**
 * Represents the assignments made in the program in a table.
 *
 * @author Brandon Yeo
 * @date 7/9/2017
 */
class AssignTable {
public:
  VAR_INDEX insertAssignRelation(const VAR_INDEX &t_index, AssignNode* t_node);
  std::list<STMT_NUM> getAllStmtListByVar(VAR_INDEX t_index);
  std::list<AssignData>& getAssignDataByVar(VAR_INDEX t_index);
  std::list<STMT_NUM> getAllStmtList();
  std::unordered_map<std::string, std::list<STMT_NUM>> getAllAssignStmtWithVar();

  std::list<AssignData>  getAssignData();

  void populateAssignToVarMap(VarTable* t_varTable);
private:
  std::unordered_map<VAR_INDEX, std::list<AssignData>> m_data;
  std::list<AssignData> m_assignMapToVar;
};