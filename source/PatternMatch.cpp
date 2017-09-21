#include "PatternMatch.h"

PatternMatch PatternMatch::getInstance() {
  if (&patternMatch == 0) {
    patternMatch = PatternMatch();
  }

  return patternMatch;
}

void PatternMatch::addAssignStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_stmtTokens) {
  std::string stmtStr = "";
  for (int i = 0; i < t_stmtTokens.size(); i++) {
    t_stmtTokens.at(i) = removeWhitespaces(t_stmtTokens.at(i));
    stmtStr += t_stmtTokens.at(i);
  }

  assignStmts.insert({ t_stmtNum, stmtStr });
}

PatternMatch::PatternMatch() {
  assignStmts = std::unordered_map<STMT_NUM, string>();
  assignStmtsSubtrees = std::unordered_map<STMT_NUM, std::vector<string>>();
}

PatternMatch::~PatternMatch() {}

/** Helper function to remove all whitespaces in a given string */
std::string PatternMatch::removeWhitespaces(std::string t_str) {
  t_str.erase(std::remove(t_str.begin(), t_str.end(), ' '), t_str.end());
  return t_str;
}

