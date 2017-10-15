#pragma once
#include <iostream>
#include <vector>
#include "GlobalTypeDef.h"
/**
* Handles utility methods for manipulation of strings.
*
* @author Brandon
* @date 8/10/2017
*/
class StringUtil
{
public:
  /**
  * 
  */
  static LIST_OF_RESULTS splitString(STRING t_line, STRING_CHARACTER t_delim = ' ');
  static STRING trimString(const STRING &str, const STRING &whitespace = " \t");
  static STRING reduceString(const STRING &str, const STRING& fill = " ", const STRING& whitespace = " \t");
  static void removeWhiteSpace(STRING &str);
  static void toLower(STRING &str);
};

