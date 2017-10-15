#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>

#include ".\GlobalTypeDef.h"

/** Class that handles Pattern matching in assignment statements.
*   @author jazlyn
*/

class PatternMatch {
public:
  PatternMatch();

  /** Adds an assignment statement's right-hand side expression to PatternMatch's data structure.
  *   NOTE: will assume expression is syntactically correct.
  *   Will assert false if a statement number is added more than once.
  *   @param t_stmtNum statement number
  *   @param t_stmtTokens representation of statement expression with each operator/variable/constant in an index of its own
  */
  void addAssignStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_stmtTokens);

  /** Given statement number, checks if given pattern matches the statement expression exactly.
  *   Assumes statement number refers to existing assignment statement.
  *   @param t_stmtNum statement number to check
  *   @param t_pattern pattern string to check (extra whitespaces will be ignored)
  *   @return true if pattern matches statement expression, else false
  */
  bool isExactPatternInStmt(STMT_NUM t_stmtNum, std::string t_pattern);

  /** Given statement number, checks if given pattern string matches the statement expression by subtree.
  *   Assumes statement number refers to existing assignment statement.
  *   @param t_stmtNum statement number to check
  *   @param t_pattern pattern string to check (extra whitespaces will be ignored)
  *   @return true if pattern matches statement expression, else false
  */
  bool isSubtreePatternInStmt(STMT_NUM t_stmtNum, std::string t_pattern);

  /** Given statement number, checks if given pattern matches the statement expression exactly.
  *   Assumes statement number refers to existing assignment statement.
  *   @param t_stmtNum statement number to check
  *   @param t_pattern pattern to check, in tokenized form (extra whitespaces will be ignored)
  *   @return true if pattern matches statement expression, else false
  */
  bool isExactPatternInStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_pattern);

  /** Given statement number, checks if given pattern string matches the statement expression by subtree.
  *   Assumes statement number refers to existing assignment statement.
  *   @param t_stmtNum statement number to check
  *   @param t_pattern pattern to check, in tokenized form (extra whitespaces will be ignored)
  *   @return true if pattern matches statement expression, else false
  */
  bool isSubtreePatternInStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_pattern);

  /** Gets all statement numbers with exact pattern match to assignment expression.
  *   @param t_pattern pattern to be matched, in tokenized form (extra whitespaces will be ignored)
  *   @return list of statement numbers
  */
  std::list<STMT_NUM> getAllStmtNumWithExactPattern(std::vector<std::string> t_pattern);

  /** Gets all statement numbers with subtree pattern match to assignment expression.
  *   @param t_pattern pattern to be matched, in tokenized form (extra whitespaces will be ignored)
  *   @return list of statement numbers
  */
  std::list<STMT_NUM> getAllStmtNumWithSubtreePattern(std::vector<std::string> t_pattern);

  std::string getPostfixStrWithTokens(std::vector<std::string> t_tokens);

  //////// for debugging
  //std::unordered_map<STMT_NUM, std::string> getAssignStmts() { return m_assignStmts; }

private:
  std::unordered_map<STMT_NUM, std::string> m_assignStmts;  /**< String representation of all assignment statements (right-hand side of equal sign) mapped to statement numbers. */
 
  std::vector<std::string> convertInfixExpressionToPostfix(std::vector<std::string> t_stmtTokens);

  bool isOperator(std::string t_str);

  // is first operator greater precedence than second operator
  // will return false if op1 is not an operator
  bool isOperatorGreaterOrEqualPrec(std::string t_op1, std::string t_op2);

  int getPrecedenceLevel(std::string t_operator);

  std::string convertVectorToStr(std::vector<std::string> t_vector);

  /* Helper methods */
  void removeWhitespaces(std::string &t_str);

  void removeWhitespacesFromVector(std::vector<std::string> &t_stmtTokens);
};