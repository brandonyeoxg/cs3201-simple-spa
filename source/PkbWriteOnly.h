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
  virtual StmtListNode* insertProcedure(std::string& t_procName) = 0;

  /**
  * Inserts a follows relation in the PKB.
  * @param t_node reference to the StmtLst that this statement belongs to.
  * @param t_curLineNum the current line number.
  * @return true if the table is successfully added.
  */
  virtual bool insertFollowsRelation(TNode* t_node, int t_curLineNum) = 0;

  /**
  * Inserts a variable that has been modified.
  * @param t_varName name of the variable being modified.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  virtual VariableNode* insertModifiedVariable(std::string t_varName, int t_curLineNum,
    std::list<STMT_NUM> t_nestedStmtLines) = 0;

  /**
  * Inserts a variable that has been used.
  * @param t_varName name of the variable that is used.
  * @param t_curLineNum the current line of the variable.
  * @param t_nestedStmtLines contains the lines of the statement list that this variable is nested in.
  * @return a reference of the variable node.
  */
  virtual VariableNode* insertUsesVariable(std::string t_varName, int m_curLineNum, std::list<STMT_NUM> t_nestedStmtLines) = 0;

  /**
  * Inserts an assignment statement into the PKB
  * @param t_parentNode reference to the parent node that the assignment statement belongs to.
  * @param t_varNode reference to the variable node that is at this assignment statement.
  * @param t_exprNode reference to the expr node of the assignment statement.
  * @param t_curLineNum the current line that this assignment is at.
  */
  virtual void insertAssignStmt(TNode* t_parentNode, VariableNode* t_varNode, TNode* t_exprNode, int t_curLineNum) = 0;

  /**
  * Inserts a while statement into the PKB.
  * @param t_parentNode reference to the parent node that this while loop belongs to.
  * @param t_varaibleNode reference to the variable node that this while loop contains.
  * @param t_curLineNum the current line number that this while statement is at.
  * @return a reference of the while node.
  */
  virtual StmtListNode* insertWhileStmt(TNode* t_parentNode, VariableNode* t_varNode, int t_curLineNum) = 0;

  /**
  * Inserts a constant into the PKB.
  * @param t_constVal the constant to be added in string form.
  * @param t_curLineNum the current line of the constant.
  * @return a reference to the constant node.
  */
  virtual ConstantNode* insertConstant(std::string t_constVal, int t_curLineNum) = 0;

  /**
  * Returns a plus operator.
  * @param t_left the node to the left.
  * @param t_right the node to the right.
  * @param t_curLineNum the current line number.
  * @return a reference to the plus node subtree.
  */
  virtual PlusNode* insertPlusOp(TNode* t_left, TNode* t_right, int t_curLineNum) = 0;

  ///////////////////////////////////////////////////////
  //  FollowTable methods 
  ///////////////////////////////////////////////////////
  /**
  * Method that inserts the line number (s2) to the unordered map of vectors containing line number s1 as key.
  * Returns false if current s1, s2 pair already exists in the map.
  * @param s1 an integer argument.
  * @param s2 an integer argument.
  * @return The status of the insertion.
  */
  virtual bool insertFollows(int t_s1, int t_s2) = 0;

  virtual bool insertParent(int t_s1, int t_s2) = 0;
};