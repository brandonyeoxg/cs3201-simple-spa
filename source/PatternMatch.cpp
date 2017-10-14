#include "PatternMatch.h"

PatternMatch::PatternMatch() {
  assignStmts = new std::unordered_map<STMT_NUM, std::string>();
}

void PatternMatch::addAssignStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_stmtTokens) {

  //assert(assignStmts->count(t_stmtNum) == 0 && assignStmtsSubtrees->count(t_stmtNum) == 0);

  std::string stmtStr = "";
  for (int i = 0; i < (int)t_stmtTokens.size(); i++) {
    t_stmtTokens.at(i) = removeWhitespaces(t_stmtTokens.at(i));
    stmtStr += t_stmtTokens.at(i);
  }

  assignStmts->insert({ t_stmtNum, stmtStr });
}

std::list<STMT_NUM> PatternMatch::getAllStmtNumWithExactPattern(std::string t_pattern) {
  t_pattern = removeWhitespaces(t_pattern);
  std::list<STMT_NUM> stmtNums = std::list<STMT_NUM>();

  for (auto iterator = assignStmts->begin(); iterator != assignStmts->end(); iterator++) {
    if (iterator->second == t_pattern) {
      stmtNums.push_back(iterator->first);
    }
  }

  return stmtNums;
}

std::list<STMT_NUM> PatternMatch::getAllStmtNumWithSubtreePattern(std::string t_pattern) {
  t_pattern = removeWhitespaces(t_pattern);
  std::list<STMT_NUM> stmtNums = std::list<STMT_NUM>();



  return stmtNums;
}

bool PatternMatch::isExactPatternInStmt(STMT_NUM t_stmtNum, std::string t_pattern) {
  assert(assignStmts->count(t_stmtNum) == 1); // should exist
  t_pattern = removeWhitespaces(t_pattern);
  return assignStmts->at(t_stmtNum) == t_pattern;
}

bool PatternMatch::isSubtreePatternInStmt(STMT_NUM t_stmtNum, std::string t_pattern) {
  t_pattern = removeWhitespaces(t_pattern);



  return false;
}

/** Helper function to remove all whitespaces in a given string */
std::string PatternMatch::removeWhitespaces(std::string t_str) {
  t_str.erase(std::remove(t_str.begin(), t_str.end(), ' '), t_str.end());
  return t_str;
}

