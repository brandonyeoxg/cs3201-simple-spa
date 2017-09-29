#include "PatternMatch.h"

PatternMatch * PatternMatch::patternMatch = nullptr;

PatternMatch PatternMatch::getInstance() {
  if (patternMatch == nullptr) {
    patternMatch = new PatternMatch();
  }

  return *patternMatch;
}

void PatternMatch::resetInstance() {
  patternMatch = nullptr;
}

void PatternMatch::addAssignStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_stmtTokens) {

  assert(assignStmts->count(t_stmtNum) == 0 && assignStmtsSubtrees->count(t_stmtNum) == 0);

  std::string stmtStr = "";
  for (int i = 0; i < (int)t_stmtTokens.size(); i++) {
    t_stmtTokens.at(i) = removeWhitespaces(t_stmtTokens.at(i));
    stmtStr += t_stmtTokens.at(i);
  }

  assignStmts->insert({ t_stmtNum, stmtStr });
  assignStmtsSubtrees->insert({ t_stmtNum, getSubtreeStringsWithStmtTokens(t_stmtTokens) });
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

std::vector<std::string> PatternMatch::getSubtreeStringsWithStmtTokens(std::vector<std::string> t_tokens) {
  return generateSubtreeStrings(t_tokens, std::vector<std::string>(), 0, t_tokens.size());
}

PatternMatch::PatternMatch() {
  assignStmts = new std::unordered_map<STMT_NUM, string>();
  assignStmtsSubtrees = new std::unordered_map<STMT_NUM, std::vector<string>>();
}

// recursive function to generate subtree strings
std::vector<std::string> PatternMatch::generateSubtreeStrings(std::vector<std::string> t_tokens, 
  std::vector<std::string> t_subtreeStrings, int t_startIndex, int t_endIndex) {

  t_subtreeStrings = addStrIfNotDuplicate(t_subtreeStrings, convertVectorToStringWithIndex(t_tokens, t_startIndex, t_endIndex));

  if (t_startIndex >= t_endIndex) {
    return t_subtreeStrings;
  }

  int indexOfOperator = INVALID_INDEX;
  int lastPlusOrMinus = INVALID_INDEX;
  int lastMultiply = INVALID_INDEX;

  // start search from back of vector
  for (int i = t_endIndex - 1; i >= t_startIndex; i--) {
    if (lastPlusOrMinus == INVALID_INDEX && 
      (t_tokens.at(i) == OPERATOR_PLUS || t_tokens.at(i) == OPERATOR_MINUS)) {
      lastPlusOrMinus = i;
    }

    if (lastMultiply == INVALID_INDEX && t_tokens.at(i) == OPERATOR_MULTIPLY) {
      lastMultiply = i;
    }

    if (lastPlusOrMinus != INVALID_INDEX && lastMultiply != INVALID_INDEX) {
      break;
    }
  }

  if (lastPlusOrMinus != INVALID_INDEX) {
    indexOfOperator = lastPlusOrMinus;
  } else if (lastMultiply != INVALID_INDEX) {
    indexOfOperator = lastMultiply;
  } else {  // no more operators
    return t_subtreeStrings;
  }

  t_subtreeStrings = generateSubtreeStrings(t_tokens, t_subtreeStrings, t_startIndex, indexOfOperator);
  t_subtreeStrings = generateSubtreeStrings(t_tokens, t_subtreeStrings, indexOfOperator + 1, t_endIndex);

  return t_subtreeStrings;
}

/** Given a vector of strings and start and end index, converts to a single string */
std::string PatternMatch::convertVectorToStringWithIndex(std::vector<std::string> t_vector, int t_startIndex, int t_endIndex) {
  std::string str = "";
  
  for (int i = t_startIndex; i < t_endIndex; i++) {
    str += t_vector.at(i);
  }

  return str;
}

// Add string to list of strings only if it is not duplicate string
std::vector<std::string> PatternMatch::addStrIfNotDuplicate(std::vector<std::string> t_listOfStr, std::string t_str) {

  for (int i = 0; i < (int)t_listOfStr.size(); i++) {
    if (t_listOfStr.at(i) == t_str) {
      return t_listOfStr;
    }
  }

  t_listOfStr.push_back(t_str);

  return t_listOfStr;
}

/** Helper function to remove all whitespaces in a given string */
std::string PatternMatch::removeWhitespaces(std::string t_str) {
  t_str.erase(std::remove(t_str.begin(), t_str.end(), ' '), t_str.end());
  return t_str;
}

