#include "PatternMatch.h"

PatternMatch::PatternMatch() {
  assignStmts = new std::unordered_map<STMT_NUM, std::string>();
}

void PatternMatch::addAssignStmt(STMT_NUM t_stmtNum, std::vector<std::string> t_stmtTokens) {
  assert(assignStmts->count(t_stmtNum) == 0);

  for (int i = 0; i < (int)t_stmtTokens.size(); i++) {
    t_stmtTokens.at(i) = removeWhitespaces(t_stmtTokens.at(i));
  }

  std::string postfixStr = convertVectorToStr(convertInfixExpressionToPostfix(t_stmtTokens));

  assignStmts->insert({ t_stmtNum, postfixStr });
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

std::vector<std::string> PatternMatch::convertInfixExpressionToPostfix(std::vector<std::string> t_stmtTokens) {
  std::vector<std::string> operatorStack = std::vector<std::string>();
  std::vector<std::string> postfix = std::vector<std::string>();

  for (int i = 0; i < t_stmtTokens.size(); i++) {
    std::string token = t_stmtTokens.at(i);
    if (isOperator(token)) {
      while (isOperatorGreaterOrEqualPrec(operatorStack.back(), token)) {
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
  assert(isOperator(t_op1) && isOperator(t_op2));

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
std::string PatternMatch::removeWhitespaces(std::string t_str) {
  t_str.erase(std::remove(t_str.begin(), t_str.end(), ' '), t_str.end());
  t_str.erase(std::remove(t_str.begin(), t_str.end(), '\t'), t_str.end());
  return t_str;
}

