#pragma once
#include <unordered_map>
#include "nodes\AssignNode.h"

typedef int VAR_INDEX_NO;
typedef int STMT_NO;

/**
 * Represents the assignments made in the program in a table.
 *
 * @author Brandon Yeo
 * @date 7/9/2017
 */
class AssignTable {
public:
  VAR_INDEX_NO insertAssignRelation(const VAR_INDEX_NO &t_index, AssignNode* t_node);
  std::list<STMT_NO> getAllStmtListByVar(VAR_INDEX_NO t_index);
  std::list<STMT_NO> getAllStmtList();
  std::unordered_map<VAR_INDEX_NO, std::list<STMT_NO>> getAllAssignStmtWithVar();
private:
  std::unordered_map<VAR_INDEX_NO, std::list<AssignNode*>> m_data;
};