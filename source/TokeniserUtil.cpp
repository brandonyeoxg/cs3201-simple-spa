#include "TokeniserUtil.h"
#include "StringUtil.h"

std::string TokeniserUtil::OPEN_BRACE = std::string("(");
std::string TokeniserUtil::CLOSE_BRACE = std::string(")");

LIST_OF_TOKENS TokeniserUtil::tokeniseLine(STRING_TOKEN t_lineToTokenise) {
  STRING_TOKEN formattedString = StringUtil::reduceString(t_lineToTokenise);
  LIST_OF_TOKENS tokens;
  STRING_TOKEN token = EMPTY_LINE;
  for (auto itr = formattedString.begin(); itr != formattedString.end(); itr++) {
    const STRING_TOKEN curStrChar = std::string(1, (*itr));
    if (isKeyDelimiter(curStrChar) && token != EMPTY_LINE) { // Tokenise the words
      tokens.push_back(token);
      token = EMPTY_LINE;
    }
    if (isOperator(curStrChar)) { // Tokenise operator
      tokens.push_back(curStrChar);
      token = EMPTY_LINE;
      continue;
    }
    if (curStrChar != " ") {
      token += (*itr);
    }
  }
  if (token.length() > 0) {
    tokens.push_back(token);
    token = EMPTY_LINE;
  }
  return tokens;
}

bool TokeniserUtil::isKeyDelimiter(STRING_TOKEN t_token) {
  return isOperator(t_token)
    || t_token == " "
    || t_token == ";"
    || t_token == "{" 
    || t_token == "}";
}

bool TokeniserUtil::isOperator(STRING_TOKEN t_token) {
  return t_token == "+"
    || t_token == "-"
    || t_token == "*"
    || t_token == "="
    || isBracket(t_token);
}

bool TokeniserUtil::isBracket(STRING_TOKEN t_token) {
  return t_token == "(" || t_token == ")";
}