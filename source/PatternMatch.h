#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>

#include ".\GlobalTypeDef.h"

class PatternMatch {
public:
  static PatternMatch getInstance();
  static void resetInstance();  // for testing purposes, resets internal instance to nullptr

  std::unordered_map<STMT_NUM, std::string> getAssignStmts();

  void addAssignStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_stmtTokens);

  std::list<STMT_NUM> getAllStmtNumWithExactPattern(std::string t_pattern);

  std::vector<std::string> getSubtreeStringsWithStmtTokens(std::vector<std::string> t_tokens);

private:
  static PatternMatch* patternMatch;

  PatternMatch();

  std::unordered_map<STMT_NUM, std::string> * assignStmts;  /**< String representation of all assignment statements (right-hand side of equal sign) mapped to statement numbers. */
  std::unordered_map<STMT_NUM, std::vector<std::string>> * assignStmtsSubtrees;  /**< Vector of all subtree strings mapped to statement number */
  
  std::vector<std::string> generateSubtreeStrings(std::vector<std::string> t_tokens, std::vector<std::string> t_subtreeStrings, int t_startIndex, int t_endIndex);
  std::string convertVectorToStringWithIndex(std::vector<std::string> t_vector, int t_startIndex, int t_endIndex);
  std::vector<std::string> addStrIfNotDuplicate(std::vector<std::string> t_listOfStr, std::string t_str);
  std::string removeWhitespaces(std::string t_str);
};