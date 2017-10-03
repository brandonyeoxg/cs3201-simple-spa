#include "PatternMatch.h"

PatternMatch * PatternMatch::patternMatch = nullptr;  // initialize as nullptr

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

std::list<STMT_NUM> PatternMatch::getAllStmtNumWithSubtreePattern(std::string t_pattern) {
  t_pattern = removeWhitespaces(t_pattern);
  std::list<STMT_NUM> stmtNums = std::list<STMT_NUM>();

  for (auto mapItr = assignStmtsSubtrees->begin(); mapItr != assignStmtsSubtrees->end(); mapItr++) {
    std::vector<std::string> subtrees = mapItr->second;
    for (auto subtreesItr : subtrees) {
      if (subtreesItr == t_pattern) {
        stmtNums.push_back(mapItr->first);
        break;  // break current loop, since this statement number is already added
      }
    }
  }

  return stmtNums;
}

bool PatternMatch::isExactPatternInStmt(STMT_NUM t_stmtNum, std::string t_pattern) {
  assert(assignStmts->count(t_stmtNum) == 1); // should exist
  t_pattern = removeWhitespaces(t_pattern);
  return assignStmts->at(t_stmtNum) == t_pattern;
}

bool PatternMatch::isSubtreePatternInStmt(STMT_NUM t_stmtNum, std::string t_pattern) {
  t_pattern = removeWhitespaces(t_pattern);

  for (auto iterator : assignStmtsSubtrees->at(t_stmtNum)) {
    if (iterator == t_pattern) {
      return true;
    }
  }

  return false;
}

std::vector<std::string> PatternMatch::getSubtreeStringsWithStmtTokens(std::vector<std::string> t_tokens) {
  return generateSubtreeStrings(t_tokens, std::vector<std::string>(), 0, t_tokens.size());
}

PatternMatch::PatternMatch() {
  assignStmts = new std::unordered_map<STMT_NUM, std::string>();
  assignStmtsSubtrees = new std::unordered_map<STMT_NUM, std::vector<std::string>>();
}

// recursive function to generate subtree strings
std::vector<std::string> PatternMatch::generateSubtreeStrings(std::vector<std::string> t_tokens, 
  std::vector<std::string> t_subtreeStrings, int t_startIndex, int t_endIndex) {

  t_subtreeStrings = addStrIfNotDuplicate(t_subtreeStrings, convertVectorToStringWithIndex(t_tokens, t_startIndex, t_endIndex));

  if (t_startIndex >= t_endIndex) {
    return t_subtreeStrings;
  }

  int indexOfOperator = findLastOperatorIndex(t_startIndex, t_endIndex, t_tokens);

  if (indexOfOperator == INVALID_INDEX) { // no more operators 
    return t_subtreeStrings;
  }

  t_subtreeStrings = generateSubtreeStrings(t_tokens, t_subtreeStrings, t_startIndex, indexOfOperator);
  t_subtreeStrings = generateSubtreeStrings(t_tokens, t_subtreeStrings, indexOfOperator + 1, t_endIndex);

  return t_subtreeStrings;
}

// find index of last operator that branches into 2 subtrees
int PatternMatch::findLastOperatorIndex(int t_startIndex, int t_endIndex, std::vector<std::string> t_tokens) {
  int indexOfOperator = INVALID_INDEX;  // index to branch subtree from

  int lastOpenBracket = findLastOpenBracketIndex(t_startIndex, t_endIndex, t_tokens);
  int lastCloseBracket = findLastCloseBracketIndex(t_startIndex, t_endIndex, t_tokens);

  int lastPlusOrMinus = findLastPlusOrMinusIndex(t_startIndex, t_endIndex, t_tokens, lastOpenBracket, lastCloseBracket);
  int lastMultiply = findLastMultiplyIndex(t_startIndex, t_endIndex, t_tokens, lastOpenBracket, lastCloseBracket);

  if (lastPlusOrMinus != INVALID_INDEX) {
    return lastPlusOrMinus;
  } else if (lastMultiply != INVALID_INDEX) {
    return lastMultiply;
  } else {  // no more operators
    return INVALID_INDEX;
  }

}

int PatternMatch::findLastOpenBracketIndex(int t_startIndex, int t_endIndex, std::vector<std::string> t_tokens) {
  for (int i = t_endIndex - 1; i >= t_startIndex; i--) {
    if (t_tokens.at(i) == BRACKET_OPEN) {
      return i;
    }
  }

  return INVALID_INDEX;
}

int PatternMatch::findLastCloseBracketIndex(int t_startIndex, int t_endIndex, std::vector<std::string> t_tokens) {
  for (int i = t_endIndex - 1; i >= t_startIndex; i--) {
    if (t_tokens.at(i) == BRACKET_CLOSE) {
      return i;
    }
  }

  return INVALID_INDEX;
}

int PatternMatch::findLastPlusOrMinusIndex(int t_startIndex, int t_endIndex, std::vector<std::string> t_tokens, int t_openBracketIndex, int t_closeBracketIndex) {
  if (t_closeBracketIndex != INVALID_INDEX) {
    for (int i = t_endIndex - 1; i > t_closeBracketIndex; i--) {
      if (isStrPlusOrMinus(t_tokens.at(i))) {
        return i;
      }
    }
  }

  if (t_openBracketIndex != INVALID_INDEX) {
    for (int i = t_openBracketIndex - 1; i >= t_startIndex; i--) {
      if (isStrPlusOrMinus(t_tokens.at(i))) {
        return i;
      }
    }
  }

  for (int i = t_endIndex - 1; i >= t_startIndex; i--) {
    if (isStrPlusOrMinus(t_tokens.at(i))) {
      return i;
    }
  }

  return INVALID_INDEX;
}

int PatternMatch::findLastMultiplyIndex(int t_startIndex, int t_endIndex, std::vector<std::string> t_tokens, int t_openBracketIndex, int t_closeBracketIndex) {
  if (t_closeBracketIndex != INVALID_INDEX) {
    for (int i = t_endIndex - 1; i > t_closeBracketIndex; i--) {
      if (t_tokens.at(i) == OPERATOR_MULTIPLY) {
        return i;
      }
    }
  }

  if (t_openBracketIndex != INVALID_INDEX) {
    for (int i = t_openBracketIndex - 1; i >= t_startIndex; i--) {
      if (t_tokens.at(i) == OPERATOR_MULTIPLY) {
        return i;
      }
    }
  }

  for (int i = t_endIndex - 1; i >= t_startIndex; i--) {
    if (t_tokens.at(i) == OPERATOR_MULTIPLY) {
      return i;
    }
  }

  return INVALID_INDEX;
}

bool PatternMatch::isStrPlusOrMinus(std::string t_str) {
  return t_str == OPERATOR_PLUS || t_str == OPERATOR_MINUS;
}

/** Given a vector of strings and start and end index, converts to a single string */
std::string PatternMatch::convertVectorToStringWithIndex(std::vector<std::string> t_vector, int t_startIndex, int t_endIndex) {
  std::string str = "";
  
  for (int i = t_startIndex; i < t_endIndex; i++) {
    str += t_vector.at(i);
  }

  return str;
}

/** Add string to list of strings only if it is not duplicate string */
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

