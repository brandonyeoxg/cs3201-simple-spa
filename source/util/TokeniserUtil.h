#pragma once
#include "../GlobalTypeDef.h"

/**
* Handles tokenising of string based on the SIMPLE grammar rules. 
* A string can be delimitered by operators (+ - * =), braces. 
* Also provides other utility that pertains to delimiting of tokens.
*/
class TokeniserUtil
{
public:
  /**
  * Returns a list of tokens from a string based on SIMPLE grammar as delimiter, ignores whitespaces or tabs.
  * For example "((x \t +   y)   *z  )  " will be tokenised into { "(", "(", "x", "+", "y", ")", "*", "z", ")"}.
  */
  static LIST_OF_TOKENS tokeniseLine(STRING_TOKEN t_lineToTokenise);

  /**
  * Returns true if the token is a key delimiter.
  * A key delimiter is defined as " ", ";", "{" , "}" or any operators ("+", "-", "*", "=").
  */
  static BOOLEAN isKeyDelimiter(STRING_TOKEN t_token);

  /**
  * Returns true if the token is an operator.
  * An operator is defined as "+", "-", "*", "=".
  */
  static BOOLEAN isOperator(STRING_TOKEN t_token);

  /**
  * Returns true if the token is a bracket.
  */
  static BOOLEAN isBracket(STRING_TOKEN t_token);

  static STRING_TOKEN OPEN_BRACKET;
  static STRING_TOKEN CLOSE_BRACKET;
};

