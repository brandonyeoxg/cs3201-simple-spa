#pragma once
#include "GlobalTypeDef.h"

class TokeniserUtil
{
public:
  static LIST_OF_TOKENS tokeniseLine(STRING_TOKEN t_lineToTokenise);
  static bool isKeyDelimiter(STRING_TOKEN t_token);
  static bool isOperator(STRING_TOKEN t_token);
  static bool isBracket(STRING_TOKEN t_token);
  static std::string OPEN_BRACE;
  static std::string CLOSE_BRACE;
};

