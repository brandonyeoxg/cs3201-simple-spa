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

  /** Adds an assignment statement's right hand side expression to PatternMatch's data structures.
  *   PatternMatch will generate strings of all possible pattern matches and store them by the statement number.
  *   NOTE: will assume expression is syntactically correct.
  *   Will check with assert that a statement number is not added more than once.
  *   @param t_stmtNum statement number
  *   @param t_stmtTokens representation of statement expression with each operator/variable/constant in an index of its own
  */
  void addAssignStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_stmtTokens);

  /** Gets all statement numbers with exact pattern match to assignment expression.
  *   @param t_pattern pattern string to be matched (extra whitespaces will be ignored)
  *   @return list of statement numbers
  */
  std::list<STMT_NUM> getAllStmtNumWithExactPattern(std::string t_pattern);

  /** Gets all statement numbers with subtree pattern match to assignment expression.
  *   @param t_pattern pattern string to be matched (extra whitespaces will be ignored)
  *   @return list of statement numbers
  */
  std::list<STMT_NUM> getAllStmtNumWithSubtreePattern(std::string t_pattern);

  /** Given statement number, checks if given pattern string matches the statement expression exactly.
  *   Assumes statement number refers to valid assignment statement.
  *   @param t_stmtNum statement number to check
  *   @param t_pattern pattern string to check (extra whitespaces will be ignored)
  *   @return true if pattern matches statement expression, else false
  */
  bool isExactPatternInStmt(STMT_NUM t_stmtNum, std::string t_pattern);

  /** Given statement number, checks if given pattern string matches the statement expression by subtree.
  *   Assumes statement number refers to valid assignment statement.
  *   @param t_stmtNum statement number to check
  *   @param t_pattern pattern string to check (extra whitespaces will be ignored)
  *   @return true if pattern matches statement expression, else false
  */
  bool isSubtreePatternInStmt(STMT_NUM t_stmtNum, std::string t_pattern);

  /** Not to be used in outside of this class. Exposed only for testing purposes.
  *   This method calls the recursive method to generate strings of all subtrees within a given expression.
  *   @param t_tokens tokens of an assignment statement's right hand side
  *   @return vector of strings of all subtrees in given expression
  */
  std::vector<std::string> getSubtreeStringsWithStmtTokens(std::vector<std::string> t_tokens);

private:
  std::unordered_map<STMT_NUM, std::string> * assignStmts;  /**< String representation of all assignment statements (right-hand side of equal sign) mapped to statement numbers. */
  std::unordered_map<STMT_NUM, std::vector<std::string>> * assignStmtsSubtrees;  /**< Vector of all subtree strings mapped to statement number */
  
  std::vector<std::string> generate(std::vector<std::string> t_tokens, std::vector<std::string> * t_subtreeStrings);

  std::vector<std::string> generateSubtreeStrings(std::vector<std::string> t_tokens, std::vector<std::string> t_subtreeStrings, int t_startIndex, int t_endIndex);
  std::vector<std::string> processBrackets(std::vector<std::string> t_tokens, std::vector<std::string> * t_subtreeStrings, int t_startIndex, int t_endIndex);
  int findLastOperatorIndex(int t_startIndex, int t_endIndex, std::vector<std::string> t_tokens);
  int findFirstOpenBracketIndex(int t_startIndex, int t_endIndex, std::vector<std::string> t_tokens);
  int findLastCloseBracketIndex(int t_startIndex, int t_endIndex, std::vector<std::string> t_tokens);
  int findLastPlusOrMinusIndex(int t_startIndex, int t_endIndex, std::vector<std::string> t_tokens);
  int findLastMultiplyIndex(int t_startIndex, int t_endIndex, std::vector<std::string> t_tokens);

  bool isStrPlusOrMinus(std::string t_str);

  /* Helper methods */
  std::string convertVectorToStringWithIndex(std::vector<std::string> t_vector, int t_startIndex, int t_endIndex);
  void addStrIfNotDuplicate(std::vector<std::string> * t_listOfStr, std::string t_str);
  std::string removeWhitespaces(std::string t_str);
  std::string removeBrackets(std::string t_str);
};