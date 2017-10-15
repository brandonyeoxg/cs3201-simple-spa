#pragma once
#include <list>

#include "GlobalTypeDef.h"
#include "nodes\StmtListNode.h"
#include "nodes\VariableNode.h"
#include "nodes\AssignNode.h"
#include "nodes\ConstantNode.h"
#include "nodes\PlusNode.h"
#include "nodes\WhileNode.h"

/**
* Represents an interface for WriteOnly methods in the PKB.
* The methods listed in this API is used by the Parser.
*
* @author Brandon
* @date 24/9/2017
*
*/
class PkbWriteOnly {
public:

  ///////////////////////////////////////////////////////
  //  PKB building methods
  ///////////////////////////////////////////////////////
  /**
  * Inserts a procedure into the PKB. AST is built from this procedure node.
  * @param t_procName name of the procedure.
  * @return a reference to the StmtListNode created from inserting the procedure.
  */
  virtual PROC_INDEX insertProcedure(const PROC_NAME& t_procName) = 0;

  /**
  * Inserts a follows relation in the PKB.
  * @param t_node reference to the StmtLst that this statement belongs to.
  * @param t_curLineNum the current line number.
  * @return true if the table is successfully added.
  */
  virtual BOOLEAN insertFollowsRelation(const LIST_OF_STMT_NUMS& t_stmtInStmtList, int t_curLineNum) = 0;

  /** Insert relationship Next(line1, line2) into PKB.
  *   @param t_line1 the program line before
  *   @param t_line2 the program line after
  */
  virtual void insertNextRelation(PROG_LINE t_line1, PROG_LINE t_line2) = 0;

  /**
  * Inserts a follows relation in the PKB.
  * @param t_node reference to the StmtLst that this statement belongs to.
  * @param t_curLineNum the current line number.
  * @return true if the table is successfully added.
  */
  virtual BOOLEAN insertParentRelation(const LIST_OF_STMT_NUMS& t_nestedStmtLineNum, int t_curLineNum) = 0;

  /**
  * Inserts a variable that has been modified.
  * @param t_varName name of the variable being modified.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  */
  virtual void insertModifies(PROC_INDEX t_procIdx, VAR_NAME t_varName, LIST_OF_STMT_NUMS t_nestedStmtLines, STMT_NUM t_curLineNum) = 0;

  /**
  * Inserts a variable that has been used.
  * @param t_varName name of the variable that is used.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  */
  virtual void insertUses(PROC_INDEX t_procIdx, VAR_NAME t_varName, LIST_OF_STMT_NUMS t_nestedStmtLines, STMT_NUM t_curLineNum) = 0;

  /**
  * Inserts an assignment statement into the PKB
  * @param t_parentNode reference to the parent node that the assignment statement belongs to.
  * @param t_varNode reference to the variable node that is at this assignment statement.
  * @param t_exprNode reference to the expr node of the assignment statement.
  * @param t_curLineNum the current line that this assignment is at.
  */
  virtual void insertAssignStmt(STMT_NUM t_lineNum, VAR_NAME t_varName, LIST_OF_TOKENS t_stmtTokens) = 0;

  /**
  * Inserts a call statement into the PKB
  */
  virtual void insertCallStmt(PROC_INDEX t_proc1, PROC_NAME t_proc2, STMT_NUM t_lineNum) = 0;

  /**
  * Inserts a while statement into the PKB.
  * @param t_parentNode reference to the parent node that this while loop belongs to.
  * @param t_varaibleNode reference to the variable node that this while loop contains.
  * @param t_curLineNum the current line number that this while statement is at.
  * @return a reference of the while node.
  */
  virtual STMT_NUM insertWhileStmt(PROC_INDEX t_procIdx, VAR_NAME varName, LIST_OF_STMT_NUMS m_nestedStmtLineNum, STMT_NUM t_curLineNum) = 0;

  /**
  * Inserts a if statement into the PKB.
  * @param t_parentNode reference to the parent node that this while loop belongs to.
  * @param t_varaibleNode reference to the variable node that this while loop contains.
  * @param t_curLineNum the current line number that this while statement is at.
  * @return a reference of the while node.
  */
  virtual STMT_NUM insertIfStmt(PROC_INDEX t_procIdx, VAR_NAME t_varName, LIST_OF_STMT_NUMS t_nestedStmtLineNum, STMT_NUM t_curLineNum) = 0;

  /**
  * Inserts a else statement into the PKB.
  * @param t_parentNode reference to the parent node that this while loop belongs to.
  * @param t_curLineNum the current line number that this while statement is at.
  * @return a reference of the while node.
  */
  virtual STMT_NUM insertElseStmt(PROC_INDEX t_procIdx, LIST_OF_STMT_NUMS t_nestedStmtLineNum, STMT_NUM t_curLineNum) = 0;

  /**
  * Inserts a constant into the PKB.
  * @param t_constVal the constant to be added in string form.
  * @param t_curLineNum the current line of the constant.
  * @return a reference to the constant node.
  */
  virtual void insertConstant(CONSTANT_TERM t_constVal) = 0;

  virtual void insertStmtList(STMT_NUM t_line) = 0;

  ///////////////////////////////////////////////////////
  //  CallsTable methods
  ///////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////
  //  NextTable methods
  ///////////////////////////////////////////////////////

  /** To be executed after all Next relationships are added to NextTable.
  *   Populates additional design abstractions.
  */
  virtual void executeAfterAllNextInserts() = 0;

};
