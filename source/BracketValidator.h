#pragma once
#include "GlobalTypeDef.h"
#include <stack>

/**
* Represents a validator to validate if the expression is wellformed.
*
* @author Brandon
* @date 8/10/2017
*/
class BracketValidator
{
public:
  static std::string OPEN_BRACE;
  static std::string CLOSE_BRACE;
  BracketValidator() {};
  ~BracketValidator() {};

  /**
  * Inserts a token representing an open brace.
  *
  * @param t_brace should only be "(" or ")".
  */
  void insertBracketToken(const STRING_TOKEN& t_brace);
  
  /**
  * Returns true if the expression is wellformed with respect to open braces.
  */
  bool isWellFormed();
  
  /**
  * Used for testing purposes only.
  * Returns the brace stack for testing.
  */
  const std::stack<STRING_TOKEN>& getBraceStack();
private:
  std::stack<STRING_TOKEN> m_braces;
};

