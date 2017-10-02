#pragma once
#include <unordered_map>
#include "nodes\AssignNode.h"
#include "GlobalTypeDef.h"
#include "VarTable.h"

/*
* Represents an entry in the AssignTable.
* The entry consist of an assignment node and a statement number.
*/
struct AssignData {
  AssignNode* m_assignNode;
  STMT_NUM m_assignStmt;
  AssignData(AssignNode* t_node, STMT_NUM t_stmt) : m_assignNode(t_node) , m_assignStmt(t_stmt) {}
};

/**
 * Represents the assignments statements made in the program in difference representation.
 *
 * @author Brandon Yeo
 * @date 7/9/2017
 */
class AssignTable {
public:

  /*
  * Inserts an assign statement into the table.
  * @param t_index the index of the variable.
  * @param t_node reference to an assign node in the AST.
  * @return the index to the assign table.
  */
  VAR_INDEX insertAssignRelation(const VAR_INDEX &t_index, AssignNode* t_node);
 
  void insertAssignStmt(STMT_NUM t_stmtNum);

  LIST_OF_STMT_NUMS& getAllAssignStmt();

  /*
  * Returns all assignment statements number that modifies the variable t_index.
  * @param t_index the index of the variable of interest.
  */
  std::list<STMT_NUM> getAllAssignStmtListByVar(VAR_INDEX t_index);
  
  /*
  * Returns all assignment statements number and AssignNode in AssignData that modifies the variable t_index.
  * @param t_index the index of the variable of interest.
  */
  std::list<AssignData> getAssignDataByVar(VAR_INDEX t_index);
  
  /*
  * Returns all assignment statements.
  */
  LIST_OF_STMT_NUMS getAllAssignStmtList();
  
  /*
  * Returns all assignment statements in a representation.
  * The representation is a variable mapped to all statement number under that variable.
  */
  std::unordered_map<VAR_NAME, std::list<STMT_NUM>> getAllVarInWithAssignStmtNum();
  
  /*
  * Returns all assignment statements in a representation.
  * The repsentation is a statement number mapped to the variable in that statement number.
  */
  std::unordered_map<STMT_NUM, VAR_NAME> getAllAssignStmtWithVar();
 
  /*
  * Returns all assignment statments in AssignData representation.
  */
  std::list<AssignData> getAssignData();

  /*
  * Populates the rest of the representation in the assignment table.
  * This method is to be called in the design extractor.
  */
  void populateAssignToVarMap(VarTable* t_varTable);
private:
  std::unordered_map<VAR_INDEX, std::list<AssignData>> m_data;
  std::unordered_map<VAR_NAME, std::list<STMT_NUM>> m_assignVarWithAssignStmtNum;
  std::list<AssignData> m_assignMapToVar;
  std::unordered_map<STMT_NUM, VAR_NAME> m_assignMapWithVar;

  LIST_OF_STMT_NUMS m_assignStmts;
};