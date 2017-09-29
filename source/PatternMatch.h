#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>

#include ".\GlobalTypeDef.h"

/** Class that handles Pattern matching in assignment statements.
*   Utilizes singleton pattern as there should only be one instance of this class at any one time.
*   Also ensures that users of this class do not need to pass a reference of an instance around all the time.
*   Maintains data structures of preprocessed strings of all assignment statements for pattern-related queries.
*   @author jazlyn
*/

class PatternMatch {
public:
  /** Singleton pattern used. Gets instance.
  *   @return PatternMatch instance
  */
  static PatternMatch getInstance();

  /** For testing purpose. Resets internal static instance to nullptr.
  *   This clears internal data structures. Not to be used in functional code.
  */
  static void resetInstance();

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

  /** Not to be used in outside of this class. Exposed only for testing purposes.
  *   This method calls the recursive method to generate strings of all subtrees within a given expression.
  *   @param t_tokens tokens of an assignment statement's right hand side
  *   @return vector of strings of all subtrees in given expression
  */
  std::vector<std::string> getSubtreeStringsWithStmtTokens(std::vector<std::string> t_tokens);

private:
  static PatternMatch* patternMatch;  /**< Static instance of this class. */

  PatternMatch();

  std::unordered_map<STMT_NUM, std::string> * assignStmts;  /**< String representation of all assignment statements (right-hand side of equal sign) mapped to statement numbers. */
  std::unordered_map<STMT_NUM, std::vector<std::string>> * assignStmtsSubtrees;  /**< Vector of all subtree strings mapped to statement number */
  
  std::vector<std::string> generateSubtreeStrings(std::vector<std::string> t_tokens, std::vector<std::string> t_subtreeStrings, int t_startIndex, int t_endIndex);
  
  /* Helper methods */
  std::string convertVectorToStringWithIndex(std::vector<std::string> t_vector, int t_startIndex, int t_endIndex);
  std::vector<std::string> addStrIfNotDuplicate(std::vector<std::string> t_listOfStr, std::string t_str);
  std::string removeWhitespaces(std::string t_str);
};