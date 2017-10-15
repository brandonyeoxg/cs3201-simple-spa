#include "PatternMatch.h"

PatternMatch::PatternMatch() {
  m_assignStmts = std::unordered_map<STMT_NUM, std::string>();
}

void PatternMatch::addAssignStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_stmtTokens) {
  assert(m_assignStmts.count(t_stmtNum) == 0);

  removeWhitespacesFromVector(t_stmtTokens);
  std::string postfixStr = getPostfixStrWithTokens(t_stmtTokens);
  m_assignStmts.insert({ t_stmtNum, postfixStr });
}

bool PatternMatch::isExactPatternInStmt(STMT_NUM t_stmtNum, std::string t_pattern) {
  assert(m_assignStmts.count(t_stmtNum) == 1);
  removeWhitespaces(t_pattern);
  return m_assignStmts.at(t_stmtNum) == t_pattern;
}

bool PatternMatch::isSubtreePatternInStmt(STMT_NUM t_stmtNum, std::string t_pattern) {
  assert(m_assignStmts.count(t_stmtNum) == 1);
  removeWhitespaces(t_pattern);
  return m_assignStmts.at(t_stmtNum).find(t_pattern) != std::string::npos;
}

bool PatternMatch::isExactPatternInStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_pattern) {
  return isExactPatternInStmt(t_stmtNum, getPostfixStrWithTokens(t_pattern));
}

bool PatternMatch::isSubtreePatternInStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_pattern) {
  return isSubtreePatternInStmt(t_stmtNum, getPostfixStrWithTokens(t_pattern));
}

std::list<STMT_NUM> PatternMatch::getAllStmtNumWithExactPattern(std::vector<std::string> t_pattern) {
  removeWhitespacesFromVector(t_pattern);
  std::string postfixPattern = getPostfixStrWithTokens(t_pattern);
  std::list<STMT_NUM> stmtNums = std::list<STMT_NUM>();

  for (auto iterator : m_assignStmts) {
    if (isExactPatternInStmt(iterator.first, postfixPattern)) {
      stmtNums.push_back(iterator.first);
    }
  }

  return stmtNums;
}

std::list<STMT_NUM> PatternMatch::getAllStmtNumWithSubtreePattern(std::vector<std::string> t_pattern) {
  removeWhitespacesFromVector(t_pattern);
  std::string postfixPattern = getPostfixStrWithTokens(t_pattern);
  std::list<STMT_NUM> stmtNums = std::list<STMT_NUM>();

  for (auto iterator : m_assignStmts) {
    if (isSubtreePatternInStmt(iterator.first, postfixPattern)) {
      stmtNums.push_back(iterator.first);
    }
  }

  return stmtNums;
}

std::string PatternMatch::getPostfixStrWithTokens(std::vector<std::string> t_tokens) {
  removeWhitespacesFromVector(t_tokens);
  return convertVectorToStr(convertInfixExpressionToPostfix(t_tokens));
}

std::vector<std::string> PatternMatch::convertInfixExpressionToPostfix(std::vector<std::string> t_stmtTokens) {
  std::vector<std::string> operatorStack = std::vector<std::string>();
  std::vector<std::string> postfix = std::vector<std::string>();

  for (auto token : t_stmtTokens) {
    if (isOperator(token)) {
      while (!operatorStack.empty() && isOperatorGreaterOrEqualPrec(operatorStack.back(), token)) {
        postfix.push_back(operatorStack.back());  //push operators into postfix expression
        operatorStack.pop_back();
      }
      operatorStack.push_back(token);
    } else if (token == BRACKET_OPEN) {
      operatorStack.push_back(token);
    } else if (token == BRACKET_CLOSE) {
      while (operatorStack.back() != BRACKET_OPEN) {
        postfix.push_back(operatorStack.back());  //push operators into postfix expression
        operatorStack.pop_back();
      }
      operatorStack.pop_back(); // remove the open bracket
    } else {  // either variable or constant
      postfix.push_back(token);
    }
  }

  while (!operatorStack.empty()) {
    postfix.push_back(operatorStack.back());  //push operators into postfix expression
    operatorStack.pop_back();
  }

  return postfix;
}

bool PatternMatch::isOperator(std::string t_str) {
  return t_str == OPERATOR_PLUS || t_str == OPERATOR_MINUS || t_str == OPERATOR_MULTIPLY;
}

bool PatternMatch::isOperatorGreaterOrEqualPrec(std::string t_op1, std::string t_op2) {
  assert(isOperator(t_op2));
  if (!isOperator(t_op1)) { //  means is a bracket
    return false;
  }

  int precedence1 = getPrecedenceLevel(t_op1);
  int precedence2 = getPrecedenceLevel(t_op2);
  
  return precedence1 >= precedence2;
}

int PatternMatch::getPrecedenceLevel(std::string t_operator) {
  if (t_operator == OPERATOR_PLUS || t_operator == OPERATOR_MINUS) {
    return 0;
  } else {  // means equal to multiply
    return 1;
  }
}

std::string PatternMatch::convertVectorToStr(std::vector<std::string> t_vector) {
  std::string str = "";
  for (auto iter : t_vector) {
    str += iter;
  }
  return str;
}

/** Helper function to remove all whitespaces in a given string
    should remove tabs as well (TEST THIS)
*/
void PatternMatch::removeWhitespaces(std::string &t_str) {
  t_str.erase(std::remove(t_str.begin(), t_str.end(), ' '), t_str.end());
  t_str.erase(std::remove(t_str.begin(), t_str.end(), '\t'), t_str.end());
}

void PatternMatch::removeWhitespacesFromVector(std::vector<std::string> &t_tokens) {
  for (int i = 0; i < (int)t_tokens.size(); i++) {
    removeWhitespaces(t_tokens.at(i));
  }
}

