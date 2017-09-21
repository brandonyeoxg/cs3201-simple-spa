#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

#include ".\GlobalTypeDef.h"

class PatternMatch {
public:
  static PatternMatch getInstance();

  void addAssignStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_stmtTokens);

private:
  PatternMatch();
  ~PatternMatch();

  static PatternMatch patternMatch;
  std::unordered_map<STMT_NUM, string> assignStmts;  /**< String representation of all assignment statements (right-hand side of equal sign) mapped to statement numbers. */ 
  std::unordered_map<STMT_NUM, std::vector<string>> assignStmtsSubtrees;  /**< Vector of all subtree strings mapped to statement number */
  
  std::string removeWhitespaces(std::string t_str);
};